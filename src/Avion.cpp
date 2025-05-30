#include "Avion.h"

Avion::Avion(int nrLocuri, const std::string& n, double pretC)
    : Transport(nrLocuri, n), pretCursa(pretC) {}


void Avion::deplaseaza() {
    pozitie += rand() % 3 + 1;
}

void Avion::afiseaza(int lungime) const {
    std::cout << nume << ": ";
    for (int i = 0; i < pozitie && i < lungime; ++i)
        std::cout << "-";
    std::cout << ">\n";
}

int Avion::getPozitie() const {
    return pozitie;
}

const std::string& Avion::getNume() const {
    return  nume;
}

double Avion::getPretCursa() const {
    return pretCursa;
}

double Avion::calculeazaReducere(int nrPersoane, bool copil) const {
    double reducere = 0.0;
    if (copil) reducere += 0.25;
    if (nrPersoane >= 20) reducere += 0.10;
    return pretCursa * (1.0 - reducere);
}

void Avion::print(std::ostream& os) const {
    os << "Avion: " << nume << "|Pret cursa: " << pretCursa << "\n";
}

void Avion::ruleazaJocAvioane() {
    Vizitator utilizator("username", "parola");
    int nrAvioane = 2, lungimeCursa;
    std::cout << "Vrei sa alegi cel mai rapid avion?  Pariaza care va castiga jocul pentru testarea vitezei.\n Introdu lungimea cursei (minute) ";
    std::cin >> lungimeCursa;

    std::vector<Avion> avioane;

    for (int i = 0; i < nrAvioane; ++i) {
        std::string numeavion;
        bool numeValid = false;
        while (!numeValid) {
            std::cout << "Numele agentiei de zbor " << i + 1 << ": ";
            std::cin >> numeavion;
            bool exista = false;
            for (const auto& avion : avioane) {
                if (avion.getNume() == numeavion) {
                    exista = true;
                    break;
                }
            }
            if (exista) {
                std::cout << "Nume deja utilizat! Te rog alege un alt nume.\n";
            } else {
                avioane.emplace_back(25, numeavion, 100);
                numeValid = true;
            }
        }
    }
    std::string avionPariat;
    std::cout << "\nPe ce avion vrei sa pariezi? Introdu numele exact: ";
    std::cin >> avionPariat;

    bool cursaInDesfasurare = true;
    while (cursaInDesfasurare) {
        cursaInDesfasurare = false;
        for (auto& avion : avioane) {
            if (avion.getPozitie() < lungimeCursa) {
                avion.deplaseaza();
                cursaInDesfasurare = true;
            }
            avion.afiseaza(lungimeCursa);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    std::sort(avioane.begin(), avioane.end(), [](const Avion& a, const Avion& b) {
        return a.getPozitie() > b.getPozitie();
    });

    std::cout << "\nClasament final:\n";
    for (size_t i = 0; i < avioane.size(); ++i) {
        std::cout << i + 1 << ". " << avioane[i].getNume() << "\n";
    }
    if (avioane.front().getNume() == avionPariat) {
        std::cout << "\nFelicitari! Ai ghicit castigatorul!";
        utilizator.adaugaPuncte(50);
    } else {
        std::cout << "\nNu ai ghicit castigatorul. Poate ai mai mult noroc data viitoare!\n";
    }
}
