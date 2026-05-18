#pragma once
#include "Vehicul.h"
#include "Autobuz.h"
#include "Tramvai.h"
#include "Troleibuz.h"
#include "Exceptii.h"
#include "Utilitare.h"
#include <memory>
#include <string>
#include <algorithm>



class VehiculBuilder {
    int capacitate = 0, km = 0;
    std::string tipVehicul, nrInmatriculare, atributSpecific;
public:
    VehiculBuilder() = default;
    VehiculBuilder& setTip(const std::string& tip) {
        tipVehicul = tip;
        std::transform(tipVehicul.begin(), tipVehicul.end(), tipVehicul.begin(), tolower);
        return *this;
    }

    VehiculBuilder& setNr(const std::string& nr) {
        nrInmatriculare = nr;
        std::transform(nrInmatriculare.begin(), nrInmatriculare.end(), nrInmatriculare.begin(), toupper);
        return *this;
    }

    VehiculBuilder& setCapacitate(int cap) {
        capacitate = cap;
        return *this;
    }

    VehiculBuilder& setKm(int km_) {
        km = km_;
        return *this;
    }

    VehiculBuilder& setAtributSpecific(const std::string& atribut) {
        atributSpecific = atribut;
        return *this;
    }

    std::shared_ptr<Vehicul> build() {
        if (nrInmatriculare.empty()) throw EroareOperatiune("Numarul de inmatriculare nu poate fi gol.");
        if (capacitate <= 0) throw EroareOperatiune("Capacitatea trebuie sa fie pozitiva");

        if (tipVehicul == "autobuz") return std::make_shared<Autobuz>(nrInmatriculare,capacitate,atributSpecific,km);
        else if (tipVehicul == "tramvai") {
            int nrVagoane = safeStoi(atributSpecific, "numar vagoane tramvai");
            return std::make_shared<Tramvai>(nrInmatriculare,capacitate,nrVagoane,km);
        } else if (tipVehicul == "troleibuz") {
            bool baterie = (atributSpecific == "1" || atributSpecific == "true");
            return std::make_shared<Troleibuz>(nrInmatriculare, capacitate, baterie, km);
        }
        throw EroareOperatiune("Tip de vehicul neidentificat: " + tipVehicul);
    }
};
