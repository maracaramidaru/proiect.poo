
#ifndef UTILIZATOR_H
#define UTILIZATOR_H
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cstdlib>
#include "Vacanta.h"

class Utilizator {
private:
    std::string rol;
    std::string username;
    std::string parola;
    std::vector<std::shared_ptr<Vacanta>> bileteRezervate;
    int puncte = 0;
    std::vector<std::string> cadouri = {
        "Voucher 100 RON",
        "Voucher 50 RON",
        "Cod de participare la tombola",
        "Pachet ghid turistic inclus in sejur",
        "Excursie bonus"
    };

public:
    Utilizator();
    Utilizator(const std::string& u, const std::string& p, const std::string& r = "vizitator");

    virtual void afiseazaMeniu() const = 0;
    virtual ~Utilizator();

    void scadePuncte(int nrPuncte);
    void adaugaPuncte(int nrPuncte);
    void acordaCadou();

    friend std::istream& operator>>(std::istream& in, Utilizator& utilizator);

    std::string getRol() const;
    std::string getUsername() const;

    bool autentificare(const std::string& user, const std::string& pass) const;

    void adaugaBilet(const std::shared_ptr<Vacanta>& bilet);

    std::string getParola() const;
    void setParola(const std::string& newParola);

    void afiseazaBilete() const;
};
#endif