#include "Vapor.h"
#include <iostream>
#include <string>
Vapor::Vapor() : Transport(20), nume("Necunoscut"), capacitate(500) {}
Vapor::Vapor(const std::string& n, int cap, double pretC)
    : Transport(cap), nume(n), capacitate(cap), pretCursa(pretC) {}

double Vapor::getPretCursa() const {
    return pretCursa;
}

double Vapor::calculeazaReducere(int nrPersoane, bool copil) const {
    double reducere = 0.0;
    if (copil) reducere += 0.25;
    if (nrPersoane >= 20) reducere += 0.10;
    return pretCursa * (1.0 - reducere);
}

void Vapor::print(std::ostream& os) const {
    os << "Vapor: " << nume << " | Capacitate: " << capacitate << " persoane | Pret cursa: " << pretCursa << " RON\n";
}
