#pragma once

#include "Vehicul.h"
#include <vector>
#include <memory>

template <typename T> class Depou;
template <typename T> std::ostream& operator<<(std::ostream& out, const Depou<T>& d);
template <typename T> void swap(Depou<T>& a, Depou<T>& b) noexcept;

template <typename T>
class Depou {
    static int contorId;
    const int id;
    int capacitateMax;
    std::string nume;
    std::vector<std::shared_ptr<T>> vehicule;
public:
    // Constructor
    Depou(const std::string& nume_, int cap);

    // Constructor de copiere
    Depou(const Depou& other);

    // Constructor de mutare
    Depou(Depou&& other) noexcept;

    // Operatorul =
    Depou& operator=(Depou other);

    ~Depou();
    void adaugaVehicul(std::shared_ptr<T> v);
    bool eliminaVehicul(const std::string& nr);
    std::shared_ptr<T> cautaVehicul(const std::string& nrInmatriculare);
    template<typename U>
    friend std::ostream& operator<<(std::ostream& out, const Depou<U>& d);

    template<typename U>
    friend void swap(Depou<U>& a, Depou<U>& b) noexcept;

    // Getters
    const std::string& getNume() const { return nume; }
    const std::vector<std::shared_ptr<T>>& getFlota() const { return vehicule; }
};

