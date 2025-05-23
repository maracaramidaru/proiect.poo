
#ifndef ORGANIZATOR_H
#define ORGANIZATOR_H
#include "Utilizator.h"
#include <iostream>

class Organizator : public Utilizator {
public:
    Organizator(const std::string& u, const std::string& p);
    void afiseazaMeniu() const override;
};


#endif //ORGANIZATOR_H
