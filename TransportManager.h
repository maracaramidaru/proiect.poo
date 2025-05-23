//
// Created by marac on 5/22/2025.
//

#ifndef TRANSPORTMANAGER_H
#define TRANSPORTMANAGER_H

#include <iostream>
#include <vector>
#include <memory>

template <typename T>
class TransportManager {
private:
    std::vector<std::shared_ptr<T>> transporturi;

public:
    void adaugaTransport(const std::shared_ptr<T>& transport) {
        transporturi.push_back(transport);
    }

    void afiseazaTransporturi() const {
        if (transporturi.empty()) {
            std::cout << "Nu exista mijloace de transport disponibile.\n";
            return;
        }

        std::cout << "\nMijloace de transport disponibile:\n";
        for (size_t i = 0; i < transporturi.size(); ++i) {
            std::cout << i + 1 << ". ";
            transporturi[i]->print(std::cout);
        }
    }

    std::shared_ptr<T> selecteazaTransport(int index) {
        if (index < 1 || index > (int)transporturi.size()) {
            std::cout << "Index invalid! Selectie anulata.\n";
            return nullptr;
        }
        return transporturi[index - 1];
    }
};


#endif
