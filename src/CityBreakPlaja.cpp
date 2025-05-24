#include "CityBreakPlaja.h"
#include "Vacanta.h"
#include <iostream>
#include <string>
#include "Bilet.h"
#include "Exceptii.h"
CityBreakPlaja::CityBreakPlaja(const std::string& destinatie, int durata,
                               const std::string& oras,
                               const std::string& dataIncepere,
                               double pret, int bilete)

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


bool CityBreakPlaja::rezervaBilet() {
    if (getBileteDisponibile() > 0) {
        --bileteDisponibile;
        std::cout << "Bilet rezervat! Bilete ramase: " << bileteDisponibile << "\n";
        return true;
    } else {
        throw BileteEpuizateException();
    }
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

std::istream& operator>>(std::istream& in, CityBreakPlaja& pachet) {
    std::cout << "Introdu destinatia: ";
    in.ignore();
    if (!std::getline(in, pachet.destinatie)) {
        in.clear();
        return in;
    }

    std::cout << "Introdu durata (zile): ";
    if (!(in >> pachet.durataZile)) {
        in.clear();
        return in;
    }
    in.ignore();

    std::cout << "Introdu orasul: ";
    if (!std::getline(in, pachet.oras)) {
        in.clear();
        return in;
    }

    std::cout << "Introdu data de incepere: ";
    if (!std::getline(in, pachet.dataIncepere)) {
        in.clear();
        return in;
    }

    std::cout << "Introdu pretul: ";
    if (!(in >> pachet.pret)) {
        in.clear();
        return in;
    }
    in.ignore();

    std::cout << "Introdu numarul de bilete disponibile: ";
    if (!(in >> pachet.bileteDisponibile)) {
        in.clear();
        return in;
    }
    in.ignore();

    return in;
}
