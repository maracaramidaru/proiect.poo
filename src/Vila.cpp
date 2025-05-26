#include "Vila.h"
#include <iostream>
#include <string>
Vila::Vila(const std::string& nume, int stele, const std::string& locatie,
           double pretPerNoapte, int camereDisponibile, int capacitateMaxima)
    : Cazare(nume, TipCazare::vila, stele, locatie, pretPerNoapte, camereDisponibile),
      areBarbeque(false),
      capacitateMaxima(capacitateMaxima) {}
void Vila::setBarbeque(bool are) {
    areBarbeque = are;
}

int Vila::getCapacitateMaxima() const {
    return capacitateMaxima;
}

double Vila::calculeazaPret(int nrNopti, int nrPersoane) {
    if (nrPersoane > capacitateMaxima) {
        throw std::runtime_error("Numarul de persoane depaseste capacitatea vilei!");
    }
    return Cazare::calculeazaPret(nrNopti, nrPersoane);
}
