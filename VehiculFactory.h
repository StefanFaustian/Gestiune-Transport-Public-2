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

class VehiculFactory {
public:
    static std::shared_ptr<Vehicul> creeazaVehicul(std::string tipVehicul,
    const std::string& nr, int cap, std::string paramSpecific, int km = 0) {
        std::transform(tipVehicul.begin(),tipVehicul.end(),tipVehicul.begin(),tolower);

        if (tipVehicul == "autobuz") {
            return std::make_shared<Autobuz>(nr,cap,paramSpecific,km);
        } else if (tipVehicul == "tramvai") {
            int nrVagoane = safeStoi(paramSpecific, "numar vagoane tramvai");
            return std::make_shared<Tramvai>(nr, cap, nrVagoane, km);
        } else if (tipVehicul == "troleibuz") {
            std::transform(paramSpecific.begin(),paramSpecific.end(),paramSpecific.begin(),tolower);
            bool baterie = paramSpecific == "true" || paramSpecific == "1";
            return std::make_shared<Troleibuz>(nr, cap, baterie, km);
        }
        throw EroareFisier("Tip de vehicul neidentificat: " + tipVehicul);
    }
};