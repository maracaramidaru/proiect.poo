
#ifndef GESTIUNECAZARI_H
#define GESTIUNECAZARI_H

#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "Cazare.h" // presupunem că ai clasa Cazare definită undeva

class GestiuneCazari {
private:
    std::map<std::string, std::vector<std::shared_ptr<Cazare>>> cazariPerDestinatie;

public:
    void adaugaCazare(const std::string& destinatie, const std::shared_ptr<Cazare>& cazare);

    std::vector<std::shared_ptr<Cazare>> getCazariDisponibile(
        const std::string& destinatie,
        int nrPersoane,
        double bugetMaxim
    );
    void afiseazaCazariPentruDestinatie(const std::string& destinatie, const std::vector<std::shared_ptr<Cazare>>& cazari) ;

};

#endif // GESTIUNECAZARI_H
