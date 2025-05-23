
#ifndef VIZITATOR_H
#define VIZITATOR_H
#include "Utilizator.h"
#include <iostream>
#include <string>
class Vizitator : public Utilizator {
public:
    Vizitator(const std::string& u, const std::string& p);
    void afiseazaMeniu() const override;
};

#endif //VIZITATOR_H
