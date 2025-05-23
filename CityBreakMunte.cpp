#include "CityBreakMunte.h"

CityBreakMunte::CityBreakMunte(std::string tara, int durata, std::string oras,
                               std::string dataIncepere, double pret, int bilete)
    : Vacanta(tara, dataIncepere, durata, pret, oras),
      bileteDisponibile(bilete) {}

CityBreakMunte::CityBreakMunte()
    : Vacanta("Destinatie necunoscuta", "01-01-2025", 7, 1000, "Necunoscut"),
      bileteDisponibile(0) {}

CityBreakMunte::CityBreakMunte(const CityBreakMunte& other)
    : Vacanta(other), bileteDisponibile(other.bileteDisponibile) {}

bool CityBreakMunte::esteOrasul(std::string orasCautat) const {
    return oras == orasCautat;
}

int CityBreakMunte::getBileteDisponibile() const {
    return bileteDisponibile;
}

void CityBreakMunte::print(std::ostream& os) const {
    os << "City Break la Munte\n";
    os << "Destinatie: " << destinatie << "\n";
    os << "Data Incepere: " << dataIncepere << "\n";
    os << "Durata: " << durataZile << " zile\n";
    os << "Pret: " << pret << " RON\n";
    os << "Bilete disponibile: " << bileteDisponibile << "\n";
    if (cazare) {
        os << "Cazare: " << cazare->getNume()
           << " (" << cazare->getStele() << " stele)\n"
           << "Pret cazare per noapte: " << cazare->getPretPerNoapte() << " RON\n";
    }
}

bool CityBreakMunte::rezervaBilet() {
    if (getBileteDisponibile() > 0) {
        --bileteDisponibile;
        std::cout << "Bilet rezervat! Bilete ramase: " << bileteDisponibile << "\n";
        return true;
    } else {
        throw BileteEpuizateException();
    }
}

double CityBreakMunte::calculeazaPretCuReducere(double pretInitial, const std::string& codPromo) {
    if (codPromo == "REDUCERE20")
        return getPret() * 0.8;
    else if (codPromo == "REDUCERE25")
        return getPret() * 0.75;
    else
        return pretInitial;
}

std::istream& operator>>(std::istream& in, CityBreakMunte& pachet) {
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
//
// Created by marac on 5/23/2025.
//
