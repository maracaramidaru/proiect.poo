
#ifndef AUTOBUZ_H
#define AUTOBUZ_H
#include <string>
#include <ostream>
#include "Transport.h"

class Autobuz : public Transport {
     std::string nume;
    double pretCursa;

public:
    Autobuz(int nrLocuri, const std::string& n, double pretC);
    double getPretCursa() const override;
    double calculeazaReducere(int nrPersoane, bool copil) const override;
    void print(std::ostream& os) const override;
};
#endif //AUTOBUZ_H
