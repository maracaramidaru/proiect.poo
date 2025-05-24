
#ifndef TRANSPORT_H
#define TRANSPORT_H
#include <iostream>
#include <string>
#include <list>
#include <vector>

class Transport {
protected:
    std::list<bool> locuri;
    int numarLocuri;
    std::string nume;
    std::vector<int> rezervariActive;

public:

    Transport() : numarLocuri(0) {}
    Transport(int nrLocuri);

    virtual void print(std::ostream& os) const = 0;
    virtual double getPretCursa() const = 0;
    virtual double calculeazaReducere(int nrPersoane, bool copil) const = 0;

    friend std::ostream& operator<<(std::ostream& os, const Transport& t);

    virtual ~Transport();

    const std::string& getNume() const;

    void rezervaLoc(int loc);
    void anuleazaRezervare(int loc);
    void afiseazaDisponibilitate() const;

    const std::vector<int>& getRezervariActive() const;
};

#endif
