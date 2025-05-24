#include "AgentieVacante.h"
#include "Enumerari.h"
#include "Vacanta.h"
#include "CityBreakPlaja.h"
#include "CityBreakMunte.h"
#include "Croaziera.h"

GestiuneCazari& AgentieVacante::getGestiuneCazari() {
    return gestiuneCazari;
}

const std::vector<std::shared_ptr<Vacanta>>& AgentieVacante::getPachete() const {
    return pachete;
}

void AgentieVacante::citestePachet() {
    std::string tip;
    std::cout << "Introdu tipul pachetului (Plaja/Munte/Croaziera): ";
    std::cin >> tip;

    if (tip == "Plaja") {
        CityBreakPlaja pachet;
        std::cin >> pachet;
        pachete.push_back(std::make_shared<CityBreakPlaja>(pachet));
    } else if (tip == "Munte") {
        CityBreakMunte pachet;
        std::cin >> pachet;
        pachete.push_back(std::make_shared<CityBreakMunte>(pachet));
    } else if (tip == "Croaziera") {
        Croaziera pachet;
        std::cin >> pachet;
        pachete.push_back(std::make_shared<Croaziera>(pachet));
    } else {
        std::cout << "Tip invalid!\n";
    }
}

void AgentieVacante::cautaDupaOras(const std::string& orasCautat) const {
    std::cout << "\nCautare dupa oras: " << orasCautat << "\n";
    bool gasit = false;
    for (const auto& pachet : pachete) {
        if (pachet->esteOrasul(orasCautat)) {
            std::cout << *pachet;
            gasit = true;
        }
    }
    if (!gasit) {
        std::cout << "Nu exista pachete pentru orasul " << orasCautat << ".\n";
    }
}

void AgentieVacante::adaugaPachet(TipVacanta tipPachet, const std::string &destinatie, const std::string &dataIncepere, int durataZile, double pret, const std::string &oras, int bilete) {
    std::shared_ptr<Vacanta> pachet = PachetFactory::creeazaPachet(tipPachet, destinatie, dataIncepere, durataZile, pret, oras, bilete);

    if (pachet) {
        pachete.push_back(pachet);
    } else {
        std::cout << "Tipul de pachet nu este valid!" << std::endl;
    }
}

void AgentieVacante::sorteazaVacanteDupaPret() {
    std::sort(pachete.begin(), pachete.end(), [](const std::shared_ptr<Vacanta>& a, const std::shared_ptr<Vacanta>& b) {
        return *a < *b;
    });

    std::cout << "Vacantele au fost sortate dupa pret (crescator):\n";
    for (const auto& vacanta : pachete) {
        vacanta->print(std::cout);
        std::cout << "------------------\n";
    }
}

void AgentieVacante::afiseazaPacheteDupaTip(const std::string& tip) const {
    bool gasit = false;
    for (const auto& pachet : pachete) {
        if (tip == "Munte" && std::dynamic_pointer_cast<CityBreakMunte>(pachet)) {
            std::cout << *pachet;
            gasit = true;
        }
        else if (tip == "Plaja" && std::dynamic_pointer_cast<CityBreakPlaja>(pachet)) {
            std::cout << *pachet;
            gasit = true;
        }
        else if (tip == "Croaziera" && std::dynamic_pointer_cast<Croaziera>(pachet)) {
            std::cout << *pachet;
            gasit = true;
        }
    }
    if (!gasit)
        std::cout << "Nu exista pachete pentru tipul selectat.\n";
}

void AgentieVacante::listeazaPachete() {
    if (pachete.empty()) {
        std::cout << "Nu exista pachete de vacanta disponibile.\n";
        return;
    }

    std::cout << "\n--- Lista de pachete ---\n";
    for (size_t i = 0; i < pachete.size(); ++i) {
        std::cout << i + 1 << ". ";
        if (std::dynamic_pointer_cast<CityBreakMunte>(pachete[i]))
            std::cout << "[MUNTE] ";
        else if (std::dynamic_pointer_cast<CityBreakPlaja>(pachete[i]))
            std::cout << "[PLAJA] ";
        else if (std::dynamic_pointer_cast<Croaziera>(pachete[i]))
            std::cout << "[CROAZIERA] ";

        std::cout << *pachete[i];
    }
    AutentificareManager manager;
    auto utilizatorCurent = manager.getUtilizatorCurent();

    if (utilizatorCurent && utilizatorCurent->getRol() == "organizator") {
        char optiune;
        std::cout << "\nDoresti sa copiezi un pachet existent? (y/n): ";
        std::cin >> optiune;

        if (optiune == 'y' || optiune == 'Y') {
            size_t index;
            std::cout << "Selecteaza numarul pachetului pe care doresti sa-l copiezi: ";
            std::cin >> index;

            if (index < 1 || index > pachete.size()) {
                std::cout << "Index invalid.\n";
                return;
            }

            std::shared_ptr<Vacanta> pachetSelectat = pachete[index - 1];
            std::shared_ptr<Vacanta> pachetNou;

            if (auto plaja = std::dynamic_pointer_cast<CityBreakPlaja>(pachetSelectat)) {
                CityBreakPlaja copie = *plaja;
                pachetNou = std::make_shared<CityBreakPlaja>(copie);
            } else if (auto munte = std::dynamic_pointer_cast<CityBreakMunte>(pachetSelectat)) {
                CityBreakMunte copie = *munte;
                pachetNou = std::make_shared<CityBreakMunte>(copie);
            } else if (auto croaziera = std::dynamic_pointer_cast<Croaziera>(pachetSelectat)) {
                Croaziera copie = *croaziera;
                pachetNou = std::make_shared<Croaziera>(copie);
            }

            if (pachetNou) {
                std::string dataNoua;
                std::cout << "Introdu noua data de incepere (format: DD-MM-YYYY): ";
                std::cin >> dataNoua;
                pachetNou->setDataIncepere(dataNoua);

                pachete.push_back(pachetNou);
                std::cout << "Pachetul a fost copiat si adaugat cu succes.\n";
            }
        }
    }
}
