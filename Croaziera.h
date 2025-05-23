
#ifndef CROAZIERA_H
#define CROAZIERA_H
#pragma once
#include "Vacanta.h"
#include "Exceptii.h"
#include <iostream>
#include <string>

class Croaziera : public Vacanta {
private:
    int bileteDisponibile;

public:
    Croaziera(std::string destinatie, int durata, std::string oras,
              std::string dataPlecare, double pret, int bilete);

    Croaziera();

    Croaziera(const Croaziera& other);

    bool esteOrasul(std::string orasCautat) const override;

    int getBileteDisponibile() const;

    bool rezervaBilet() override;

    double calculeazaPretCuReducere(double pretInitial, const std::string& codPromo);

    void print(std::ostream& os) const override;

    friend std::istream& operator>>(std::istream& in, Croaziera& pachet);
};

#endif //CROAZIERA_H
