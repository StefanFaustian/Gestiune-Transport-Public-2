#include "Manager.h"
#include "Exceptii.h"
#include "Autobuz.h"
#include "Tramvai.h"
#include "Troleibuz.h"
#include "Utilitare.h"
#include <fstream>
#include <sstream>
#include <algorithm> // pentru transform

Manager::Manager(const std::string& nume) : numeManager(nume) {}

void Manager::afis() const {
    for (auto& dep : depouri) {
        std::cout << dep << std::endl;
    }
    for (auto& linie : linii)
        std::cout << linie << std::endl;
}

std::shared_ptr<Vehicul> Manager::gasesteVehiculGlobal(const std::string& nrInmatriculare) {
    for (auto& dep : depouri) {
        auto v = dep.cautaVehicul(nrInmatriculare);
        if (v)
            return v;
    }
    return nullptr; // cazul in care nu a fost gasit vehiculul cautat
}

void Manager::raportReviziiUrgente() {
    std::cout << "\n--- RAPORT VEHICULE CE NECESITA REVIZIE URGENTA ---\n";
    bool gasit = false;

    for (const auto& nrInmatriculare : numereInmatriculate) {
        const auto vehicul = gasesteVehiculGlobal(nrInmatriculare);
        if (vehicul && vehicul->necesitaRevizie()) {
            std::cout << "Vehiculul " << vehicul->getNrInmatriculare() << " necesita revizie urgent!\n";
            gasit = true;
        }
    }
    if (!gasit) {
        std::cout << "Toata flota este in stare perfecta de functionare!\n";
    }
    std::cout << "---------------------------------------------------\n";
}

void Manager::trimiteInService(const std::string& nrInmatriculare) {
    if (Linie::esteVehiculActiv(nrInmatriculare)) {
        throw EroareOperatiune("Vehiculul " + nrInmatriculare + " este pe traseu! Asteapta retragerea catre depou mai intai!");
    }

    const auto vehicul = gasesteVehiculGlobal(nrInmatriculare);
    if (!vehicul) {
        throw EroareOperatiune("Vehiculul " + nrInmatriculare + " nu exista in sistem!");
    }
    vehicul->efectueazaRevizie();
}

void Manager::incarcaDepouri(const std::string& numeFisier) {
    std::ifstream fin(numeFisier);
    if (!fin.is_open()) {
        throw EroareFisier("Fisierul " + numeFisier + " nu poate fi gasit.");
    }

    std::string rand;
    while (std::getline(fin,rand)) {
        // sunt excluse liniile goale si cele comentate cu '#'
        trim(rand);
        if (rand.empty() || rand[0] == '#') continue;

        std::stringstream randParse(rand); // parsare pentru split
        std::string numeDepou, capacitateStr, numeFisierSursa;
        if (!(std::getline(randParse, numeDepou, ',') &&
        std::getline(randParse, capacitateStr, ',') &&
        std::getline(randParse, numeFisierSursa))) { //  || numeDepou.empty() || numeFisierSursa.empty()

            throw EroareFisier("Citirea a esuat pentru ca linia " + rand + " nu respecta formatul de input");
        }
        trim(numeDepou), trim(capacitateStr), trim(numeFisierSursa);
        try {
            int capacitate = safeStoi(capacitateStr,"capacitate depou");
            depouri.emplace_back(Depou<Vehicul>(numeDepou,capacitate));
            incarcaFlota(depouri.back(),numeFisierSursa);
        } catch (const EroareGenerala& err) {
            std::cerr << err.what() << '\n';
        }
    }
}

void Manager::adaugaVehiculNou(const std::string& numeDepou, const std::shared_ptr<Vehicul>& vehiculNou) {
    const std::string nr = vehiculNou->getNrInmatriculare();

    if (numereInmatriculate.count(nr)) {
        throw EroareOperatiune("Vehiculul cu numarul " + nr + " exista deja in sistem!");
    }

    const auto itDepou = std::find_if(depouri.begin(), depouri.end(),
        [&numeDepou](const Depou<Vehicul>& d) { return d.getNume() == numeDepou; });

    if (itDepou == depouri.end())
        throw EroareOperatiune("Depoul '" + numeDepou + "' nu există!");

    itDepou->adaugaVehicul(vehiculNou);
    numereInmatriculate.insert(nr);

    std::cout << "Vehiculul " << nr << " a fost adaugat cu succes in " << numeDepou<< ".\n";
}

void Manager::stergeVehicul(const std::string& nrInmatriculare) {
    // Un vehicul care este activ pe o linie nu poate fi sters
    if (Linie::esteVehiculActiv(nrInmatriculare)) {
        throw EroareOperatiune("Vehiculul " + nrInmatriculare + " este pe traseu. Asteapta retragerea catre depou mai intai!");
    }

    bool gasitSiSters = false;
    for (auto& depou : depouri) {
        if (depou.eliminaVehicul(nrInmatriculare)) {
            gasitSiSters = true;
            break;
        }
    }

    if (gasitSiSters) {
        numereInmatriculate.erase(nrInmatriculare);
        std::cout << "Vehiculul " << nrInmatriculare << " a fost scos definitiv din flota.\n";
    } else {
        throw EroareOperatiune("Vehiculul cu numarul " + nrInmatriculare + " nu a fost gasit in sistem.");
    }
}

void Manager::incarcaFlota(Depou<Vehicul>& depou, const std::string& numeFisier) {
    std::ifstream fin(numeFisier);
    if (!fin.is_open()) {
        throw EroareOperatiune("Fisierul " + numeFisier + " nu poate fi gasit.");
    }

    std::string rand;
    while (std::getline(fin,rand)) {
        // sunt excluse liniile goale si cele comentate cu '#'
        trim(rand);
        if (rand.empty() || rand[0] == '#') continue;

        std::string tipVehicul, nrInmatriculare, capacitateStr, kmStr;
        try {
            std::stringstream randParse(rand);
            if (!(std::getline(randParse, tipVehicul, ',') &&
                  std::getline(randParse, nrInmatriculare, ',') &&
                  std::getline(randParse, capacitateStr, ','))) {
                throw EroareFisier("Citirea a esuat pentru ca linia " + rand + " nu respecta formatul de input.");
            }
            if (numereInmatriculate.count(nrInmatriculare))
                throw EroareOperatiune("Numarul de inmatriculare " + nrInmatriculare + " este deja folosit.");


            trim(tipVehicul), trim(nrInmatriculare), trim(capacitateStr), trim(kmStr);
            int capacitate = safeStoi(capacitateStr,"capacitate vehicul");

            // Tratare cazuri case-sensitive
            std::transform(tipVehicul.begin(),tipVehicul.end(),tipVehicul.begin(),tolower);
            std::transform(nrInmatriculare.begin(),nrInmatriculare.end(),nrInmatriculare.begin(),toupper);

            std::shared_ptr<Vehicul> vehicul = nullptr;
            if (tipVehicul == "autobuz") {
                std::string motor;
                if (!std::getline(randParse, motor, ','))
                    throw EroareFisier("Citirea a esuat pentru ca linia " + rand + " nu respecta formatul de input pentru autobuze.");

                trim(motor);
                std::getline(randParse, kmStr, ',');
                int km = kmStr.empty() ? 0 : safeStoi(kmStr, "kilometri la bord");
                vehicul = std::make_shared<Autobuz>(nrInmatriculare,capacitate,motor,km);

            } else if (tipVehicul == "tramvai") {
                std::string nrVagoaneStr;
                if (!std::getline(randParse, nrVagoaneStr, ','))
                    throw EroareFisier("Citirea a esuat pentru ca linia " + rand + " nu respecta formatul de input pentru tramvaie.");

                trim(nrVagoaneStr);
                int nrVagoane = safeStoi(nrVagoaneStr,"numar vagoane tramvai");
                std::getline(randParse, kmStr, ',');
                int km = kmStr.empty() ? 0 : safeStoi(kmStr, "kilometri la bord");
                vehicul = std::make_shared<Tramvai>(nrInmatriculare,capacitate,nrVagoane,km);

            } else if (tipVehicul == "troleibuz") {
                std::string baterieStr;
                if (!std::getline(randParse, baterieStr, ','))
                    throw EroareFisier("Citirea a esuat pentru ca linia " + rand + " nu respecta formatul de input pentru troleibuze.");

                trim(baterieStr);
                std::transform(baterieStr.begin(), baterieStr.end(), baterieStr.begin(), tolower);
                bool baterie = (baterieStr == "true" || baterieStr == "1");
                std::getline(randParse, kmStr, ',');
                int km = kmStr.empty() ? 0 : safeStoi(kmStr, "kilometri la bord");
                vehicul = std::make_shared<Troleibuz>(nrInmatriculare,capacitate,baterie,km);

            } else {
                throw EroareFisier("Tip de vehicul neidentificat: " + tipVehicul + '\n');
            }

            depou.adaugaVehicul(vehicul);
            numereInmatriculate.insert(nrInmatriculare);

        } catch (const EroareGenerala& err) {
            std::cerr << err.what() << '\n';
        }
    }
}

void Manager::incarcaLinii(const std::string& numeFisier) {
    std::ifstream fin(numeFisier);
    if (!fin.is_open()) {
        throw EroareFisier("Fisierul " + numeFisier + " nu poate fi gasit.");
    }

    std::string rand;
    while (std::getline(fin, rand)) {
        trim(rand);
        if (rand.empty() || rand[0] == '#') continue;

        if (rand[0] == '>') {  // se citesc date despre despre linie
            std::string dateRand = rand.substr(1); // Taierea caracterului '>'
            std::stringstream randParse(dateRand);
            std::string indicativ, tipStr;

            std::getline(randParse, indicativ, ',');
            std::getline(randParse, tipStr, ',');
            trim(indicativ); trim(tipStr);
            std::transform(tipStr.begin(), tipStr.end(), tipStr.begin(), toupper);

            TipVehiculLinie tip;
            if (tipStr == "AUTOBUZ" )  tip = TipVehiculLinie::AUTOBUZ;
            else if (tipStr == "TRAMVAI")  tip = TipVehiculLinie::TRAMVAI;
            else if (tipStr == "TROLEIBUZ")  tip = TipVehiculLinie::TROLEIBUZ;
            else throw EroareFisier("Tip de vehicul asignat liniei " + indicativ + " neidentificat");

            linii.emplace_back(indicativ, tip);
        } else { // se citesc statiile
            if (linii.empty()) throw EroareFisier("Statie gasita inainte de definirea unei linii!");

            std::stringstream randParse(rand);
            std::string numeStatie, distantaStr;
            std::getline(randParse, numeStatie, ',');
            std::getline(randParse, distantaStr, ',');
            trim(numeStatie); trim(distantaStr);

            int distanta = safeStoi(distantaStr, "distanta statie");
            linii.back().adaugaStatie(numeStatie, distanta);
        }
    }
}

void Manager::alocaVehiculPeLinie(const std::string& nrInmatriculare, const std::string& indicativLinie) {
    // cautarea liniei
    const auto& itLinie = std::find_if(linii.begin(), linii.end(),
        [&indicativLinie](const Linie& l) { return l.getIndicativ() == indicativLinie; });

    if (itLinie == linii.end()) {
        throw EroareOperatiune("Linia " + indicativLinie + " nu exista!");
    }

    auto vehicul = gasesteVehiculGlobal(nrInmatriculare);
    if (!vehicul) {
        throw EroareOperatiune("Vehiculul " + nrInmatriculare + " nu exista!");
    }

    if (itLinie->getTipVehicul() == TipVehiculLinie::AUTOBUZ && !dynamic_cast<Autobuz*>(vehicul.get()))
        throw EroareOperatiune("Nu poti pune acest vehicul (" + nrInmatriculare + ") pe o linie de AUTOBUZ!");
    if (itLinie->getTipVehicul() == TipVehiculLinie::TRAMVAI && !dynamic_cast<Tramvai*>(vehicul.get()))
        throw EroareOperatiune("Nu poti pune acest vehicul (" + nrInmatriculare + ") pe o linie de TRAMVAI!");
    if (itLinie->getTipVehicul() == TipVehiculLinie::TROLEIBUZ && !dynamic_cast<Troleibuz*>(vehicul.get()))
        throw EroareOperatiune("Nu poti pune acest vehicul (" + nrInmatriculare + ") pe o linie de TROLEIBUZ!");

    itLinie->trimiteVehicul(nrInmatriculare);
}

void Manager::incheieZiuaDeLucru() {
    const int CICLURI_PE_ZI = 15; // Numarul de cicluri realizate de vehicul pe linia asignata
    const int KM_ACCES = 10; // 5km de la depou la traseu + 5km inapoi

    for (auto& linie : linii) {
        const int kmTotalNou = (linie.calculeazaDistantaTurRetur() * CICLURI_PE_ZI) + KM_ACCES;

        for (const auto& nrInmatriculare : linie.getVehiculePeTraseu()) {
            const auto vehicul = gasesteVehiculGlobal(nrInmatriculare);
            if (vehicul)
                vehicul->actualizeazaKM(kmTotalNou);
        }
        linie.retrageToataFlota();
    }

    std::cout << "Ziua de lucru s-a incheiat! Toate vehiculele s-au intors in depouri.\n";
}