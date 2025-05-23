#include "Vila.h"

Vila::Vila(const std::string& nume, const std::string& locatie,
           double pretPerNoapte, int camereDisponibile, int capacitateMaxima)
    : Cazare(nume, TipCazare::vila, 0, locatie, pretPerNoapte, camereDisponibile),
      areBucatarie(true), areGradina(true), areBarbeque(false),
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
//
// Created by marac on 5/23/2025.
//
