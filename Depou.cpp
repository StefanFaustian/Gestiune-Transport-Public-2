#include "Autobuz.h"
#include "Exceptii.h"
#include "Depou.h"
#include <memory>
#include <algorithm> // pentru std::find_if()

template <typename T>
Depou<T>::Depou(const std::string& nume_, const int cap) : id(++contorId), capacitateMax(cap), nume(nume_) {
    if (nume_.empty()) {
        throw EroareValidareDepou("Un depou trebuie sa aiba un nume.");
    }
}

template <typename T>
Depou<T>::Depou(const Depou<T>& other) : id(++contorId), capacitateMax(other.capacitateMax), nume(other.nume) {
    for (const auto& vehicul : other.vehicule) {
        std::shared_ptr<T> copieVehicul(dynamic_cast<T*>(vehicul->clone()));
        this->adaugaVehicul(copieVehicul);
    }
}

template <typename T>
Depou<T>::Depou(Depou<T>&& other) noexcept
    : id(other.id),
      capacitateMax(other.capacitateMax),
      nume(std::move(other.nume)),
      vehicule(std::move(other.vehicule)) // se muta vectorul de shared_ptr, operatie de complexitate O(1), cu mult mai rapid decat CC!
{
    other.capacitateMax = 0;
}

template <typename T>
Depou<T>& Depou<T>::operator=(Depou<T> other) {
    swap(*this, other);
    return *this;
}

template <typename T>
Depou<T>::~Depou() = default;

template <typename T>
void Depou<T>::adaugaVehicul(const std::shared_ptr<T> v) {
    if (vehicule.size() >= static_cast<size_t>(capacitateMax)) {
        throw EroareCapacitateDepou(nume);
    }
    vehicule.emplace_back(v);
}

template <typename T>
bool Depou<T>::eliminaVehicul(const std::string& nr) {
    const auto it = std::find_if(vehicule.begin(), vehicule.end(),
        [&nr](const std::shared_ptr<T>& v) {
            return v->getNrInmatriculare() == nr;
        });

    if (it != vehicule.end()) {
        vehicule.erase(it);
        return true;
    }

    return false; // Nu s-a găsit în acest depou
}

template <typename T>
std::shared_ptr<T> Depou<T>::cautaVehicul(const std::string& nrInmatriculare) {
    const auto it = std::find_if(vehicule.begin(), vehicule.end(),
        [&nrInmatriculare](const std::shared_ptr<T>& v) { return v->getNrInmatriculare() == nrInmatriculare;});

    if (it != vehicule.end()) {
        return *it;
    }
    return nullptr; // cazul in care nu a fost gasit vehiculul cautat
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const Depou<T>& d) {
    out << "(ID " << d.id << ") Depoul " << d.nume << " -~- Inventar\n";
    for (const auto& vehicul : d.vehicule) {
        out << *vehicul;
    }
    return out;
}

template <typename T>
void swap(Depou<T>& a, Depou<T>& b) noexcept {
    using std::swap;
    swap(a.capacitateMax, b.capacitateMax);
    swap(a.nume, b.nume);
    swap(a.vehicule, b.vehicule);
}

template <typename T>
int Depou<T>::contorId = 0;

template class Depou<Vehicul>;
template std::ostream& operator<<(std::ostream& out, const Depou<Vehicul>& d);
template void swap(Depou<Vehicul>& a, Depou<Vehicul>& b) noexcept;