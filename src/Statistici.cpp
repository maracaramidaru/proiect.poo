#include "Statistici.h"
#include <iostream>
#include <algorithm>
Statistici::Statistici() : totalRezervari(0), totalVanzari(0.0) {}

Statistici& Statistici::getInstance() {
    static Statistici instance;
    return instance;
}

void Statistici::adaugaRezervare(const std::string& destinatie, double pret) {
    rezervariPerDestinatie[destinatie]++;
    totalRezervari++;
    totalVanzari += pret;
}

void Statistici::afiseazaStatistici() const {
    std::cout << "\n=== Statistici ===\n";
    std::cout << "Total rezervari: " << totalRezervari << "\n";
    std::cout << "Total vanzari: " << totalVanzari << " RON\n";

    std::cout << "\nRezervari per destinatie:\n";

    auto maxRezervari = std::max_element(
        rezervariPerDestinatie.begin(),
        rezervariPerDestinatie.end(),
        [](const auto& p1, const auto& p2) {
            return p1.second < p2.second;
        }
    );

    for (const auto& [destinatie, numar] : rezervariPerDestinatie) {
        std::cout << destinatie << ": " << numar;
        if (!rezervariPerDestinatie.empty() && maxRezervari->first == destinatie) {
            std::cout << " (Cea mai populara destinatie!)";
        }
        std::cout << "\n";
    }
}
