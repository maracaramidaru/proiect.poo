
#ifndef HOTEL_H
#define HOTEL_H

#include <string>
#include "Cazare.h"

class Hotel : public Cazare {
private:
    bool arePiscina;
    bool areSpa;

public:
    Hotel(const std::string& nume, int stele, const std::string& locatie,
          double pretPerNoapte, int camereDisponibile);

    void setPiscina(bool are);
    void setSpa(bool are);

    double calculeazaPret(int nrNopti, int nrPersoane) override;
};

#endif // HOTEL_H

