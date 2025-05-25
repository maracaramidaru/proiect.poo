
#include <memory>
#include "../include/AgentieVacante.h"
#include "../include/Hotel.h"
#include "../include/Vila.h"
#include "../include/Enumerari.h"
#include "../include/Meniu.h"

int main() {
    AgentieVacante agentie;

    auto hotelAtena = std::make_shared<Hotel>("Hotel Olympic", 4, "Atena", 500.0, 50);
    hotelAtena->setMicDejun(true);
    hotelAtena->setPiscina(true);
    hotelAtena->adaugaFacilitate("Wi-Fi gratuit");
    hotelAtena->adaugaFacilitate("Parcare");
    agentie.getGestiuneCazari().adaugaCazare("Grecia", hotelAtena);

    auto hotelAlpi = std::make_shared<Hotel>("Mountain Resort", 5, "Innsbruck", 800.0, 30);
    hotelAlpi->setAllInclusive(true);
    hotelAlpi->setSpa(true);
    hotelAlpi->adaugaFacilitate("Centru SPA");
    hotelAlpi->adaugaFacilitate("Sala fitness");
    agentie.getGestiuneCazari().adaugaCazare("Innsbruck", hotelAlpi);

    auto vilaVenetia = std::make_shared<Vila>("Villa Venezia",3, "Venetia", 1200.0, 5, 10);
    vilaVenetia->setBarbeque(true);
    vilaVenetia->adaugaFacilitate("Gradina privata");
    vilaVenetia->adaugaFacilitate("Vedere la canal");
    agentie.getGestiuneCazari().adaugaCazare("Italia", vilaVenetia);

    agentie.adaugaPachet(TipVacanta::CityBreakPlaja, "Grecia", "2025-06-10", 7, 2500.0, "Atena", 15);
    agentie.adaugaPachet(TipVacanta::CityBreakMunte, "Innsbruck", "2025-07-01", 5, 1800.0, "Innsbruck", 13);
    agentie.adaugaPachet(TipVacanta::CityBreakPlaja, "Italia", "10-11-2025", 10, 4000.0, "Venetia", 20);
    agentie.adaugaPachet(TipVacanta::Croaziera, "Franta", "12-08-2025", 3, 1000.0, "Cannes", 10);
    agentie.adaugaPachet(TipVacanta::CityBreakMunte, "Elvetia", "10-03-2026",5,1200.0, "Bern", 7);

    Meniu& meniu = Meniu::getInstance();
    meniu.executa(agentie);
    return 0;
}
