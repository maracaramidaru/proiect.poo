
#ifndef VILA_H
#define VILA_H

#include <string>
#include <stdexcept>
#include "Cazare.h"  // Include clasa de bază

class Vila : public Cazare {
private:
    //bool areBucatarie;
    //bool areGradina;
    bool areBarbeque;
    int capacitateMaxima;

public:
    Vila(const std::string& nume,int stele, const std::string& locatie,
         double pretPerNoapte, int camereDisponibile, int capacitateMaxima);

    void setBarbeque(bool are);
    int getCapacitateMaxima() const;

    double calculeazaPret(int nrNopti, int nrPersoane) override;
};

#endif // VILA_H
