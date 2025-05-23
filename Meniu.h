
#ifndef MENIU_H
#define MENIU_H
#include <iostream>
#include <vector>
#include <memory>
#include <map>
#include "AgentieVacante.h"
#include "Utilizator.h"
#include "Transport.h"
#include "Vacanta.h"
#include "CityBreakPlaja.h"
#include "CityBreakMunte.h"
#include "Croaziera.h"
#include "Statistici.h"
#include "Cazare.h"
#include "Hotel.h"
#include "Vila.h"

class Meniu {
public:
    Meniu();
    void executa(AgentieVacante& agentie);
private:
    std::shared_ptr<Utilizator> utilizator;
    std::vector<std::shared_ptr<Vacanta>> pachete;
    std::shared_ptr<Transport> transportCurent;
    std::map<std::string, std::shared_ptr<Transport>> transporturiRezervate;

    void adaugaPachet(AgentieVacante& agentie);
    void cautaPachete(AgentieVacante& agentie);
    void selecteazaTransport(int nrBilete, const std::string& tipVacanta);
    void rezervaVacanta(AgentieVacante& agentie);
    void anuleazaLoc();

};
#endif