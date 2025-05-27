
#ifndef BILET_H
#define BILET_H

#include "Vacanta.h"

#include <string>

class Bilet : public Vacanta {
private:


public:
    Bilet(const std::string& destinatie,
       const std::string& dataIncepere,
       int durataZile,
       double pret,
       const std::string& oras);





    void print(std::ostream& os) const override;
};

#endif //BILET_H
