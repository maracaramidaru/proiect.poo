#include "Organizator.h"
#include <iostream>
#include <string>

Organizator::Organizator(const std::string& u, const std::string& p)
    : Utilizator(u, p, "organizator") {}

void Organizator::afiseazaMeniu() const {
    std::cout << "\nMeniu Organizator:\n";
    std::cout << "1. Adauga pachet nou\n";
    std::cout << "2. Afiseaza toate pachetele\n";
    std::cout << "3. Iesire\n";
    std::cout << "0. Inapoi la meniul principal\n";

}
