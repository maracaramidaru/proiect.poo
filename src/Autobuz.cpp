#include "Autobuz.h"

Autobuz::Autobuz(int nrLocuri, const std::string& n, double pretC)
    : Transport(nrLocuri, n), pretCursa(pretC) {}

double Autobuz::getPretCursa() const {
    return pretCursa;
}

double Autobuz::calculeazaReducere(int nrPersoane, bool copil) const {
    double reducere = 0.0;
    if (copil) reducere += 0.25;
    if (nrPersoane >= 20) reducere += 0.10;
    return pretCursa * (1.0 - reducere);
}

void Autobuz::print(std::ostream& os) const {
    os << "Autobuz: " << nume << "|Pret cursa:" << pretCursa << "\n";
}