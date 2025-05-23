
#ifndef MENIU_H
#define MENIU_H
#include <vector>
#include <memory>
#include <map>
#include "AgentieVacante.h"
#include "Utilizator.h"
#include "Transport.h"
#include "Vacanta.h"
#include <string>
class Meniu {
public:
    static Meniu& getInstance();

    void executa(AgentieVacante& agentie);

private:
    Meniu();

    Meniu(const Meniu&) = delete;
    Meniu& operator=(const Meniu&) = delete;

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