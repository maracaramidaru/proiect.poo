
#ifndef VAPOR_H
#define VAPOR_H

#include "Transport.h"
#include <string>
#include <iostream>

class Vapor : public Transport {
    int capacitate;
    double pretCursa;
public:
    Vapor();
    Vapor(const std::string& n, int cap, double pretC);
    double getPretCursa() const override;
    double calculeazaReducere(int nrPersoane, bool copil) const override;
    void print(std::ostream& os) const override;
};
#endif //VAPOR_H
