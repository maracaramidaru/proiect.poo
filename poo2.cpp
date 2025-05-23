#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <random>
#include <algorithm>
#include <thread>
#include <chrono>
#include <list>
#include <exception>
#include <map>

class BileteEpuizateException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Nu mai sunt bilete disponibile pentru aceasta vacanta!";
    }
};

class Vacanta {
protected:
    std::string destinatie;
    std::string dataIncepere;
    int durataZile;
    double pret;
    std::string oras;

public:
    Vacanta(std::string destinatie, std::string dataIncepere, int durataZile, double pret, std::string oras){
        this->destinatie = destinatie;
        this->dataIncepere = dataIncepere;
        this->durataZile = durataZile;
        this->pret = pret;
        this->oras = oras;
        }
    virtual ~Vacanta(){}
    virtual void print(std::ostream& os) const = 0;
    virtual bool esteOrasul(std::string orasCautat) const = 0;
    virtual bool rezervaBilet()=0;
    virtual int getBileteDisponibile()const=0;

    friend std::ostream& operator<<(std::ostream& os, const Vacanta& v) {
        v.print(os);
        return os;
    }
    Vacanta(double p) : pret(p) {}

    double getPret() const {
        return pret;
    }
    bool operator<(const Vacanta& other) const {
        return this->getPret() < other.getPret();
    }
    void setDataIncepere(const std::string& dataNoua) {
        dataIncepere = dataNoua;
    }

    Vacanta(const Vacanta& other)
        : destinatie(other.destinatie),
          dataIncepere(other.dataIncepere),
          durataZile(other.durataZile),
          pret(other.pret),
          oras(other.oras) {}

    Vacanta& operator=(const Vacanta& other) {
        if (this != &other) {
            destinatie = other.destinatie;
            dataIncepere = other.dataIncepere;
            durataZile = other.durataZile;
            pret = other.pret;
            oras = other.oras;
        }
        return *this;
    }

};


class Utilizator {
private:
    std::string rol;
    std::string username;
    std::string parola;
    std::vector<std::shared_ptr<Vacanta>> bileteRezervate;
    int puncte = 0;
    std::vector<std::string> cadouri = {
        "Voucher 100 RON",
        "Voucher 50 RON",
        "Cod de participare la tombola",
        "Pachet ghid turistic inclus in sejur"
    };


public:
    Utilizator() {}
    Utilizator(const std::string& u, const std::string& p, const std::string& r = "vizitator")
        : username(u), parola(p), rol(r) {}
    virtual void afiseazaMeniu() const = 0;
    virtual ~Utilizator() {}

    void scadePuncte(int nrPuncte) {
        puncte -= nrPuncte;
    }
        void adaugaPuncte(int nrPuncte) {
            puncte += nrPuncte;
            std::cout << "Ai acumulat " << nrPuncte << " puncte! Total puncte: " << puncte << "\n";

            if (puncte >= 100 && !cadouri.empty()) {
                acordaCadou();
                scadePuncte(nrPuncte);
            }
        }

        void acordaCadou() {
            if (cadouri.empty()) {
                std::cout << "Nu mai sunt cadouri disponibile!\n";
                return;
            }

            int indexAleatoriu = rand() % cadouri.size();

            std::cout << "Felicitari! Ai primit: " << cadouri[indexAleatoriu] << "\n";

            cadouri.erase(cadouri.begin() + indexAleatoriu);
        }

    friend std::istream& operator>>(std::istream& in, Utilizator& utilizator) {
        std::cout << "Introdu rol (vizitator/organizator): ";
        in >> utilizator.rol;
        while (utilizator.rol != "vizitator" && utilizator.rol != "organizator") {
            std::cout << "Rol invalid. Introdu din nou: ";
            in >> utilizator.rol;
        }

        std::cout << "Introdu username: ";
        in >> utilizator.username;

        std::cout << "Introdu parola: ";
        in >> utilizator.parola;

        return in; // Adaugă această linie
    }
    std::string getRol() const {
        return rol;
    }

    std::string getUsername() const {
        return username;
    }

    bool autentificare(const std::string& user, const std::string& pass) const {
        return username == user && parola == pass;
    }

    void adaugaBilet(const std::shared_ptr<Vacanta>& bilet) {
        bileteRezervate.push_back(bilet);
    }

    std::string getParola() const {
        return parola;
    }
    void setParola(const std::string& newParola) {
        parola = newParola;
    }

    void afiseazaBilete() const {
        if (bileteRezervate.empty()) {
            std::cout << "Nu ai bilete rezervate.\n";
            return;
        }

        std::cout << "Biletele tale rezervate:\n";
        std::cout << bileteRezervate.size()<< "bilete pentru:";
        for (const auto& bilet : bileteRezervate) {
            std::cout<<*bilet;
        }
    }
};

class Vizitator : public Utilizator {
public:
    Vizitator(const std::string& u, const std::string& p)
        : Utilizator(u, p, "vizitator") {}

    void afiseazaMeniu() const override{
        std::cout << "\nMeniu Vizitator:\n";
        std::cout << "1. Afiseaza toate pachetele\n";
        std::cout << "2. Cauta pachete dupa oras\n";
        std::cout << "3. Afiseaza pachete Munte\n";
        std::cout << "4. Afiseaza pachete Plaja\n";
        std::cout << "5. Afiseaza City Break-uri\n";
        std::cout << "6. Rezerva City Break\n";
        std::cout << "7. Afiseaza biletele mele\n";
        std::cout << "8. Afiseaza cele mai ieftine pachete de vacanta\n";
        std::cout<<"9.Anulaeaza loc.\n";
        std::cout << "0. Delogare\n";
    }
};

class Organizator : public Utilizator {
public:
    Organizator(const std::string& u, const std::string& p)
        : Utilizator(u, p, "organizator") {}

    void afiseazaMeniu() const override {
        std::cout << "\nMeniu Organizator:\n";
        std::cout << "1. Adauga pachet nou\n";
        std::cout << "2. Afiseaza toate pachetele\n";
        std::cout << "0. Inapoi la meniul principal\n";
    }
};


class AutentificareManager {
private:
    static std::shared_ptr<AutentificareManager> instance;
    std::shared_ptr<Utilizator> utilizatorCurent;

    AutentificareManager() {}

public:
    static std::shared_ptr<AutentificareManager> getInstance() {
        if (!instance) {
            instance = std::shared_ptr<AutentificareManager>(new AutentificareManager());
        }
        return instance;
    }

    void login() {
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

            std::string rol;
            std::cout << "Introdu rol (vizitator/organizator): ";
            std::cin >> rol;
            std::cin.ignore();

            if (rol == "vizitator") {
                utilizatorNou = std::make_shared<Vizitator>("", "");
            } else if (rol == "organizator") {
                utilizatorNou = std::make_shared<Organizator>("", "");
            } else {
                std::cout << "Rol invalid. Nu s-a creat utilizatorul.\n";
                return;
            }

            std::cin >> *utilizatorNou;

            utilizatorCurent = utilizatorNou;

            std::cout << "Utilizator inregistrat si autentificat cu succes!\n";
        }
    }
    std::shared_ptr<Utilizator> getUtilizatorCurent() const {
        return utilizatorCurent;
    }

    bool esteAutentificat() const {
        return utilizatorCurent != nullptr;
    }

    void logout() {
        if (utilizatorCurent) {
            std::cout << "Utilizatorul " << utilizatorCurent->getUsername() << " s-a delogat.\n";
        }
        utilizatorCurent.reset();
    }
};

std::shared_ptr<AutentificareManager> AutentificareManager::instance = nullptr;

class Bilet : public Vacanta {
private:
    static int nrAchizitii;

public:
    void print(std::ostream& os) const override {
        os << "Bilet pentru: " << destinatie << " in " << oras
           << ", data: " << dataIncepere << ", durata: " << durataZile
           << " zile, pret: " << pret << " EUR\n";
    }

    Bilet(std::string destinatie, std::string dataIncepere, int durataZile,
          double pret, std::string oras)
        : Vacanta(destinatie, dataIncepere, durataZile, pret, oras) {
        ++nrAchizitii;
        std::cout << "Bilet cumparat pentru: " << destinatie << ", oras: " << oras << "\n";
    }

    Bilet(const Bilet& other)
        : Vacanta(other.destinatie, other.dataIncepere, other.durataZile, other.pret, other.oras) {
        ++nrAchizitii;
        std::cout << "Bilet COPIAT pentru: " << destinatie << "\n";
    }

    Bilet& operator=(const Bilet& other) {
        if (this != &other) {
            destinatie = other.destinatie;
            dataIncepere = other.dataIncepere;
            durataZile = other.durataZile;
            pret = other.pret;
            oras = other.oras;
            ++nrAchizitii;
            std::cout << "Bilet atribuit pentru: " << destinatie << "\n";
        }
        return *this;
    }

    static void afiseazaNrAchizitii() {
        std::cout << "Total bilete vandute: " << nrAchizitii << "\n";
    }
};

int Bilet::nrAchizitii = 0;


class CityBreakPlaja : public Vacanta {
private:
    int bileteDisponibile;

public:
    CityBreakPlaja(std::string tara, int durata, std::string oras,
                   std::string dataIncepere, double pret, int bilete)
        : Vacanta(tara, dataIncepere, durata, pret, oras),
          bileteDisponibile(bilete) {}

    CityBreakPlaja()
        : Vacanta("Destinație necunoscută", "01-01-2025", 7, 1000, "Necunoscut"),
          bileteDisponibile(0) {}
    CityBreakPlaja(const CityBreakPlaja& other)
    : Vacanta(other), bileteDisponibile(other.bileteDisponibile) {}

    bool esteOrasul(std::string orasCautat) const override {
        return oras == orasCautat;
    }

    int getBileteDisponibile() const override{
        return bileteDisponibile;
    }

    void print(std::ostream& os) const override {
        os << "City Break la Plaja\n";
        os << "Destinatie: " << destinatie << "\n";
        os << "Data Incepere: " << dataIncepere << "\n";
        os << "Durata: " << durataZile << " zile\n";
        os << "Pret: " << pret << " RON\n";
        os << "Bilete disponibile: " << bileteDisponibile << "\n";
    }

    bool rezervaBilet() override {
        if (getBileteDisponibile() > 0) {
            --bileteDisponibile;
            std::cout << "Bilet rezervat! Bilete ramase: " << bileteDisponibile << "\n";
            return true;
        } else {
            throw BileteEpuizateException();
        }
    }

    double calculeazaPretCuReducere(double pretInitial, const std::string& codPromo) {
        if (codPromo == "REDUCERE20")
            return getPret() * 0.8;
        else if (codPromo == "REDUCERE25")
            return getPret() * 0.75;
        else
            return pretInitial;
    }
    // Prietenie pentru operator >>
    friend std::istream& operator>>(std::istream& in, CityBreakPlaja& pachet);
};
// Operator >> pentru citire
std::istream& operator>>(std::istream& in, CityBreakPlaja& pachet) {
    std::cout << "Introdu destinatia: ";
    in >> pachet.destinatie;
    std::cout << "Introdu durata (zile): ";
    in >> pachet.durataZile;
    std::cout << "Introdu orasul: ";
    in >> pachet.oras;
    std::cout << "Introdu data de incepere: ";
    in >> pachet.dataIncepere;
    std::cout << "Introdu pretul: ";
    in >> pachet.pret;
    std::cout << "Introdu numarul de bilete disponibile: ";
    in >> pachet.bileteDisponibile;
    return in;
}


class CityBreakMunte : public Vacanta {
private:
    int bileteDisponibile;

public:
    CityBreakMunte(std::string tara, int durata, std::string oras,
                   std::string dataIncepere, double pret, int bilete)
        : Vacanta(tara, dataIncepere, durata, pret, oras),
          bileteDisponibile(bilete) {}

    CityBreakMunte()
        : Vacanta("Destinație necunoscută", "01-01-2025", 7, 1000, "Necunoscut"),
          bileteDisponibile(0) {}

    CityBreakMunte(const CityBreakMunte& other)
    : Vacanta(other), bileteDisponibile(other.bileteDisponibile) {}

    bool esteOrasul(std::string orasCautat) const override {
        return oras == orasCautat;
    }

    int getBileteDisponibile() const override {
        return bileteDisponibile;
    }

    void print(std::ostream& os) const override {
        os << "City Break la Munte\n";
        os << "Destinatie: " << destinatie << "\n";
        os << "Data Incepere: " << dataIncepere << "\n";
        os << "Durata: " << durataZile << " zile\n";
        os << "Pret: " << pret << " RON\n";
        os << "Bilete disponibile: " << bileteDisponibile << "\n";
    }

    bool rezervaBilet() override {
        if (getBileteDisponibile() > 0) {
            --bileteDisponibile;
            std::cout << "Bilet rezervat! Bilete ramase: " << bileteDisponibile << "\n";
            return true;
        } else {
            throw BileteEpuizateException();
        }
    }

    double calculeazaPretCuReducere(double pretInitial, const std::string& codPromo) {
        if (codPromo == "REDUCERE20")
            return getPret() * 0.8;
        else if (codPromo == "REDUCERE25")
            return getPret() * 0.75;
        else
            return pretInitial;
    }

    friend std::istream& operator>>(std::istream& in, CityBreakMunte& pachet);
};
std::istream& operator>>(std::istream& in, CityBreakMunte& pachet) {
    std::cout << "Introdu destinatia: ";
    in >> pachet.destinatie;
    std::cout << "Introdu durata (zile): ";
    in >> pachet.durataZile;
    std::cout << "Introdu orasul: ";
    in >> pachet.oras;
    std::cout << "Introdu data de incepere: ";
    in >> pachet.dataIncepere;
    std::cout << "Introdu pretul: ";
    in >> pachet.pret;
    std::cout << "Introdu numarul de bilete disponibile: ";
    in >> pachet.bileteDisponibile;
    return in;
}



class Croaziera : public Vacanta {
private:
    int bileteDisponibile;
    int durata;

public:
    Croaziera(std::string destinatie, int durata, std::string oras,
              std::string dataPlecare, double pret, int bilete)
        : Vacanta(destinatie, dataPlecare, durata, pret, oras),
          bileteDisponibile(bilete){}

    Croaziera()
        : Vacanta("Destinație necunoscută", "01-01-2025", 7, 1000, "Necunoscut"),
          bileteDisponibile(0) {}

    Croaziera(const Croaziera& other)
    : Vacanta(other), bileteDisponibile(other.bileteDisponibile), durata(other.durata) {}

    bool esteOrasul(std::string orasCautat) const override {
        return oras == orasCautat;
    }

    int getBileteDisponibile() const {
        return bileteDisponibile;
    }

    bool rezervaBilet() override {
        if (getBileteDisponibile() > 0) {
            --bileteDisponibile;
            std::cout << "Bilet rezervat! Bilete ramase: " << bileteDisponibile << "\n";
            return true;
        } else {
            throw BileteEpuizateException();
        }
    }
    double calculeazaPretCuReducere(double pretInitial, const std::string& codPromo) {
        if (codPromo == "CROAZIERA15")
            return getPret() * 0.85;
        else if (codPromo == "CROAZIERA25")
            return getPret() * 0.75;
        else
            return pretInitial;
    }

    void print(std::ostream& os) const override {
        os << "Croaziera:\n";
        os << "Destiatia: " << destinatie << "\n";
        os << "Data plecare: " << dataIncepere << "\n";
        os << "Durata: " << durata << " zile\n";
        os << "Oras imbarcare: " << oras << "\n";
        os << "Pret: " << pret << " RON\n";
        os << "Bilete disponibile: " << bileteDisponibile << "\n";
    }
    friend std::istream& operator>>(std::istream& in, Croaziera& pachet);
};
// Operator >> pentru citire
std::istream& operator>>(std::istream& in, Croaziera& pachet) {
    std::cout << "Introdu destinatia: ";
    in >> pachet.destinatie;
    std::cout << "Introdu durata (zile): ";
    in >> pachet.durataZile;
    std::cout << "Introdu orasul de imbarcare: ";
    in >> pachet.oras;
    std::cout << "Introdu data de plecare: ";
    in >> pachet.dataIncepere;
    std::cout << "Introdu pretul: ";
    in >> pachet.pret;
    std::cout << "Introdu numarul de bilete disponibile: ";
    in >> pachet.bileteDisponibile;
    return in;
}


class PachetFactory {
public:
    static std::shared_ptr<Vacanta> creeazaPachet(const std::string& tip, const std::string& destinatie, const std::string& dataIncepere, int durataZile, double pret, const std::string& oras, int bilete = 0) {
        if (tip == "Munte") {
            return std::make_shared<CityBreakMunte>(destinatie, durataZile, oras, dataIncepere, pret, bilete);
        } else if (tip == "Plaja") {
            return std::make_shared<CityBreakPlaja>(destinatie, durataZile, oras, dataIncepere, pret, bilete);
        } else if (tip == "Croaziera") {
            return std::make_shared<Croaziera>(destinatie, durataZile, oras, dataIncepere, pret, bilete);
        }
        return nullptr;
    }
};

class AgentieVacante {
private:
    std::vector<std::shared_ptr<Vacanta>> pachete;
    std::vector<std::shared_ptr<CityBreakMunte>> pachetemunte;
    std::vector<std::shared_ptr<CityBreakPlaja>> pacheteplaja;
    std::vector<std::shared_ptr<Croaziera>> croaziere;
    int tip;
public:

    void listeazaPachete() ;
    void afiseazaPacheteDupaTip(const std::string& tip) const;
    void cautaDupaOras(std::string orasCautat) const;
    void afiseazaPacheteMunte() const;
    void afiseazaPachetePlaja() const;
    void afiseazaCroaziere() const;
    void adaugaPachet(const std::string &tipPachet, const std::string &destinatie, const std::string &dataIncepere, int durataZile, double pret, const std::string &oras, int bilete = 0);
    void sorteazaVacanteDupaPret();
    const std::vector<std::shared_ptr<Vacanta>>& getPachete() const {
        return pachete;
    }

void citestePachet() {
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

};


void AgentieVacante::cautaDupaOras(std::string orasCautat) const {
    std::cout << "\nCautare dupa oras: " << orasCautat << "\n";
    bool gasit = false;
    for (auto pachet : pachete) {
        if (pachet->esteOrasul(orasCautat)) {
            std::cout << *pachet;
            gasit = true;
        }
    }
    if (!gasit) {
        std::cout << "Nu exista pachete pentru orasul " << orasCautat << ".\n";
    }
}

void AgentieVacante::adaugaPachet(const std::string &tipPachet, const std::string &destinatie, const std::string &dataIncepere, int durataZile, double pret, const std::string &oras, int bilete) {
    std::shared_ptr<Vacanta> pachet;

    if (tipPachet == "Plaja") {
        pachet = std::make_shared<CityBreakPlaja>(destinatie, durataZile, oras, dataIncepere, pret, bilete);
        std::cout << "Sejur la plaja adaugat: " << destinatie << std::endl;
    } else if (tipPachet == "Munte") {
        pachet = std::make_shared<CityBreakMunte>(destinatie, durataZile, oras, dataIncepere, pret, bilete);
        std::cout << "Sejur la munte adaugat: " << destinatie << std::endl;
    } else if (tipPachet == "Croaziera") {
        pachet = std::make_shared<Croaziera>(destinatie, durataZile, oras, dataIncepere, pret, bilete);
        std::cout << "Pachet de Croaziera adaugat: " << oras <<"-"<<destinatie<< std::endl;
    } else {
        std::cout << "Tipul de pachet nu este valid!" << std::endl;
        return;
    }

    pachete.push_back(pachet);
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
        if (tip == "Munte" && dynamic_cast<CityBreakMunte*>(pachet.get())) {
            std::cout << *pachet;
            gasit = true;
        }
        else if (tip == "Plaja" && dynamic_cast<CityBreakPlaja*>(pachet.get())) {
            std::cout << *pachet;
            gasit = true;
        }
        else if (tip == "Croaziera" && dynamic_cast<Croaziera*>(pachet.get())) {
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

    char optiune;
    std::cout << "\nDoresti sa selectezi si sa copiezi un pachet? (y/n): ";
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


class Transport {
protected:
    std::list<bool> locuri;
    int numarLocuri;
    std::string nume;
    std::vector<int> rezervariActive; // Track active reservations

public:
    Transport() : numarLocuri(0) {}
    virtual void print(std::ostream& os) const = 0;
    virtual double getPretCursa() const = 0;
    virtual double calculeazaReducere(int nrPersoane, bool copil) const = 0;

    Transport(int nrLocuri) : numarLocuri(nrLocuri), locuri(nrLocuri, false) {}

    friend std::ostream& operator<<(std::ostream& os, const Transport& t) {
        t.print(os);
        return os;
    }

    virtual ~Transport() {}

    std::string getNume() const {
        return nume;
    }

    void rezervaLoc(int loc) {
        if (loc < 1 || loc > numarLocuri) {
            std::cout << "Loc invalid! Introdu un loc între 1 și " << numarLocuri << ".\n";
            return;
        }

        auto it = locuri.begin();
        std::advance(it, loc - 1);

        if (*it) {
            std::cout << "Locul " << loc << " este deja ocupat!\n";
        } else {
            *it = true;
            rezervariActive.push_back(loc); // Add to active reservations
            std::cout << "Locul " << loc << " a fost rezervat!\n";
        }
    }

    void anuleazaRezervare(int loc) {
        if (loc < 1 || loc > numarLocuri) {
            std::cout << "Loc invalid! Introdu un loc între 1 și " << numarLocuri << ".\n";
            return;
        }

        auto it = locuri.begin();
        std::advance(it, loc - 1);

        // Check if the seat is in active reservations
        auto rezervare = std::find(rezervariActive.begin(), rezervariActive.end(), loc);
        if (rezervare == rezervariActive.end()) {
            std::cout << "Locul " << loc << " nu era rezervat de tine!\n";
            return;
        }

        *it = false;
        rezervariActive.erase(rezervare); // Remove from active reservations
        std::cout << "Rezervarea locului " << loc << " a fost anulată cu succes!\n";
    }

    void afiseazaDisponibilitate() const {
        std::cout << "\nDisponibilitate locuri:\n";
        int index = 1;
        for (bool ocupat : locuri) {
            bool esteRezervareActiva = std::find(rezervariActive.begin(), rezervariActive.end(), index) != rezervariActive.end();
            std::cout << index << (ocupat ? (esteRezervareActiva ? "[R]" : "[x]") : "[ ]") << " ";
            if (index % 2 == 0) std::cout << "\n";
            index++;
        }
        std::cout << "\n";
    }

    const std::vector<int>& getRezervariActive() const {
        return rezervariActive;
    }
};


class Avion: public Transport {
    std::string nume;
    int pozitie = 0;
    double pretCursa;
public:
    Avion(const int nrLocuri,const std::string& n,double pretC) : Transport(25), nume(n) ,pretCursa(pretC){}
    void deplaseaza() {
        pozitie += rand() % 3 + 1;
    }
    void afiseaza(int lungime) const {
        std::cout << nume << ": ";
        for (int i = 0; i < pozitie && i < lungime; ++i)
            std::cout << "-";
        std::cout << ">\n";
    }
    int getPozitie() const {
        return pozitie;
    }
    std::string getNume() const {
        return nume;
    }
    double getPretCursa() const override {
        return pretCursa;
    }
    double calculeazaReducere(int nrPersoane, bool copil) const override {
        double reducere = 0.0;

        if (copil) reducere += 0.25;
        if (nrPersoane >= 20) reducere += 0.10;

        return pretCursa * (1.0 - reducere);
    }

    void print(std::ostream& os) const override {
        os << "Avion: " << nume <<"|Pret cursa: "<<pretCursa<<"\n";
    }


void ruleazaJocAvioane() {
        Vizitator utilizator("username", "parola");
    int nrAvioane=2, lungimeCursa;
    std::cout << "Vrei sa alegi cel mai rapid avion?  Pariaza care va castiga jocul pentru testarea vitezei.\n Introdu lungimea cursei (minute) ";
    std::cin >> lungimeCursa;

    std::vector<Avion> avioane;

        for (int i = 0; i < nrAvioane; ++i) {
            std::string nume;
            bool numeValid = false;

            while (!numeValid) {
                std::cout << "Numele agentiei de zbor " << i + 1 << ": ";
                std::cin >> nume;
                bool exista = false;
                for (const auto& avion : avioane) {
                    if (avion.getNume() == nume) {
                        exista = true;
                        break;
                    }
                }

                if (exista) {
                    std::cout << "Nume deja utilizat! Te rog alege un alt nume.\n";
                } else {
                    avioane.push_back(Avion(25, nume,100));
                    numeValid = true;
                }
            }
        }
        std::string avionPariat;
        std::cout << "\nPe ce avion vrei să pariezi? Introdu numele exact: ";
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
        std::cout << i + 1 << ". " << avioane[i].getNume()<<"\n";
    }
        if (avioane.front().getNume() == avionPariat) {
            std::cout << "\nFelicitari! Ai ghicit castigatorul!";
            utilizator.adaugaPuncte(50);
        } else {
            std::cout << "\nNu ai ghicit castigatorul. Poate ai mai mult noroc data viitoare!\n";
        }

}
};



class Autobuz:public Transport {
private:
    std::string nume;
    int pozitie;
    double pretCursa;
public:
    Autobuz(int nrLocuri,const std::string& n,double pretC): Transport(25), nume(n),pretCursa(pretC){}
    double getPretCursa() const override {
        return pretCursa;
    }
    double calculeazaReducere(int nrPersoane, bool copil) const override {
        double reducere = 0.0;

        if (copil) reducere += 0.25;
        if (nrPersoane >= 20) reducere += 0.10;

        return pretCursa * (1.0 - reducere);
    }

    void print(std::ostream& os) const override {
        os << "Autobuz: " << nume <<"|Pret cursa:"<<pretCursa<< "\n";
    }

};



class Vapor :public Transport {
private:
    std::string nume;
    int capacitate;
    double pretCursa;
public:

    Vapor() : Transport(20), nume("Necunoscut"), capacitate(500) {}

    Vapor(const std::string& n, int cap, double pretC)
        : Transport(30),nume(n), capacitate(cap), pretCursa(pretC) {}

    double getPretCursa() const override {
        return pretCursa;
    }

    double calculeazaReducere(int nrPersoane, bool copil) const override {
        double reducere = 0.0;

        if (copil) reducere += 0.25;
        if (nrPersoane >= 20) reducere += 0.10;

        return pretCursa * (1.0 - reducere);
    }

    void print(std::ostream& os) const override {
        os << "Vapor: " << nume << " | Capacitate: " << capacitate
           << " persoane | Pret cursa: " << pretCursa << " RON\n";
    }

};

template <typename T>
class TransportManager {
private:
    std::vector<std::shared_ptr<T>> transporturi;

public:
    void adaugaTransport(const std::shared_ptr<T>& transport) {
        transporturi.push_back(transport);
    }

    void afiseazaTransporturi() const {
        if (transporturi.empty()) {
            std::cout << "Nu exista mijloace de transport disponibile.\n";
            return;
        }

        std::cout << "\nMijloace de transport disponibile:\n";
        for (size_t i = 0; i < transporturi.size(); ++i) {
            std::cout << i + 1 << ". ";
            transporturi[i]->print(std::cout);
        }
    }

    std::shared_ptr<T> selecteazaTransport(int index) {
        if (index < 1 || index > (int)transporturi.size()) {
            std::cout << "Index invalid! Selectie anulata.\n";
            return nullptr;
        }
        return transporturi[index - 1];
    }
};

class Meniu {
public:
    void executa(AgentieVacante& agentie);
    Meniu();
private:
    std::shared_ptr<Utilizator> utilizator;
    std::vector<std::shared_ptr<Vacanta>> pachete;
    std::shared_ptr<Transport> transportCurent;
    std::map<std::string, std::shared_ptr<Transport>> transporturiRezervate;

    void adaugaPachet(AgentieVacante& agentie);
    void cautaPachete(AgentieVacante& agentie);

    void selecteazaTransport(int nrBilete, const std::string& tipVacanta) {
        TransportManager<Avion> managerAvioane;
        TransportManager<Autobuz> managerAutobuze;
        TransportManager<Vapor> managerVapoare;

        managerAvioane.adaugaTransport(std::make_shared<Avion>(25, "BlueAir", 100.0));
        managerAvioane.adaugaTransport(std::make_shared<Avion>(25, "Tarom", 200.0));
        managerAutobuze.adaugaTransport(std::make_shared<Autobuz>(25, "Autobuz1", 150.0));
        managerAutobuze.adaugaTransport(std::make_shared<Autobuz>(25, "Autobuz2", 150.0));
        managerVapoare.adaugaTransport(std::make_shared<Vapor>("Vaporul Mare", 500, 2000.0));

        if (tipVacanta == "Croaziera") {
            std::cout << "Pentru croazieră, transportul este inclus (Vapor):\n";
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

                std::cout << "Câte persoane sunt în grup? ";
                std::cin >> nrPersoane;

                std::cout << "Este pentru copil? (d/n): ";
                std::cin >> esteCopilChar;
                esteCopil = (esteCopilChar == 'd' || esteCopilChar == 'D');

                double pretFinal = avionSelectat->calculeazaReducere(nrPersoane, esteCopil);
                std::cout << "Preț final cu reduceri aplicate: " << pretFinal << " lei\n";
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

                std::cout << "Câte persoane sunt în grup? ";
                std::cin >> nrPersoane;

                std::cout << "Este pentru copil? (d/n): ";
                std::cin >> esteCopilChar;
                esteCopil = (esteCopilChar == 'd' || esteCopilChar == 'D');

                double pretFinal = autobuzSelectat->calculeazaReducere(nrPersoane, esteCopil);
                std::cout << "Preț final cu reduceri aplicate: " << pretFinal << " lei\n";
            }
        } else {
            std::cout << "Alegere invalidă.\n";
        }
    }

    void rezervaVacanta(AgentieVacante& agentie) {
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

        std::cout << "\nVacante disponibile pentru categoria selectata:\n";
        agentie.afiseazaPacheteDupaTip(tipVacanta);

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
                vacanteFiltrate.push_back(pachet);
            }
        }

        if (alegere > (int)vacanteFiltrate.size()) {
            std::cout << "Alegere invalida.\n";
            return;
        }

        auto vacantaSelectata = vacanteFiltrate[alegere - 1];

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
                for (int i = 0; i < nrBilete; ++i)
                    vacantaSelectata->rezervaBilet();

                std::cout << "Rezervare efectuata cu succes!\n";

                selecteazaTransport(nrBilete, tipVacanta);
                if (transportCurent != nullptr) {
                    pretTotal += nrBilete * transportCurent->getPretCursa();
                    std::cout << "Pretul final dupa adaugarea transportului: " << pretTotal << " RON\n";
                }
            } catch (const BileteEpuizateException& e) {
                std::cout << "Eroare la rezervare: " << e.what() << "\n";
            }
        } else {
            std::cout << "Plata anulata. Rezervarea nu a fost efectuata.\n";
        }
    }

    void anuleazaLoc() {
        if (transportCurent) {
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
        } else {
            std::cout << "Nu ai selectat niciun transport pentru care sa poti anula rezervarea!\n";
        }
    }
};

Meniu::Meniu() :
    utilizator(nullptr),
    transportCurent(nullptr)
    {
}
void Meniu::executa(AgentieVacante& agentie) {
    auto manager = AutentificareManager::getInstance();

    while (true) {
        manager->login();

        if (!manager->esteAutentificat()) {
            std::cout << "Autentificare eșuată. Ieșire din program.\n";
            return;
        }

        utilizator = manager->getUtilizatorCurent();
        std::string rol = utilizator->getRol();
        int optiune;

        std::cout << "\nBine ai venit la agentia de vacante, "
                  << utilizator->getUsername() << "!\n";

        do {
            utilizator->afiseazaMeniu();
            std::cout << "Alege o optiune: ";
            std::cin >> optiune;
            std::cin.ignore();

            if (rol == "vizitator") {
                switch (optiune) {
                    case 1: agentie.listeazaPachete();
                    break;
                    case 2: cautaPachete(agentie);
                    break;
                    case 3: agentie.afiseazaPacheteDupaTip("Munte");
                    break;
                    case 4: agentie.afiseazaPacheteDupaTip("Plaja");
                    break;
                    case 5: agentie.afiseazaPacheteDupaTip("CityBreak");
                    break;
                    case 6: rezervaVacanta(agentie);
                    break;
                    case 7: utilizator->afiseazaBilete();
                    break;
                    case 8: agentie.sorteazaVacanteDupaPret();
                    break;
                    case 9: anuleazaLoc();
                    break;
                    case 0:
                        manager->logout();
                        utilizator = nullptr;
                    break;
                    default: std::cout << "Optiune invalida.\n";
                }
            } else if (rol == "organizator") {
                switch (optiune) {
                    case 1: adaugaPachet(agentie); break;
                    case 2: agentie.listeazaPachete(); break;
                    case 0:
                        manager->logout();
                        utilizator = nullptr;
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
void Meniu::cautaPachete(AgentieVacante& agentie) {
    std::string oras;
    std::cin.ignore();
    std::cout << "Introdu orasul pe care il cauti: ";
    std::getline(std::cin, oras);
    agentie.cautaDupaOras(oras);
}

int main() {

    AgentieVacante agentie;
    agentie.adaugaPachet("Plaja", "Grecia", "2025-06-10", 7, 2500.0, "Atena",15);
    agentie.adaugaPachet("Munte", "Alpi", "2025-07-01", 5, 1800.0, "Innsbruck",13);
    agentie.adaugaPachet("Plaja", "Italia", "10-11-2025", 10, 4000.0, "Venetia", 20);
    agentie.adaugaPachet("Croaziera","Franta","12-08-2025",3,1000.0,"Cannes",10);
    Meniu meniu;
    meniu.executa(agentie);
    return 0;
}
//
// Created by marac on 5/20/2025.
//
