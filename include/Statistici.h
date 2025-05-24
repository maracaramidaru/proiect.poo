

#ifndef STATISTICI_H
#define STATISTICI_H
#include <iostream>
#include <map>
#include <string>
#include <algorithm>

class Statistici {
private:
    std::map<std::string, int> rezervariPerDestinatie;
    int totalRezervari;
    double totalVanzari;

    Statistici();

public:
    static Statistici& getInstance();

    void adaugaRezervare(const std::string& destinatie, double pret);
    void afiseazaStatistici() const;
    void afiseazaNrBileteVandute() const;
    Statistici(const Statistici&) = delete;
    Statistici& operator=(const Statistici&) = delete;
};
#endif