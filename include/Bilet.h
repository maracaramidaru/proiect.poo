
#ifndef BILET_H
#define BILET_H

#include "Vacanta.h"

#include <string>

class Bilet : public Vacanta {
private:
    //static int nrAchizitii;

public:
    Bilet(std::string destinatie, std::string dataIncepere, int durataZile,
          double pret, std::string oras);


    //static void afiseazaNrAchizitii();

    void print(std::ostream& os) const override;
};

#endif //BILET_H
