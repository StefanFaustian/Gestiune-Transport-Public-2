#include "Manager.h"
#include <iostream>
#include "Meniu.h"
#include "Exceptii.h"


int main() {

    Manager& SA = Manager::getManager();
    try {
        SA.incarcaDepouri("Depouri.txt");
        SA.incarcaLinii("Linii.txt");
    } catch (const EroareGenerala& err) {
        std::cerr << err.what() << '\n';
        return 1;
    }
    Meniu meniu(SA);
    meniu.start();
    return 0;
}
