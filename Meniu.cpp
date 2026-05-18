#include "Meniu.h"
// #include "Autobuz.h"
// #include "Tramvai.h"
// #include "Troleibuz.h"
#include "Depou.h"
#include "Exceptii.h"
#include "VehiculBuilder.h"
#include <iostream>

Meniu::Meniu(Manager& m) : manager(m) {}

void Meniu::afiseazaOptiuni() const {
    std::cout << "\n========== MENIU DISPECERAT ==========\n"
              << "1. Afisare Flota si Linii (Inventar)\n"
              << "2. Adauga un vehicul nou intr-un depou\n"
              << "3. Sterge un vehicul dintr-un depou\n"
              << "4. Aloca vehicul pe traseu (Inceputul unei zile)\n"
              << "5. Incheie ziua de lucru (Actualizare KM)\n"
              << "6. Afiseaza vehicule ce necesita revizie\n"
              << "7. Trimite vehicul in service (Resetare KM revizie)\n"
              << "0. Iesire din program\n"
              << "==========================================\n"
              << "Alege o optiune: ";
}

std::string Meniu::alegeVehiculDinDepou() const {
    const auto& depouri = manager.getDepouri();

    std::cout << "\n--- SELECTEAZA DEPOUL ---\n";
    for (int i = 0; i < depouri.size(); i++) {
        std::cout << i + 1 << ". " << depouri[i].getNume() << "\n";
    }
    std::cout << "Alege numarul depoului: ";
    int optDepou; std::cin >> optDepou;
    if (optDepou < 1 || optDepou > depouri.size()) throw EroareOperatiune("Depou invalid!");

    const auto& depouAles = depouri[optDepou - 1];
    const auto& flota = depouAles.getFlota();
    if (flota.empty()) throw EroareOperatiune("Acest depou nu are vehicule!");

    std::cout << "\n--- VEHICULE IN " << depouAles.getNume() << " ---\n";
    for (int i = 0; i < flota.size(); i++) {
        std::cout << i + 1 << ". " << flota[i]->getNrInmatriculare() << "\n";
    }
    std::cout << "Alege vehicululul: ";
    int optVehicul; std::cin >> optVehicul;
    if (optVehicul < 1 || optVehicul > flota.size()) throw EroareOperatiune("Vehicul invalid!");

    return flota[optVehicul - 1]->getNrInmatriculare();
}

std::string Meniu::alegeLinie() const {
    const auto& linii = manager.getLinii();

    std::cout << "\n--- SELECTEAZA LINIA ---\n";
    for (int i = 0; i < linii.size(); i++) {
        std::cout << i + 1 << ". Linia " << linii[i].getIndicativ() << "\n";
    }
    std::cout << "Alege linia: ";
    int optLinie; std::cin >> optLinie;
    if (optLinie < 1 || optLinie > linii.size()) throw EroareOperatiune("Linie invalida!");

    return linii[optLinie - 1].getIndicativ();
}

void Meniu::trateazaAdaugareVehicul() const {
    const auto& depouri = manager.getDepouri();
    std::string tip, nrInmat, atributSpecific;
    int capacitate, km;

    std::cout << "\n--- SELECTEAZA DEPOUL PENTRU ADAUGARE ---\n";
    for (int i = 0; i < depouri.size(); i++) {
        std::cout << i + 1 << ". " << depouri[i].getNume() << "\n";
    }
    std::cout << "Alege depou: ";
    int optDepou; std::cin >> optDepou;
    if (optDepou < 1 || optDepou > depouri.size()) {
        throw EroareOperatiune("Optiune depou invalida!");
    }
    std::string numeDepou = depouri[optDepou - 1].getNume();
    std::cout << "Tip vehicul (autobuz/tramvai/troleibuz): ";
    std::cin >> tip;
    std::cout << "Numar inmatriculare: ";
    std::cin >> nrInmat;
    std::cout << "Capacitate: ";
    std::cin >> capacitate;
    std::cout << "Kilometraj curent: ";
    std::cin >> km;

    std::transform(tip.begin(), tip.end(), tip.begin(), tolower);

    if (tip == "autobuz")
        std::cout << "Tip motor (diesel/electric): ";
    else if (tip == "tramvai")
        std::cout << "Numar vagoane: ";
    else if (tip == "troleibuz")
        std::cout << "Are baterie (1/true pentru Da, 0/false pentru Nu): ";
    else
        throw EroareOperatiune("Tip vehicul necunoscut!");
    std::cin >> atributSpecific;
    VehiculBuilder asamblare;
    std::shared_ptr<Vehicul> v = asamblare.setTip(tip)
                                          .setNr(nrInmat)
                                          .setCapacitate(capacitate)
                                          .setKm(km)
                                          .setAtributSpecific(atributSpecific)
                                          .build();
    manager.adaugaVehiculNou(numeDepou, v);
}

void Meniu::trateazaTrimitereInService() const {
    std::vector<std::string> masiniFaraRevizie;
    const auto& depouri = manager.getDepouri();
    for (const auto& depou : depouri) {
        for (const auto& v : depou.getFlota()) {
            if (v->necesitaRevizie()) {
                masiniFaraRevizie.push_back(v->getNrInmatriculare());
            }
        }
    }

    if (masiniFaraRevizie.empty()) {
        std::cout << "Niciun vehicul nu are nevoie de revizie in acest moment!\n";
        return;
    }

    std::cout << "\n--- VEHICULE CE NECESITA REVIZIE ---\n";
    for (int i = 0; i < masiniFaraRevizie.size(); i++) {
        std::cout << i + 1 << ". Vehiculul " << masiniFaraRevizie[i] << "\n";
    }

    std::cout << "Alege vehiculul de trimis in service (1-" << masiniFaraRevizie.size() << "): ";
    int alegere; std::cin >> alegere;
    if (alegere < 1 || alegere > masiniFaraRevizie.size()) {
        throw EroareOperatiune("Optiune invalida!");
    }

    manager.trimiteInService(masiniFaraRevizie[alegere - 1]);
}

void Meniu::start() const {
    bool activ = true;
    while (activ) {
        afiseazaOptiuni();
        int optiune;
        if (!(std::cin >> optiune)) {
            std::cin.clear(); std::cin.ignore(10000, '\n');
            std::cout << "Te rog introdu un numar valid!\n";
            continue;
        }

        try {
            switch (optiune) {
            case 1: manager.afis(); break;
            case 2: trateazaAdaugareVehicul(); break;
            case 3: {
                    std::string nr = alegeVehiculDinDepou();
                    manager.stergeVehicul(nr);
                    break;
            }
            case 4: {
                    std::string nr = alegeVehiculDinDepou();
                    std::string linie = alegeLinie();
                    manager.alocaVehiculPeLinie(nr, linie);
                    break;
            }
            case 5: manager.incheieZiuaDeLucru(); break;
            case 6: manager.raportReviziiUrgente(); break;
            case 7: { trateazaTrimitereInService(); break; }
            case 0: std::cout << "La revedere!\n"; activ = false; break;
            default: std::cout << "Optiune invalida!\n";
            }
        } catch (const EroareGenerala& err) {
            std::cerr << err.what() << "\n";
        }
    }
}