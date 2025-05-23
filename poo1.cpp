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
//#include <numeric>

enum class TipVacanta {
    CityBreakPlaja,
    CityBreakMunte,
    Croaziera
};

enum class TipUtilizator {
    Vizitator,
    Organizator
};

enum class TipTransport {
    Avion,
    Autobuz,
    Vapor
};

enum class FacilitatiHotel {
    MicDejun,
    Spa,
    Piscina,
    Restaurant,
    Parcare,
    WiFi
};

class BileteEpuizateException : public std::exception {
public:
    [[nodiscard]] const char* what() const noexcept override {
        return "Nu mai sunt bilete disponibile pentru aceasta vacanta!";
    }
};

class DataInvalidaException : public std::exception {
private:
    std::string mesaj;
public:
    DataInvalidaException(const std::string& msg) : mesaj(msg) {}
    
    [[nodiscard]] const char* what() const noexcept override {
        return mesaj.c_str();
    }
};

class ValidatorData {
public:
    static void valideazaData(const std::string& data) {
        if (data.length() != 10 || data[2] != '-' || data[5] != '-') {
            throw DataInvalidaException("Format data invalid. Folositi formatul: ZZ-LL-AAAA");
        }

        try {
            int zi = std::stoi(data.substr(0, 2));
            int luna = std::stoi(data.substr(3, 2));
            int an = std::stoi(data.substr(6, 4));

            if (luna < 1 || luna > 12) {
                throw DataInvalidaException("Luna invalida. Luna trebuie sa fie intre 1 si 12.");
            }

            int zileLuna[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
            if (an % 4 == 0 && (an % 100 != 0 || an % 400 == 0)) {
                zileLuna[1] = 29;
            }

            if (zi < 1 || zi > zileLuna[luna - 1]) {
                throw DataInvalidaException("Zi invalida pentru luna specificata.");
            }


        } catch (const std::invalid_argument&) {
            throw DataInvalidaException("Format data invalid. Folositi numere pentru zi, luna si an.");
        }
    }
};

class Cazare {
protected:
    enum class TipCazare {
        hotel,
        pensiune,
        apartament,
        vila,
        resort
    };

    std::string nume;
    TipCazare tip;
    int stele;
    std::string locatie;
    double pretPerNoapte;
    std::vector<std::string> facilitati;
    int camereDisponibile;
    bool micDejun;
    bool demipensiune;
    bool allInclusive;

public:
    Cazare(const std::string& nume, TipCazare tip, int stele, const std::string& locatie, 
           double pretPerNoapte, int camereDisponibile)
        : nume(nume), tip(tip), stele(stele), locatie(locatie), 
          pretPerNoapte(pretPerNoapte), camereDisponibile(camereDisponibile),
          micDejun(false), demipensiune(false), allInclusive(false) {}

    virtual ~Cazare() = default;

    virtual double calculeazaPret(int nrNopti, int nrPersoane) {
        double pretBaza = pretPerNoapte * nrNopti;
        if (micDejun) pretBaza += 50 * nrNopti * nrPersoane;
        if (demipensiune) pretBaza += 100 * nrNopti * nrPersoane;
        if (allInclusive) pretBaza += 200 * nrNopti * nrPersoane;
        return pretBaza;
    }

    bool verificaDisponibilitate(int nrCamere) const {
        return camereDisponibile >= nrCamere;
    }

    void adaugaFacilitate(const std::string& facilitate) {
        facilitati.push_back(facilitate);
    }

    const std::vector<std::string>& getFacilitati() const {
        return facilitati;
    }

    const std::string& getNume() const {
        return nume;
    }
    const std::string& getLocatie() const {
        return locatie;
    }
    double getPretPerNoapte() const {
        return pretPerNoapte;
    }
    int getStele() const {
        return stele;
    }
    
    void setMicDejun(bool include) {
        micDejun = include;
    }
    void setDemipensiune(bool include) {
        demipensiune = include;
    }
    void setAllInclusive(bool include) {
        allInclusive = include;
    }
};

class Hotel : public Cazare {
private:
    bool arePiscina;
    bool areSpa;
    bool areRestaurant;

public:
    Hotel(const std::string& nume, int stele, const std::string& locatie, 
          double pretPerNoapte, int camereDisponibile)
        : Cazare(nume, TipCazare::hotel, stele, locatie, pretPerNoapte, camereDisponibile),
          arePiscina(false), areSpa(false), areRestaurant(true) {}

    void setPiscina(bool are) {
        arePiscina = are;
    }
    void setSpa(bool are) {
        areSpa = are;
    }
    void setRestaurant(bool are) {
        areRestaurant = are;
    }

    double calculeazaPret(int nrNopti, int nrPersoane) override {
        double pretTotal = Cazare::calculeazaPret(nrNopti, nrPersoane);
        if (areSpa) pretTotal += 100 * nrNopti;
        if (arePiscina) pretTotal += 50 * nrNopti;
        return pretTotal;
    }
};

class Vila : public Cazare {
private:
    bool areBucatarie;
    bool areGradina;
    bool areBarbeque;
    int capacitateMaxima;

public:
    Vila(const std::string& nume, const std::string& locatie, 
         double pretPerNoapte, int camereDisponibile, int capacitateMaxima)
        : Cazare(nume, TipCazare::vila, 0, locatie, pretPerNoapte, camereDisponibile),
          areBucatarie(true), areGradina(true), areBarbeque(false),
          capacitateMaxima(capacitateMaxima) {}

    void setBarbeque(bool are) {
        areBarbeque = are;
    }
    int getCapacitateMaxima() const {
        return capacitateMaxima;
    }

    double calculeazaPret(int nrNopti, int nrPersoane) override {
        if (nrPersoane > capacitateMaxima) {
            throw std::runtime_error("Numarul de persoane depaseste capacitatea vilei!");
        }
        return Cazare::calculeazaPret(nrNopti, nrPersoane);
    }
};

class GestiuneCazari {
private:
    std::map<std::string, std::vector<std::shared_ptr<Cazare>>> cazariPerDestinatie;

public:
    void adaugaCazare(const std::string& destinatie, std::shared_ptr<Cazare> cazare) {
        cazariPerDestinatie[destinatie].push_back(cazare);
    }

    std::vector<std::shared_ptr<Cazare>> getCazariDisponibile(
        const std::string& destinatie,
        int nrPersoane,
        double bugetMaxim
    ) {
        std::vector<std::shared_ptr<Cazare>> rezultate;
        
        auto it = cazariPerDestinatie.find(destinatie);
        if (it == cazariPerDestinatie.end()) {
            return rezultate;
        }

        for (const auto& cazare : it->second) {
            if (cazare->verificaDisponibilitate(1) && 
                cazare->getPretPerNoapte() * nrPersoane <= bugetMaxim) {
                rezultate.push_back(cazare);
            }
        }
        
        return rezultate;
    }

    void afiseazaCazariPentruDestinatie(const std::string& destinatie) const {
        auto it = cazariPerDestinatie.find(destinatie);
        if (it == cazariPerDestinatie.end()) {
            std::cout << "Nu exista cazari disponibile pentru destinatia " << destinatie << "\n";
            return;
        }

        std::cout << "\nCazari disponibile in " << destinatie << ":\n";
        for (const auto& cazare : it->second) {
            std::cout << "\nNume: " << cazare->getNume()
                     << "\nStele: " << cazare->getStele()
                     << "\nPret per noapte: " << cazare->getPretPerNoapte() << " RON"
                     << "\nFacilitati: ";
            
            const auto& facilitati = cazare->getFacilitati();
            if (facilitati.empty()) {
                std::cout << "Nu sunt specificate";
            } else {
                for (const auto& facilitate : facilitati) {
                    std::cout << facilitate << ", ";
                }
            }
            std::cout << "\n------------------\n";
        }
    }
};

class Vacanta {
protected:
    std::string destinatie;
    std::string dataIncepere;
    int durataZile;
    double pret;
    std::string oras;
    std::shared_ptr<Cazare> cazare;

public:
    Vacanta(std::string destinatie, std::string dataIncepere, int durataZile, double pret, std::string oras)
        : destinatie(destinatie),
          dataIncepere(dataIncepere),
          durataZile(durataZile),
          pret(pret),
          oras(oras),
          cazare(nullptr) {
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

    int getDurataZile() const {
        return durataZile;
    }

    std::string getDestinatie() const {
        return destinatie;
    }

    bool operator<(const Vacanta& other) const {
        return this->getPret() < other.getPret();
    }

    void setDataIncepere(std::string dataNoua) {
        while (true) {
            try {
                ValidatorData::valideazaData(dataNoua);
                dataIncepere = dataNoua;
                return;
            } catch (const DataInvalidaException& e) {
                std::cout << "Eroare la setarea datei: " << e.what() << "\n";
                std::cout << "Introdu din nou data (format: DD-MM-YYYY): ";
                if (!(std::cin >> dataNoua)) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
            }
        }
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

    void setCazare(std::shared_ptr<Cazare> cazareNoua) {
        cazare = cazareNoua;
        if (cazare) {
            pret += cazare->calculeazaPret(durataZile, 1);
        }
    }

    std::shared_ptr<Cazare> getCazare() const {
        return cazare;
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
        std::cout << "Introdu username: ";
        in.ignore();
        if (!std::getline(in, utilizator.username)) {
            in.clear();
            return in;
        }

        std::cout << "Introdu parola: ";
        if (!std::getline(in, utilizator.parola)) {
            in.clear();
            return in;
        }

        return in;
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
        std::cout << bileteRezervate.size() << " bilete pentru:\n";
        for (const auto& bilet : bileteRezervate) {
            std::cout << *bilet;
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
        std::cout << "5. Afiseaza Croaziere\n";
        std::cout << "6. Rezerva vacanta\n";
        std::cout << "7. Afiseaza biletele mele\n";
        std::cout << "8. Afiseaza cele mai ieftine pachete de vacanta\n";
        std::cout << "9. Anuleaza loc\n";
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
    std::shared_ptr<Utilizator> utilizatorCurent;
    AutentificareManager() {}

public:
    static std::shared_ptr<AutentificareManager> getInstance() {
        static std::shared_ptr<AutentificareManager> instance(new AutentificareManager());
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

            TipUtilizator tipUtilizator;
            bool rolValid = false;

            while (!rolValid) {
                std::string rol;
                std::cout << "Introdu rol (1 pentru vizitator, 2 pentru organizator): ";
                int alegereRol;

                if (std::cin >> alegereRol) {
                    switch(alegereRol) {
                        case 1:
                            tipUtilizator = TipUtilizator::Vizitator;
                            utilizatorNou = std::make_shared<Vizitator>("", "");
                            rolValid = true;
                            break;
                        case 2:
                            tipUtilizator = TipUtilizator::Organizator;
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
        : Vacanta("Destinatie necunoscuta", "01-01-2025", 7, 1000, "Necunoscut"),
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
        if (cazare) {
            os << "Cazare: " << cazare->getNume()
               << " (" << cazare->getStele() << " stele)"
               << "\nPret cazare per noapte: " << cazare->getPretPerNoapte() << " RON\n";
        }
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

    friend std::istream& operator>>(std::istream& in, CityBreakPlaja& pachet);
};

std::istream& operator>>(std::istream& in, CityBreakPlaja& pachet) {
    std::cout << "Introdu destinatia: ";
    in.ignore();
    if (!std::getline(in, pachet.destinatie)) {
        in.clear();
        return in;
    }

    std::cout << "Introdu durata (zile): ";
    if (!(in >> pachet.durataZile)) {
        in.clear();
        return in;
    }
    in.ignore();

    std::cout << "Introdu orasul: ";
    if (!std::getline(in, pachet.oras)) {
        in.clear();
        return in;
    }

    std::cout << "Introdu data de incepere: ";
    if (!std::getline(in, pachet.dataIncepere)) {
        in.clear();
        return in;
    }

    std::cout << "Introdu pretul: ";
    if (!(in >> pachet.pret)) {
        in.clear();
        return in;
    }
    in.ignore();

    std::cout << "Introdu numarul de bilete disponibile: ";
    if (!(in >> pachet.bileteDisponibile)) {
        in.clear();
        return in;
    }
    in.ignore();

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
        : Vacanta("Destinatie necunoscuta", "01-01-2025", 7, 1000, "Necunoscut"),
          bileteDisponibile(0) {}

    CityBreakMunte(const CityBreakMunte& other)
    : Vacanta(other), bileteDisponibile(other.bileteDisponibile) {}

    bool esteOrasul(std::string orasCautat) const override {
        return oras == orasCautat;
    }

    int getBileteDisponibile() const {
        return bileteDisponibile;
    }

    void print(std::ostream& os) const override {
        os << "City Break la Munte\n";
        os << "Destinatie: " << destinatie << "\n";
        os << "Data Incepere: " << dataIncepere << "\n";
        os << "Durata: " << durataZile << " zile\n";
        os << "Pret: " << pret << " RON\n";
        os << "Bilete disponibile: " << bileteDisponibile << "\n";
        if (cazare) {
            os << "Cazare: " << cazare->getNume()
               << " (" << cazare->getStele() << " stele)"
               << "\nPret cazare per noapte: " << cazare->getPretPerNoapte() << " RON\n";
        }
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
    in.ignore();
    if (!std::getline(in, pachet.destinatie)) {
        in.clear();
        return in;
    }

    std::cout << "Introdu durata (zile): ";
    if (!(in >> pachet.durataZile)) {
        in.clear();
        return in;
    }
    in.ignore();

    std::cout << "Introdu orasul: ";
    if (!std::getline(in, pachet.oras)) {
        in.clear();
        return in;
    }

    std::cout << "Introdu data de incepere: ";
    if (!std::getline(in, pachet.dataIncepere)) {
        in.clear();
        return in;
    }

    std::cout << "Introdu pretul: ";
    if (!(in >> pachet.pret)) {
        in.clear();
        return in;
    }
    in.ignore();

    std::cout << "Introdu numarul de bilete disponibile: ";
    if (!(in >> pachet.bileteDisponibile)) {
        in.clear();
        return in;
    }
    in.ignore();

    return in;
}



class Croaziera : public Vacanta {
private:
    int bileteDisponibile;

public:
    Croaziera(std::string destinatie, int durata, std::string oras,
              std::string dataPlecare, double pret, int bilete)
        : Vacanta(destinatie, dataPlecare, durata, pret, oras),
          bileteDisponibile(bilete) {}

    Croaziera()
        : Vacanta("Destinatie necunoscuta", "01-01-2025", 7, 1000, "Necunoscut"),
          bileteDisponibile(0) {}

    Croaziera(const Croaziera& other)
        : Vacanta(other), bileteDisponibile(other.bileteDisponibile) {}

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
        os << "Durata: " << durataZile << " zile\n";
        os << "Oras imbarcare: " << oras << "\n";
        os << "Pret: " << pret << " RON\n";
        os << "Bilete disponibile: " << bileteDisponibile << "\n";
        if (cazare) {
            os << "Cazare: " << cazare->getNume()
               << " (" << cazare->getStele() << " stele)"
               << "\nPret cazare per noapte: " << cazare->getPretPerNoapte() << " RON\n";
        }
    }
    friend std::istream& operator>>(std::istream& in, Croaziera& pachet);
};

std::istream& operator>>(std::istream& in, Croaziera& pachet) {
    std::cout << "Introdu destinatia: ";
    in.ignore();
    if (!std::getline(in, pachet.destinatie)) {
        in.clear();
        return in;
    }

    std::cout << "Introdu durata (zile): ";
    if (!(in >> pachet.durataZile)) {
        in.clear();
        return in;
    }
    in.ignore();

    std::cout << "Introdu orasul de imbarcare: ";
    if (!std::getline(in, pachet.oras)) {
        in.clear();
        return in;
    }

    std::cout << "Introdu data de plecare: ";
    if (!std::getline(in, pachet.dataIncepere)) {
        in.clear();
        return in;
    }

    std::cout << "Introdu pretul: ";
    if (!(in >> pachet.pret)) {
        in.clear();
        return in;
    }
    in.ignore();

    std::cout << "Introdu numarul de bilete disponibile: ";
    if (!(in >> pachet.bileteDisponibile)) {
        in.clear();
        return in;
    }
    in.ignore();

    return in;
}


class PachetFactory {
public:
    static std::shared_ptr<Vacanta> creeazaPachet(
        TipVacanta tip,
        const std::string& destinatie,
        const std::string& dataIncepere,
        int durataZile,
        double pret,
        const std::string& oras,
        int bilete = 0) {

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
};

class AgentieVacante {
private:
    std::vector<std::shared_ptr<Vacanta>> pachete;
    GestiuneCazari gestiuneCazari;
    int tip;
public:
    GestiuneCazari& getGestiuneCazari() { return gestiuneCazari; }

    void listeazaPachete();
    void afiseazaPacheteDupaTip(const std::string& tip) const;
    void cautaDupaOras(std::string orasCautat) const;
    void adaugaPachet(TipVacanta tipPachet, const std::string &destinatie, const std::string &dataIncepere, int durataZile, double pret, const std::string &oras, int bilete = 0);
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

    auto manager = AutentificareManager::getInstance();
    auto utilizatorCurent = manager->getUtilizatorCurent();

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


class Transport {
protected:
    std::list<bool> locuri;
    int numarLocuri;
    std::string nume;
    std::vector<int> rezervariActive;

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

    const std::string& getNume() const {
        return nume;
    }

    void rezervaLoc(int loc) {
        if (loc < 1 || loc > numarLocuri) {
            std::cout << "Loc invalid! Introdu un loc între 1 și " << numarLocuri << ".\n";
            return;
        }

        int index = 1;
        for (auto& loc_curent : locuri) {
            if (index == loc) {
                if (loc_curent) {
                    std::cout << "Locul " << loc << " este deja ocupat!\n";
                } else {
                    loc_curent = true;
                    rezervariActive.push_back(loc);
                    std::cout << "Locul " << loc << " a fost rezervat!\n";
                }
                return;
            }
            index++;
        }
    }

    void anuleazaRezervare(int loc) {
        if (loc < 1 || loc > numarLocuri) {
            std::cout << "Loc invalid! Introdu un loc între 1 și " << numarLocuri << ".\n";
            return;
        }

        auto rezervare = std::find(rezervariActive.begin(), rezervariActive.end(), loc);
        if (rezervare == rezervariActive.end()) {
            std::cout << "Locul " << loc << " nu era rezervat de tine!\n";
            return;
        }

        int index = 1;
        for (auto& loc_curent : locuri) {
            if (index == loc) {
                loc_curent = false;
                break;
            }
            index++;
        }

        rezervariActive.erase(rezervare);
        std::cout << "Rezervarea locului " << loc << " a fost anulata cu succes!\n";
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



class Statistici {
private:
    std::map<std::string, int> rezervariPerDestinatie;
    int totalRezervari;
    double totalVanzari;

    Statistici() : totalRezervari(0), totalVanzari(0.0) {}

public:
    static Statistici& getInstance() {
        static Statistici instance;
        return instance;
    }

    void adaugaRezervare(const std::string& destinatie, double pret) {
        rezervariPerDestinatie[destinatie]++;
        totalRezervari++;
        totalVanzari += pret;
    }

    void afiseazaStatistici() const {
        std::cout << "\n=== Statistici ===\n";
        std::cout << "Total rezervari: " << totalRezervari << "\n";
        std::cout << "Total vanzari: " << totalVanzari << " RON\n";
        std::cout << "\nRezervari per destinatie:\n";
        
        auto maxRezervari = std::max_element(
            rezervariPerDestinatie.begin(), 
            rezervariPerDestinatie.end(),
            [](const auto& p1, const auto& p2) {
                return p1.second < p2.second;
            }
        );

        for (const auto& [destinatie, numar] : rezervariPerDestinatie) {
            std::cout << destinatie << ": " << numar;
            if (!rezervariPerDestinatie.empty() && maxRezervari->first == destinatie) {
                std::cout << " (Cea mai populara destinatie!)";
            }
            std::cout << "\n";
        }
    }

    Statistici(const Statistici&) = delete;
    Statistici& operator=(const Statistici&) = delete;
};


class Meniu {
public:
    void executa(AgentieVacante& agentie);
    Meniu() : transportCurent(nullptr), utilizator(nullptr) {}
private:
    std::shared_ptr<Utilizator> utilizator;
    std::vector<std::shared_ptr<Vacanta>> pachete;
    std::shared_ptr<Transport> transportCurent;
    std::map<std::string, std::shared_ptr<Transport>> transporturiRezervate;

    void adaugaPachet(AgentieVacante& agentie);
    void cautaPachete(AgentieVacante& agentie);
    void selecteazaTransport(int nrBilete, const std::string& tipVacanta);

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
            vacantaSelectata->getDestinatie(), 1, bugetMaxim);
        
        if (cazariDisponibile.empty()) {
            std::cout << "Nu exista cazari disponibile in limita bugetului specificat.\n";
        } else {
            for (const auto& cazare : cazariDisponibile) {
                std::cout << cazare->getNume() << " - " << cazare->getPretPerNoapte() 
                         << " RON/noapte (" << cazare->getStele() << " stele)\n";
            }
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
                        std::cout << "Doriti mic dejun? (d/n): ";
                        std::cin >> optiune;
                        cazareSelectata->setMicDejun(optiune == 'd' || optiune == 'D');

                        std::cout << "Doriti demipensiune? (d/n): ";
                        std::cin >> optiune;
                        cazareSelectata->setDemipensiune(optiune == 'd' || optiune == 'D');

                        std::cout << "Doriti all inclusive? (d/n): ";
                        std::cin >> optiune;
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

    void anuleazaLoc() {
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

            // If no more active reservations, remove from transporturiRezervate
            if (transportCurent->getRezervariActive().empty()) {
                transporturiRezervate.erase(utilizator->getUsername());
                transportCurent = nullptr;
            }
        } else {
            std::cout << "Nu ai nicio rezervare activă pentru niciun mijloc de transport!\n";
        }
    }
};

void Meniu::executa(AgentieVacante& agentie) {
    int tipUtilizator;
    std::string nume;
    Autobuz autobuz(20, nume, 120);
    auto manager = AutentificareManager::getInstance();

    while (true) {
        manager->login();

        if (!manager->esteAutentificat()) {
            std::cout << "Autentificare eșuată. Ieșire din program.\n";
            return;
        }

        std::shared_ptr<Utilizator> utilizatorCurent = manager->getUtilizatorCurent();
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
                switch (optiune) {
                    case 1: agentie.listeazaPachete();
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
                    case 0: manager->logout();
                    break;
                    default: std::cout << "Optiune invalida.\n";
                }
            } else if (rol == "organizator") {
                switch (optiune) {
                    case 1: adaugaPachet(agentie); break;
                    case 2: agentie.listeazaPachete(); break;
                    case 0: break;
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

            std::cout << "Cate persoane sunt în grup? ";
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

            std::cout << "Cate persoane sunt în grup? ";
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
}

class RezervareInvalidaException : public std::exception {
private:
    std::string mesaj;
public:
    RezervareInvalidaException(const std::string& msg) : mesaj(msg) {}
    
    [[nodiscard]] const char* what() const noexcept override {
        return mesaj.c_str();
    }
};

int main() {
    AgentieVacante agentie;

    auto hotelAtena = std::make_shared<Hotel>("Hotel Olympic", 4, "Atena", 500.0, 50);
    auto hotelLux = std::make_shared<Hotel>("Hotel Lux", 5, "Paris", 1000.0, 30);
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
    
    auto vilaVenetia = std::make_shared<Vila>("Villa Venezia", "Venetia", 1200.0, 5, 10);
    vilaVenetia->setBarbeque(true);
    vilaVenetia->adaugaFacilitate("Gradina privata");
    vilaVenetia->adaugaFacilitate("Vedere la canal");
    agentie.getGestiuneCazari().adaugaCazare("Italia", vilaVenetia);

    agentie.adaugaPachet(TipVacanta::CityBreakPlaja, "Grecia", "2025-06-10", 7, 2500.0, "Atena", 15);
    agentie.adaugaPachet(TipVacanta::CityBreakMunte, "Innsbruck", "2025-07-01", 5, 1800.0, "Innsbruck", 13);
    agentie.adaugaPachet(TipVacanta::CityBreakPlaja, "Italia", "10-11-2025", 10, 4000.0, "Venetia", 20);
    agentie.adaugaPachet(TipVacanta::Croaziera, "Franta", "12-08-2025", 3, 1000.0, "Cannes", 10);

    Meniu meniu;
    meniu.executa(agentie);
    return 0;
}
