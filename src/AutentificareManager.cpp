#include "AutentificareManager.h"
# include "Enumerari.h"
#include <iostream>
#include <string>
#include "Vizitator.h"
#include "Organizator.h"
void AutentificareManager::login() {
    std::string userInput;
    std::cout << "Introdu username-ul: ";
    std::getline(std::cin, userInput);

    if (userInput == "mara") {
        std::string parolaCorecta = "parola123";
        std::string incercareParola;
        do {
            std::cout << "Introdu parola pentru mara: ";
            std::getline(std::cin, incercareParola);
            if (incercareParola != parolaCorecta) {
                std::cout << "Parola gresita. Incearca din nou.\n";
            }
        } while (incercareParola != parolaCorecta);

        utilizatorCurent = std::make_shared<Vizitator>("mara", parolaCorecta);

        std::cout << "Autentificare reusita! Bine ai venit, mara.\n";
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

