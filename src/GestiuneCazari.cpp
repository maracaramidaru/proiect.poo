#include "GestiuneCazari.h"

void GestiuneCazari::adaugaCazare(const std::string& destinatie, const std::shared_ptr<Cazare>& cazare) {
    cazariPerDestinatie[destinatie].push_back(cazare);
}

std::vector<std::shared_ptr<Cazare>> GestiuneCazari::getCazariDisponibile(
    const std::string& destinatie,
    int nrPersoane,
    double bugetMaxim
) {
    std::vector<std::shared_ptr<Cazare>> rezultate;

    auto it = cazariPerDestinatie.find(destinatie);
    if (it == cazariPerDestinatie.end()) {
        return rezultate;
    }

    for (const auto& cazare : it->second) {
        if (cazare->verificaDisponibilitate(1) &&
            cazare->getPretPerNoapte() * nrPersoane <= bugetMaxim) {
            rezultate.push_back(cazare);
            }
    }

    return rezultate;
}
void GestiuneCazari::afiseazaCazariPentruDestinatie(const std::string& destinatie, const std::vector<std::shared_ptr<Cazare>>& cazari) {
    if (cazari.empty()) {
        std::cout << "Nu exista cazari disponibile pentru destinatia " << destinatie << " in bugetul specificat.\n";
        return;
    }

    std::cout << "\nCazari disponibile in " << destinatie << ":\n";
    for (const auto& cazare : cazari) {
        std::cout << "\nNume: " << cazare->getNume()
                  << "\nStele: " << cazare->getStele()
                  << "\nPret per noapte: " << cazare->getPretPerNoapte() << " RON"
                  << "\nFacilitati: ";

        const auto& facilitati = cazare->getFacilitati();
        if (facilitati.empty()) {
            std::cout << "Nu sunt specificate";
        } else {
            for (const auto& facilitate : facilitati) {
                std::cout << facilitate << ", ";
            }
        }
        std::cout << "\n------------------\n";
    }
}

//
// Created by marac on 5/23/2025.
//
