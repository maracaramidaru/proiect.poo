#include "AutentificareManager.h"
# include "Enumerari.h"
#include <iostream>
#include <string>
#include "Vizitator.h"
#include "Organizator.h"
AutentificareManager::AutentificareManager() {
    utilizatori.push_back(std::make_shared<Vizitator>("mara", "parola123"));
}

void AutentificareManager::login() {
    std::string userInput;
    std::cout << "Introdu username-ul: ";
    std::getline(std::cin, userInput);

    // Cauta utilizatorul cu username-ul dat
    std::shared_ptr<Utilizator> gasit = nullptr;
    for (const auto& u : utilizatori) {
        if (u->getUsername() == userInput) {
            gasit = u;
            break;
        }
    }

    if (gasit != nullptr) {
        std::string parolaInput;
        bool autentificat = false;
        int incercari = 0;

        while (!autentificat && incercari < 3) {
            std::cout << "Introdu parola pentru " << userInput << ": ";
            std::getline(std::cin, parolaInput);

            if (gasit->autentificare(userInput, parolaInput)) {
                autentificat = true;
                utilizatorCurent = gasit;
                std::cout << "Autentificare reusita! Bine ai venit, " << userInput << ".\n";
            } else {
                std::cout << "Parola gresita. Incearca din nou.\n";
                incercari++;
            }
        }

        if (!autentificat) {
            std::cout << "Prea multe incercari esuate. Iesire.\n";
            utilizatorCurent = nullptr;
        }
    } else {
        std::cout << "Utilizator inexistent. Creeaza un utilizator nou.\n";

        std::shared_ptr<Utilizator> utilizatorNou;
        bool rolValid = false;

        while (!rolValid) {
            std::cout << "Introdu rol (1 pentru vizitator, 2 pentru organizator): ";
            int alegereRol;

            if (std::cin >> alegereRol) {
                switch (alegereRol) {
                    case 1:
                        utilizatorNou = std::make_shared<Vizitator>("", "");
                        rolValid = true;
                        break;
                    case 2:
                        utilizatorNou = std::make_shared<Organizator>("", "");
                        rolValid = true;
                        break;
                    default:
                        std::cout << "Rol invalid. Te rog alege 1 sau 2.\n";
                }
            } else {
                std::cout << "Te rog introdu un numar valid (1 sau 2).\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> *utilizatorNou;

        // Seteaza username-ul corect la noul utilizator
        utilizatorNou->setUsername(userInput);

        // Adauga utilizatorul in lista pentru a putea fi gasit ulterior
        utilizatori.push_back(utilizatorNou);

        utilizatorCurent = utilizatorNou;

        std::cout << "Utilizator inregistrat si autentificat cu succes!\n";
    }
}

std::shared_ptr<Utilizator> AutentificareManager::getUtilizatorCurent() const {
    return utilizatorCurent;
}

bool AutentificareManager::esteAutentificat() const {
    return utilizatorCurent != nullptr;
}

void AutentificareManager::logout() {
    if (utilizatorCurent) {
        std::cout << "Utilizatorul " << utilizatorCurent->getUsername() << " s-a delogat.\n";
    }
    utilizatorCurent.reset();
}

