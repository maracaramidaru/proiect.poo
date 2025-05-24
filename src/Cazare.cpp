#include "Cazare.h"

Cazare::Cazare(const std::string& nume, TipCazare tip, int stele, const std::string& locatie,
               double pretPerNoapte, int camereDisponibile)
    : nume(nume), tip(tip), stele(stele), locatie(locatie),
      pretPerNoapte(pretPerNoapte), camereDisponibile(camereDisponibile),
      micDejun(false), demipensiune(false), allInclusive(false) {}

double Cazare::calculeazaPret(int nrNopti, int nrPersoane) {
    double pretBaza = pretPerNoapte * nrNopti;
    if (micDejun) pretBaza += 50 * nrNopti * nrPersoane;
    if (demipensiune) pretBaza += 100 * nrNopti * nrPersoane;
    if (allInclusive) pretBaza += 200 * nrNopti * nrPersoane;
    return pretBaza;
}

bool Cazare::verificaDisponibilitate(int nrCamere) const {
    return camereDisponibile >= nrCamere;
}

void Cazare::adaugaFacilitate(const std::string& facilitate) {
    facilitati.push_back(facilitate);
}

const std::vector<std::string>& Cazare::getFacilitati() const {
    return facilitati;
}

const std::string& Cazare::getNume() const {
    return nume;
}

// const std::string& Cazare::getLocatie() const {
//     return locatie;
// }

double Cazare::getPretPerNoapte() const {
    return pretPerNoapte;
}

int Cazare::getStele() const {
    return stele;
}

void Cazare::setMicDejun(bool include) {
    micDejun = include;
}

void Cazare::setDemipensiune(bool include) {
    demipensiune = include;
}

void Cazare::setAllInclusive(bool include) {
    allInclusive = include;
}
