#include "Vacanta.h"
#include <iostream>
#include <string>
#include <numeric>
#include "ValidatorData.h"

Vacanta::Vacanta(std::string destinatie, std::string dataIncepere, int durataZile, double pret, std::string oras)
    : destinatie(destinatie),
      dataIncepere(dataIncepere),
      durataZile(durataZile),
      pret(pret),
      oras(oras),
      cazare(nullptr) {
}
Vacanta::Vacanta(double p) : durataZile(0), pret(p) {}


Vacanta::~Vacanta() {}

std::ostream& operator<<(std::ostream& os, const Vacanta& v) {
    v.print(os);
    return os;
}

double Vacanta::getPret() const {
    return pret;
}

int Vacanta::getDurataZile() const {
    return durataZile;
}

const std::string& Vacanta::getDestinatie() const {
    return destinatie;
}

bool Vacanta::operator<(const Vacanta& other) const {
    return this->getPret() < other.getPret();
}
void Vacanta::setDataIncepere(const std::string& dataNoua) {
    ValidatorData::valideazaData(dataNoua);
    dataIncepere = dataNoua;
}


Vacanta::Vacanta(const Vacanta& other)
    : destinatie(other.destinatie),
      dataIncepere(other.dataIncepere),
      durataZile(other.durataZile),
      pret(other.pret),
      oras(other.oras) {
}

Vacanta& Vacanta::operator=(const Vacanta& other) {
    if (this != &other) {
        destinatie = other.destinatie;
        dataIncepere = other.dataIncepere;
        durataZile = other.durataZile;
        pret = other.pret;
        oras = other.oras;
    }
    return *this;
}

void Vacanta::setCazare(std::shared_ptr<Cazare> cazareNoua) {
    cazare = cazareNoua;
    if (cazare) {
        pret += cazare->calculeazaPret(durataZile, 1);
    }
}
//
// std::shared_ptr<Cazare> Vacanta::getCazare() const {
//     return cazare;
// }
