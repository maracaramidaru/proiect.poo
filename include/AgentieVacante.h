#ifndef AGENTIEVACANTE_H
#define AGENTIEVACANTE_H

#include <vector>
#include <memory>
#include <algorithm>
#include "Vacanta.h"
#include "CityBreakPlaja.h"
#include "CityBreakMunte.h"
#include "Croaziera.h"
#include "PachetFactory.h"
#include "GestiuneCazari.h"
#include "AutentificareManager.h"
#include "Enumerari.h"

class AgentieVacante {
private:
    std::vector<std::shared_ptr<Vacanta>> pachete;
    GestiuneCazari gestiuneCazari;

public:
    GestiuneCazari& getGestiuneCazari();

    void listeazaPachete(const AutentificareManager& manager);
    void afiseazaPacheteDupaTip(const std::string& tip) const;
    void cautaDupaOras(const std::string& orasCautat) const;
    void adaugaPachet(TipVacanta tipPachet, const std::string &destinatie, const std::string &dataIncepere, int durataZile, double pret, const std::string &oras, int bilete = 0);
    void sorteazaVacanteDupaPret();
    const std::vector<std::shared_ptr<Vacanta>>& getPachete() const;

    void citestePachet();
};

#endif // AGENTIEVACANTE_H
