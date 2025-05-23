#include "PachetFactory.h"
#include "CityBreakMunte.h"
#include "CityBreakPlaja.h"
#include "Croaziera.h"

std::shared_ptr<Vacanta> PachetFactory::creeazaPachet(
    TipVacanta tip,
    const std::string& destinatie,
    const std::string& dataIncepere,
    int durataZile,
    double pret,
    const std::string& oras,
    int bilete)
{
    switch(tip) {
        case TipVacanta::CityBreakMunte:
            return std::make_shared<CityBreakMunte>(destinatie, durataZile, oras, dataIncepere, pret, bilete);
        case TipVacanta::CityBreakPlaja:
            return std::make_shared<CityBreakPlaja>(destinatie, durataZile, oras, dataIncepere, pret, bilete);
        case TipVacanta::Croaziera:
            return std::make_shared<Croaziera>(destinatie, durataZile, oras, dataIncepere, pret, bilete);
        default:
            return nullptr;
    }
}
