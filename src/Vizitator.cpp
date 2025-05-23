#include "Vizitator.h"
#include <iostream>
#include <string>
Vizitator::Vizitator(const std::string& u, const std::string& p)
    : Utilizator(u, p, "vizitator") {}

void Vizitator::afiseazaMeniu() const {
    std::cout << "\nMeniu Vizitator:\n";
    std::cout << "1. Afiseaza toate pachetele\n";
    std::cout << "2. Cauta pachete dupa oras\n";
    std::cout << "3. Afiseaza pachete Munte\n";
    std::cout << "4. Afiseaza pachete Plaja\n";
    std::cout << "5. Afiseaza Croaziere\n";
    std::cout << "6. Rezerva vacanta\n";
    std::cout << "7. Afiseaza biletele mele\n";
    std::cout << "8. Afiseaza cele mai ieftine pachete de vacanta\n";
    std::cout << "9. Anuleaza loc\n";
    std::cout << "0. Delogare\n";
}
