
#ifndef CITYBREAKMUNTE_H
#define CITYBREAKMUNTE_H

#include "Vacanta.h"
#include "Exceptii.h"
#include <iostream>
#include <string>
#include <limits>
class CityBreakMunte : public Vacanta {
private:
    int bileteDisponibile;

public:
    CityBreakMunte(const std::string& tara, int durata,
               const std::string& oras,
               const std::string& dataIncepere,
               double pret, int bilete);

    CityBreakMunte();

    CityBreakMunte(const CityBreakMunte& other);

    bool esteOrasul(std::string orasCautat) const override;

    int getBileteDisponibile() const override;

    void print(std::ostream& os) const override;

    bool rezervaBilet() override;



    friend std::istream& operator>>(std::istream& in, CityBreakMunte& pachet);
};

#endif //CITYBREAKMUNTE_H
