#include "Croaziera.h"
#include <iostream>
#include <string>
#include "Vacanta.h"
Croaziera::Croaziera(std::string destinatie, int durata, std::string oras,
                     std::string dataPlecare, double pret, int bilete)
    : Vacanta(destinatie, dataPlecare, durata, pret, oras),
      bileteDisponibile(bilete) {}

Croaziera::Croaziera()
    : Vacanta("Destinatie necunoscuta", "01-01-2025", 7, 1000, "Necunoscut"),
      bileteDisponibile(0) {}

Croaziera::Croaziera(const Croaziera& other)
    : Vacanta(other), bileteDisponibile(other.bileteDisponibile) {}

bool Croaziera::esteOrasul(std::string orasCautat) const {
    return oras == orasCautat;
}

int Croaziera::getBileteDisponibile() const {
    return bileteDisponibile;
}

bool Croaziera::rezervaBilet() {
    if (getBileteDisponibile() > 0) {
        --bileteDisponibile;
        std::cout << "Bilet rezervat! Bilete ramase: " << bileteDisponibile << "\n";
        return true;
    } else {
        throw BileteEpuizateException();
    }
}
//
// double Croaziera::calculeazaPretCuReducere(double pretInitial, const std::string& codPromo) {
//     if (codPromo == "CROAZIERA15")
//         return getPret() * 0.85;
//     else if (codPromo == "CROAZIERA25")
//         return getPret() * 0.75;
//     else
//         return pretInitial;
// }

void Croaziera::print(std::ostream& os) const {
    os << "Croaziera:\n";
    os << "Destiatia: " << destinatie << "\n";
    os << "Data plecare: " << dataIncepere << "\n";
    os << "Durata: " << durataZile << " zile\n";
    os << "Oras imbarcare: " << oras << "\n";
    os << "Pret: " << pret << " RON\n";
    os << "Bilete disponibile: " << bileteDisponibile << "\n";
    if (cazare) {
        os << "Cazare: " << cazare->getNume()
           << " (" << cazare->getStele() << " stele)"
           << "\nPret cazare per noapte: " << cazare->getPretPerNoapte() << " RON\n";
    }
}

std::istream& operator>>(std::istream& in, Croaziera& pachet) {
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

    std::cout << "Introdu orasul de imbarcare: ";
    if (!std::getline(in, pachet.oras)) {
        in.clear();
        return in;
    }

    std::cout << "Introdu data de plecare: ";
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
