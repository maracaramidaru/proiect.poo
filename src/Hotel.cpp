#include "Hotel.h"

Hotel::Hotel(const std::string& nume, int stele, const std::string& locatie,
             double pretPerNoapte, int camereDisponibile)
    : Cazare(nume, TipCazare::hotel, stele, locatie, pretPerNoapte, camereDisponibile),
      arePiscina(false), areSpa(false), areRestaurant(true) {}

void Hotel::setPiscina(bool are) {
    arePiscina = are;
}

void Hotel::setSpa(bool are) {
    areSpa = are;
}

void Hotel::setRestaurant(bool are) {
    areRestaurant = are;
}

double Hotel::calculeazaPret(int nrNopti, int nrPersoane) {
    double pretTotal = Cazare::calculeazaPret(nrNopti, nrPersoane);
    if (areSpa) pretTotal += 100 * nrNopti;
    if (arePiscina) pretTotal += 50 * nrNopti;
    return pretTotal;
}
//
// Created by marac on 5/23/2025.
//
