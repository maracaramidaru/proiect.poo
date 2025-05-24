
#ifndef AVION_H
#define AVION_H

#include "Transport.h"
#include <string>
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <algorithm>
#include "Vizitator.h"

class Avion : public Transport {
    //std::string nume;
    int pozitie = 0;
    double pretCursa;
public:
    Avion(const int nrLocuri, const std::string& n, double pretC);
    void deplaseaza();
    void afiseaza(int lungime) const;
    int getPozitie() const;
    double getPretCursa() const override;
    double calculeazaReducere(int nrPersoane, bool copil) const override;
    void print(std::ostream& os) const override;
    void ruleazaJocAvioane();
    const std::string& getNume() const ;

};

#endif //AVION_H
