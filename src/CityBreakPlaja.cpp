#include "CityBreakPlaja.h"
#include "Vacanta.h"
#include <iostream>
#include <string>


CityBreakPlaja::CityBreakPlaja( std::string destinatie, int durata,  std::string oras,
                                std::string dataIncepere, double pret, int bilete)
    : Vacanta(destinatie, dataIncepere, durata, pret, oras), bileteDisponibile(bilete) {}


CityBreakPlaja::CityBreakPlaja()
    : Vacanta("Destinatie necunoscuta", "01-01-2025", 7, 1000, "Necunoscut"), bileteDisponibile(0) {}

CityBreakPlaja::CityBreakPlaja(const CityBreakPlaja& other)
    : Vacanta(other), bileteDisponibile(other.bileteDisponibile) {}

CityBreakPlaja& CityBreakPlaja::operator=(const CityBreakPlaja& other) {
    if (this != &other) {
        Vacanta::operator=(other);
        bileteDisponibile = other.bileteDisponibile;
    }
    return *this;
}

bool CityBreakPlaja::esteOrasul(std::string orasCautat) const {
    return oras == orasCautat;
}

int CityBreakPlaja::getBileteDisponibile() const {
    return bileteDisponibile;
}

void CityBreakPlaja::print(std::ostream& os) const {
    os << "City Break la Plaja\n";
    os << "Destinatie: " << destinatie << "\n";
    os << "Data Incepere: " << dataIncepere << "\n";
    os << "Durata: " << durataZile << " zile\n";
    os << "Pret: " << pret << " RON\n";
    os << "Bilete disponibile: " << bileteDisponibile << "\n";
    if (cazare) {
        os << "Cazare: " << cazare->getNume()
           << " (" << cazare->getStele() << " stele)\n";
    }
}
