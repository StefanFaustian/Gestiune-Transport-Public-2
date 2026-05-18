#pragma once
#include "Manager.h"
#include <string>

class Meniu {
    Manager& manager;
    void afiseazaOptiuni() const;
    std::string alegeVehiculDinDepou() const;
    std::string alegeLinie() const;
    void trateazaAdaugareVehicul() const;
    void trateazaTrimitereInService() const;

public:
    explicit Meniu(Manager& m);
    void start() const;
};