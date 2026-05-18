#pragma once

#include "Depou.h"
#include "Linie.h"
#include <string>
#include <unordered_set>
#include <vector>

class Manager {
    std::string numeManager;
    std::unordered_set<std::string> numereInmatriculate;  // verificare rapida a unicitatii vehiculelor
    std::vector<Depou<Vehicul>> depouri;
    std::vector<Linie> linii;
    void incarcaFlota(Depou<Vehicul>& depou, const std::string& numeFisier);
public:
    void afis() const;
    explicit Manager(const std::string& nume);
    std::shared_ptr<Vehicul> gasesteVehiculGlobal(const std::string& nrInmatriculare);
    void raportReviziiUrgente();
    void trimiteInService(const std::string& nrInmatriculare);
    void adaugaVehiculNou(const std::string& numeDepou, const std::shared_ptr<Vehicul>& vehiculNou);
    void stergeVehicul(const std::string& nrInmatriculare);
    void incarcaDepouri(const std::string& numeFisier);
    void incarcaLinii(const std::string& numeFisier);
    void alocaVehiculPeLinie(const std::string& nrInmatriculare, const std::string& indicativLinie);
    void incheieZiuaDeLucru();

    // Getters
    const std::vector<Depou<Vehicul>>& getDepouri() const { return depouri; }
    const std::vector<Linie>& getLinii() const { return linii; }

    ~Manager() = default;
};