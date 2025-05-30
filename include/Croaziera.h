
#ifndef CROAZIERA_H
#define CROAZIERA_H
#include "Vacanta.h"
#include <iostream>
#include <string>

class Croaziera : public Vacanta {
private:
    int bileteDisponibile;

public:
    Croaziera(const std::string& destinatie, int durata, const std::string& oras,
              const std::string& dataPlecare, double pret, int bilete);

    Croaziera();

    Croaziera(const Croaziera& other);

    bool esteOrasul(std::string orasCautat) const override;

    int getBileteDisponibile() const override;

    bool rezervaBilet() override;

    void print(std::ostream& os) const override;

    friend std::istream& operator>>(std::istream& in, Croaziera& pachet);
};

#endif //CROAZIERA_H
