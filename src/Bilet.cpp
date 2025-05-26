#include "Bilet.h"
#include <iostream>

Bilet::Bilet(const std::string& destinatie,
             const std::string& dataIncepere,
             int durataZile,
             double pret,
             const std::string& oras)

    : Vacanta(destinatie, dataIncepere, durataZile, pret, oras) {
    std::cout << "Bilet cumparat pentru: " << destinatie << ", oras: " << oras << "\n";
}

void Bilet::print(std::ostream& os) const {
    os << "Bilet pentru: " << destinatie << " in " << oras
       << ", data: " << dataIncepere << ", durata: " << durataZile
       << " zile, pret: " << pret << " EUR\n";
}
