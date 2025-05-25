#include "Transport.h"
#include "TransportManager.h"
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <list>


Transport::Transport(int nrLocuri) :  numarLocuri(nrLocuri) {
    locuri.resize(nrLocuri, false);
}
Transport::Transport(int nrLocuri, const std::string& nume)
    : numarLocuri(nrLocuri), nume(nume) {
    locuri.resize(nrLocuri, false);
}

Transport::~Transport() {}

std::ostream& operator<<(std::ostream& os, const Transport& t) {
    t.print(os);
    return os;
}

const std::string& Transport::getNume() const {
    return nume;
}

void Transport::rezervaLoc(int loc) {
    if (loc < 1 || loc > numarLocuri) {
        std::cout << "Loc invalid! Introdu un loc între 1 și " << numarLocuri << ".\n";
        return;
    }

    int index = 1;
    for (auto& loc_curent : locuri) {
        if (index == loc) {
            if (loc_curent) {
                std::cout << "Locul " << loc << " este deja ocupat!\n";
            } else {
                loc_curent = true;
                rezervariActive.push_back(loc);
                std::cout << "Locul " << loc << " a fost rezervat!\n";
            }
            return;
        }
        index++;
    }
}

void Transport::anuleazaRezervare(int loc) {
    if (loc < 1 || loc > numarLocuri) {
        std::cout << "Loc invalid! Introdu un loc între 1 și " << numarLocuri << ".\n";
        return;
    }

    auto rezervare = std::find(rezervariActive.begin(), rezervariActive.end(), loc);
    if (rezervare == rezervariActive.end()) {
        std::cout << "Locul " << loc << " nu era rezervat de tine!\n";
        return;
    }

    int index = 1;
    for (auto& loc_curent : locuri) {
        if (index == loc) {
            loc_curent = false;
            break;
        }
        index++;
    }

    rezervariActive.erase(rezervare);
    std::cout << "Rezervarea locului " << loc << " a fost anulata cu succes!\n";
}

void Transport::afiseazaDisponibilitate() const {
    std::cout << "\nDisponibilitate locuri:\n";
    int index = 1;
    for (bool ocupat : locuri) {
        bool esteRezervareActiva = std::find(rezervariActive.begin(), rezervariActive.end(), index) != rezervariActive.end();
        std::cout << index << (ocupat ? (esteRezervareActiva ? "[R]" : "[x]") : "[ ]") << " ";
        if (index % 2 == 0) std::cout << "\n";
        index++;
    }
    std::cout << "\n";
}

const std::vector<int>& Transport::getRezervariActive() const {
    return rezervariActive;
}
