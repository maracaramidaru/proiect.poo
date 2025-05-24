
#ifndef CITYBREAKPLAJA_H
#define CITYBREAKPLAJA_H
#include "Vacanta.h"
#include <iostream>
#include <string>
class CityBreakPlaja : public Vacanta {
private:
    int bileteDisponibile;

public:

    CityBreakPlaja( std::string tara, int durata, std::string oras,
                    std::string dataIncepere, double pret, int bilete);


    CityBreakPlaja();
    CityBreakPlaja(const CityBreakPlaja& other);
    CityBreakPlaja& operator=(const CityBreakPlaja& other);

    bool esteOrasul(std::string orasCautat) const override;
    int getBileteDisponibile() const override;

    bool rezervaBilet();

    void print(std::ostream& os) const override;
    friend std::istream& operator>>(std::istream& in, CityBreakPlaja& pachet);
};

#endif //CITYBREAKPLAJA_H
