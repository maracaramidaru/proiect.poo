#include "Meniu.h"
#include "Transport.h"
#include "Autobuz.h"
#include "Avion.h"
#include "Vapor.h"
#include "TransportManager.h"
#include <iostream>
#include <string>
#include "Hotel.h"
#include "Statistici.h"
#include "Vila.h"
#include <vector>
#include "Cazare.h"
#include "AgentieVacante.h"
#include "Utilizator.h"
#include "Vacanta.h"
#include "Croaziera.h"
#include "CityBreakMunte.h"
#include "CityBreakPlaja.h"
#include <memory>
#include "AutentificareManager.h"
#include <algorithm>
#include "Enumerari.h"

Meniu& Meniu::getInstance() {
    static Meniu instance;
    return instance;
}

Meniu::Meniu() : utilizator(nullptr),transportCurent(nullptr) {}

 void Meniu::rezervaVacanta(AgentieVacante& agentie) {
        int categorie;
        std::cout << "Selecteaza categoria de vacante:\n";
        std::cout << "1. City Break la Plaja\n";
        std::cout << "2. City Break la Munte\n";
        std::cout << "3. Croaziera\n";
        std::cout << "0. Anulare\n";
        std::cin >> categorie;

        if (categorie == 0) {
            std::cout << "Anulare.\n";
            return;
        }

        std::string tipVacanta;
        if (categorie == 1)
            tipVacanta = "Plaja";
        else if (categorie == 2)
            tipVacanta = "Munte";
        else if (categorie == 3)
            tipVacanta = "Croaziera";
        else {
            std::cout << "Categorie invalida.\n";
            return;
        }

        double bugetMaxim;
        std::cout << "\nIntrodu bugetul maxim disponibil (RON): ";
        std::cin >> bugetMaxim;

        std::cout << "\nVacante disponibile pentru categoria selectata in bugetul specificat:\n";
        bool existaVacanteInBuget = false;
        for (const auto& pachet : agentie.getPachete()) {
            if ((tipVacanta == "Plaja" && dynamic_cast<CityBreakPlaja*>(pachet.get())) ||
                (tipVacanta == "Munte" && dynamic_cast<CityBreakMunte*>(pachet.get())) ||
                (tipVacanta == "Croaziera" && dynamic_cast<Croaziera*>(pachet.get()))) {
                if (pachet->getPret() <= bugetMaxim) {
                    std::cout << *pachet;
                    existaVacanteInBuget = true;
                }
            }
        }

        if (!existaVacanteInBuget) {
            std::cout << "Nu exista pachete disponibile in bugetul specificat.\n";
            return;
        }

        int alegere;
        std::cout << "Introdu numarul vacantei dorite (0 pentru anulare): ";
        std::cin >> alegere;

        if (alegere <= 0) {
            std::cout << "Anulare.\n";
            return;
        }

        std::vector<std::shared_ptr<Vacanta>> vacanteFiltrate;
        for (const auto& pachet : agentie.getPachete()) {
            if ((tipVacanta == "Plaja" && dynamic_cast<CityBreakPlaja*>(pachet.get())) ||
                (tipVacanta == "Munte" && dynamic_cast<CityBreakMunte*>(pachet.get())) ||
                (tipVacanta == "Croaziera" && dynamic_cast<Croaziera*>(pachet.get()))) {
                if (pachet->getPret() <= bugetMaxim) {
                    vacanteFiltrate.push_back(pachet);
                }
            }
        }

        if (alegere > (int)vacanteFiltrate.size()) {
            std::cout << "Alegere invalida.\n";
            return;
        }

        auto vacantaSelectata = vacanteFiltrate[alegere - 1];

       std::cout << "\nCazari disponibile pentru " << vacantaSelectata->getDestinatie()
          << " in limita bugetului de " << bugetMaxim << " RON:\n";

auto cazariDisponibile = agentie.getGestiuneCazari().getCazariDisponibile(
    vacantaSelectata->getDestinatie(), 1, bugetMaxim
);

if (cazariDisponibile.empty()) {
    std::cout << "Nu exista cazari disponibile in limita bugetului specificat.\n";
} else {
    agentie.getGestiuneCazari().afiseazaCazariPentruDestinatie(
        vacantaSelectata->getDestinatie(),
        cazariDisponibile
    );
}

        int nrBilete;
        std::cout << "Cate bilete doresti sa rezervi? ";
        std::cin >> nrBilete;
        double pretTotal = nrBilete * vacantaSelectata->getPret();
        std::cin.ignore();
        std::string codPromo;
        std::cout << "Ai un cod promotional? (REDUCERE20 / REDUCERE25 sau Enter pentru niciunul): ";
        std::getline(std::cin, codPromo);

        if (codPromo == "REDUCERE20")
            pretTotal *= 0.8;
        else if (codPromo == "REDUCERE25")
            pretTotal *= 0.75;

        std::cout << "Pret total de platit: " << pretTotal << " RON\n";
        char confirmare;
        std::cout << "Confirma plata? (y/n): ";
        std::cin >> confirmare;

        if (confirmare == 'y' || confirmare == 'Y') {
            try {
                for (int i = 0; i < nrBilete; ++i) {
                    vacantaSelectata->rezervaBilet();
                }


                Statistici::getInstance().adaugaRezervare(vacantaSelectata->getDestinatie(), pretTotal);

                std::cout << "Rezervare efectuata cu succes!\n";
                Statistici::getInstance().afiseazaStatistici();

                selecteazaTransport(nrBilete, tipVacanta);
                if (transportCurent != nullptr) {
                    pretTotal += nrBilete * transportCurent->getPretCursa();
                    std::cout << "Pretul final dupa adaugarea transportului: " << pretTotal << " RON\n";
                }


                if (!std::dynamic_pointer_cast<Croaziera>(vacantaSelectata)) {

                    std::cout << "\nSelecteaza tipul de cazare:\n";
                    std::cout << "1. Hotel\n";
                    std::cout << "2. Vila\n";
                    int tipCazare;
                    std::cin >> tipCazare;

                    std::shared_ptr<Cazare> cazareSelectata;
                    if (tipCazare == 1) {
                        std::cout << "\nHoteluri disponibile:\n";
                        std::vector<std::shared_ptr<Hotel>> hoteluri;
                        hoteluri.push_back(std::make_shared<Hotel>("Grand Hotel", 5, vacantaSelectata->getDestinatie(), 200.0, 50));
                        hoteluri.push_back(std::make_shared<Hotel>("Comfort Inn", 4, vacantaSelectata->getDestinatie(), 150.0, 30));
                        hoteluri.push_back(std::make_shared<Hotel>("City Hotel", 3, vacantaSelectata->getDestinatie(), 100.0, 40));

                        for (size_t i = 0; i < hoteluri.size(); ++i) {
                            std::cout << i + 1 << ". " << hoteluri[i]->getNume() << " (" << hoteluri[i]->getStele()
                                    << " stele) - " << hoteluri[i]->getPretPerNoapte() << " RON/noapte\n";
                        }

                        int alegereHotel;
                        std::cout << "Alege numarul hotelului: ";
                        std::cin >> alegereHotel;

                        if (alegereHotel > 0 && alegereHotel <= static_cast<int>(hoteluri.size())) {
                            cazareSelectata = hoteluri[alegereHotel - 1];
                        }
                    } else if (tipCazare == 2) {
                        std::cout << "\nVile disponibile:\n";
                        std::vector<std::shared_ptr<Vila>> vile;
                        vile.push_back(std::make_shared<Vila>("Vila Panorama", vacantaSelectata->getDestinatie(), 300.0, 1, 8));
                        vile.push_back(std::make_shared<Vila>("Vila Garden", vacantaSelectata->getDestinatie(), 250.0, 1, 6));
                        vile.push_back(std::make_shared<Vila>("Vila Forest", vacantaSelectata->getDestinatie(), 200.0, 1, 4));

                        for (size_t i = 0; i < vile.size(); ++i) {
                            std::cout << i + 1 << ". " << vile[i]->getNume() << " (Capacitate: "
                                    << vile[i]->getCapacitateMaxima() << " persoane) - "
                                    << vile[i]->getPretPerNoapte() << " RON/noapte\n";
                        }

                        int alegereVila;
                        std::cout << "Alege numarul vilei: ";
                        std::cin >> alegereVila;

                        if (alegereVila > 0 && alegereVila <= static_cast<int>(vile.size())) {
                            cazareSelectata = vile[alegereVila - 1];
                        }
                    }
                    if (cazareSelectata) {
                        std::cout << "\nOptiuni suplimentare:\n";
                        char optiune;

                        do {
                            std::cout << "Doriti mic dejun? (d/n): ";
                            std::cin >> optiune;
                            if (optiune != 'd' && optiune != 'D' && optiune != 'n' && optiune != 'N') {
                                std::cout << "Optiune invalida! Va rugam sa introduceti 'd' sau 'n'.\n";
                            }
                        } while (optiune != 'd' && optiune != 'D' && optiune != 'n' && optiune != 'N');
                        cazareSelectata->setMicDejun(optiune == 'd' || optiune == 'D');

                        do {
                            std::cout << "Doriti demipensiune? (d/n): ";
                            std::cin >> optiune;
                            if (optiune != 'd' && optiune != 'D' && optiune != 'n' && optiune != 'N') {
                                std::cout << "Optiune invalida! Va rugam sa introduceti 'd' sau 'n'.\n";
                            }
                        } while (optiune != 'd' && optiune != 'D' && optiune != 'n' && optiune != 'N');
                        cazareSelectata->setDemipensiune(optiune == 'd' || optiune == 'D');

                        do {
                            std::cout << "Doriti all inclusive? (d/n): ";
                            std::cin >> optiune;
                            if (optiune != 'd' && optiune != 'D' && optiune != 'n' && optiune != 'N') {
                                std::cout << "Optiune invalida! Va rugam sa introduceti 'd' sau 'n'.\n";
                            }
                        } while (optiune != 'd' && optiune != 'D' && optiune != 'n' && optiune != 'N');
                        cazareSelectata->setAllInclusive(optiune == 'd' || optiune == 'D');

                        double pretCazare = cazareSelectata->calculeazaPret(vacantaSelectata->getDurataZile(), nrBilete);
                        pretTotal += pretCazare;
                        std::cout << "Cost cazare: " << pretCazare << " RON\n";
                        std::cout << "Pret total final (transport + cazare): " << pretTotal << " RON\n";

                        vacantaSelectata->setCazare(cazareSelectata);
                    }
                } else {
                    std::cout << "\nPentru croaziere, cazarea este inclusa in pret.\n";
                }

                utilizator->adaugaBilet(vacantaSelectata);
                std::cout << "Biletul a fost adaugat in lista ta de rezervari!\n";

            } catch (const BileteEpuizateException& e) {
                std::cout << "Eroare la rezervare: " << e.what() << "\n";
            }
        } else {
            std::cout << "Plata anulata. Rezervarea nu a fost efectuata.\n";
        }
    }

    void Meniu::anuleazaLoc() {
        if (!utilizator) {
            std::cout << "Trebuie să fii autentificat pentru a anula o rezervare.\n";
            return;
        }

        auto it = transporturiRezervate.find(utilizator->getUsername());
        if (it != transporturiRezervate.end()) {
            transportCurent = it->second;
            std::cout << "Rezervarile tale active:\n";
            const auto& rezervari = transportCurent->getRezervariActive();
            if (rezervari.empty()) {
                std::cout << "Nu ai nicio rezervare activă pentru acest transport.\n";
                return;
            }

            std::cout << "Locurile tale rezervate: ";
            for (int loc : rezervari) {
                std::cout << loc << " ";
            }
            std::cout << "\n";

            int loc;
            std::cout << "Introdu numarul locului pe care vrei sa-l anulezi: ";
            std::cin >> loc;

            transportCurent->anuleazaRezervare(loc);
            transportCurent->afiseazaDisponibilitate();


            if (transportCurent->getRezervariActive().empty()) {
                transporturiRezervate.erase(utilizator->getUsername());
                transportCurent = nullptr;
            }
        } else {
            std::cout << "Nu ai nicio rezervare activă pentru niciun mijloc de transport!\n";
        }
    };

void Meniu::executa(AgentieVacante& agentie) {
    std::string nume;
    Autobuz autobuz(20, nume, 120);
    AutentificareManager manager;

    while (true) {
        manager.login();

        if (!manager.esteAutentificat()) {
            std::cout << "Autentificare eșuată. Ieșire din program.\n";
            return;
        }

        std::shared_ptr<Utilizator> utilizatorCurent = manager.getUtilizatorCurent();
        utilizator = utilizatorCurent;
        std::string rol = utilizatorCurent->getRol();
        int optiune;

        std::cout << "\nBine ai venit la agentia de vacante, "
                  << utilizatorCurent->getUsername() << "!\n";

        do {
            utilizatorCurent->afiseazaMeniu();
            std::cout << "Alege o optiune: ";
            std::cin >> optiune;
            std::cin.ignore();

            if (rol == "vizitator") {
                if (optiune == 10) {
                    std::cout << "La revedere!\n";
                    return;
                }
                switch (optiune) {
                    case 1: agentie.listeazaPachete(manager);
                    break;
                    case 2: cautaPachete(agentie);
                    break;
                    case 3: agentie.afiseazaPacheteDupaTip("Munte");
                    break;
                    case 4: agentie.afiseazaPacheteDupaTip("Plaja");
                    break;
                    case 5: agentie.afiseazaPacheteDupaTip("Croaziera");
                    break;
                    case 6: rezervaVacanta(agentie);
                    break;
                    case 7: utilizatorCurent->afiseazaBilete();
                    break;
                    case 8: agentie.sorteazaVacanteDupaPret();
                    break;
                    case 9: anuleazaLoc();
                    break;
                    case 0: manager.logout();
                    break;
                    default:
                        std::cout << "Optiune invalida.\n";
                }
            } else if (rol == "organizator") {
                if (optiune == 3) {
                    std::cout << "La revedere!\n";
                    return;
                }
                switch (optiune) {
                    case 1: adaugaPachet(agentie); break;
                    case 2: agentie.listeazaPachete(manager); break;
                    case 0: manager.logout();
                    break;
                    default: std::cout << "Optiune invalida.\n";
                }
            }
        } while (optiune != 0);
    }
}
void Meniu::adaugaPachet(AgentieVacante& agentie) {
    std::cout << "Adaugare pachet nou:\n";
    agentie.citestePachet();
}
void Meniu::cautaPachete(const AgentieVacante& agentie) {
    std::string oras;
    std::cout << "Introdu orasul pe care il cauti: ";
    std::getline(std::cin, oras);
    agentie.cautaDupaOras(oras);
}

void Meniu::selecteazaTransport(int nrBilete, const std::string& tipVacanta) {
    if (!utilizator) {
        std::cout << "Eroare: Utilizator neinitializat!\n";
        return;
    }

    TransportManager<Avion> managerAvioane;
    TransportManager<Autobuz> managerAutobuze;
    TransportManager<Vapor> managerVapoare;

    managerAvioane.adaugaTransport(std::make_shared<Avion>(25, "BlueAir", 100.0));
    managerAvioane.adaugaTransport(std::make_shared<Avion>(25, "Tarom", 200.0));
    managerAutobuze.adaugaTransport(std::make_shared<Autobuz>(25, "Autobuz1", 150.0));
    managerAutobuze.adaugaTransport(std::make_shared<Autobuz>(25, "Autobuz2", 150.0));
    managerVapoare.adaugaTransport(std::make_shared<Vapor>("Vaporul Mare", 500, 2000.0));

    if (tipVacanta == "Croaziera") {
        std::cout << "Pentru croaziera, transportul este inclus (Vapor):\n";
        managerVapoare.afiseazaTransporturi();
        return;
    }

    std::cout << "Selecteaza mijlocul de transport:\n";
    std::cout << "1. Avion\n";
    std::cout << "2. Autobuz\n";
    int alegere;
    std::cin >> alegere;

    if (alegere == 1) {
        managerAvioane.afiseazaTransporturi();
        std::cout << "Selecteaza avionul dorit: ";
        int index;
        std::cin >> index;

        auto avionSelectat = managerAvioane.selecteazaTransport(index);
        if (avionSelectat) {
            transportCurent = avionSelectat;
            transporturiRezervate[utilizator->getUsername()] = transportCurent;
            std::cout << "Ai selectat avionul: " << avionSelectat->getNume() << "\n";
            avionSelectat->afiseazaDisponibilitate();
            for (int i = 0; i < nrBilete; ++i) {
                int loc;
                std::cout << "Introdu numarul locului pentru biletul " << i + 1 << ": ";
                std::cin >> loc;
                avionSelectat->rezervaLoc(loc);
                avionSelectat->afiseazaDisponibilitate();
                utilizator->adaugaPuncte(50);
            }
            avionSelectat->ruleazaJocAvioane();

            int nrPersoane;
            char esteCopilChar;
            bool esteCopil;

            std::cout << "Cate persoane sunt in grup? ";
            std::cin >> nrPersoane;

            std::cout << "Este pentru copil? (d/n): ";
            std::cin >> esteCopilChar;
            esteCopil = (esteCopilChar == 'd' || esteCopilChar == 'D');

            double pretFinal = avionSelectat->calculeazaReducere(nrPersoane, esteCopil);
            std::cout << "Pret final cu reduceri aplicate: " << pretFinal << " lei\n";
        }
    } else if (alegere == 2) {
        managerAutobuze.afiseazaTransporturi();
        std::cout << "Selecteaza autobuzul dorit: ";
        int index;
        std::cin >> index;

        auto autobuzSelectat = managerAutobuze.selecteazaTransport(index);
        if (autobuzSelectat) {
            transportCurent = autobuzSelectat;
            transporturiRezervate[utilizator->getUsername()] = transportCurent;
            std::cout << "Ai selectat autobuzul: " << autobuzSelectat->getNume() << "\n";
            autobuzSelectat->afiseazaDisponibilitate();
            for (int i = 0; i < nrBilete; ++i) {
                int loc;
                std::cout << "Introdu numarul locului pentru biletul " << i + 1 << ": ";
                std::cin >> loc;
                autobuzSelectat->rezervaLoc(loc);
                autobuzSelectat->afiseazaDisponibilitate();
                utilizator->adaugaPuncte(50);
            }

            int nrPersoane;
            char esteCopilChar;
            bool esteCopil;

            std::cout << "Cate persoane sunt in grup? ";
            std::cin >> nrPersoane;

            std::cout << "Este pentru copil? (d/n): ";
            std::cin >> esteCopilChar;
            esteCopil = (esteCopilChar == 'd' || esteCopilChar == 'D');

            double pretFinal = autobuzSelectat->calculeazaReducere(nrPersoane, esteCopil);
            std::cout << "Pret final cu reduceri aplicate: " << pretFinal << " lei\n";
        }
    } else {
        std::cout << "Alegere invalida.\n";
    }
};