
#ifndef PACHETFACTORY_H
#define PACHETFACTORY_H

#include <memory>
#include <string>
#include "Vacanta.h"
#include "Enumerari.h"
class PachetFactory {
public:
    static std::shared_ptr<Vacanta> creeazaPachet(
        TipVacanta tip,
        const std::string& destinatie,
        const std::string& dataIncepere,
        int durataZile,
        double pret,
        const std::string& oras,
        int bilete = 0);
};

#endif
