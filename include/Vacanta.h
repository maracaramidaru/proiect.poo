#ifndef VACANTA_H
#define VACANTA_H

#include <iostream>
#include <memory>
#include <string>
#include "Cazare.h"
#include "ValidatorData.h"

class Vacanta {
protected:
     std::string destinatie;
     std::string dataIncepere;
    int durataZile;
    double pret;
     std::string oras;
    std::shared_ptr<Cazare> cazare;

public:
    Vacanta(std::string destinatie, std::string dataIncepere, int durataZile, double pret, std::string oras);
    Vacanta(double p);
    virtual ~Vacanta();

    virtual void print(std::ostream& os) const = 0;
    virtual bool esteOrasul(std::string orasCautat) const = 0;
    virtual bool rezervaBilet() = 0;
    virtual int getBileteDisponibile() const = 0;

    friend std::ostream& operator<<(std::ostream& os, const Vacanta& v);

    double getPret() const;
    int getDurataZile() const;
    std::string getDestinatie() const;

    bool operator<(const Vacanta& other) const;

    void setDataIncepere(std::string dataNoua);

    Vacanta(const Vacanta& other);
    Vacanta& operator=(const Vacanta& other);

    void setCazare(std::shared_ptr<Cazare> cazareNoua);
   // std::shared_ptr<Cazare> getCazare() const;
};

#endif // VACANTA_H
