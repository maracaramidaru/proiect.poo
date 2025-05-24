#include "Utilizator.h"

Utilizator::Utilizator() {}

Utilizator::Utilizator(const std::string& u, const std::string& p, const std::string& r)
    : rol(r), username(u), parola(p) {}


Utilizator::~Utilizator() {}

void Utilizator::scadePuncte(int nrPuncte) {
    puncte -= nrPuncte;
}

void Utilizator::adaugaPuncte(int nrPuncte) {
    puncte += nrPuncte;
    std::cout << "Ai acumulat " << nrPuncte << " puncte! Total puncte: " << puncte << "\n";

    if (puncte >= 100 && !cadouri.empty()) {
        acordaCadou();
        scadePuncte(nrPuncte);
    }
}

void Utilizator::acordaCadou() {
    if (cadouri.empty()) {
        std::cout << "Nu mai sunt cadouri disponibile!\n";
        return;
    }

    int indexAleatoriu = rand() % cadouri.size();

    std::cout << "Felicitari! Ai primit: " << cadouri[indexAleatoriu] << "\n";

    cadouri.erase(cadouri.begin() + indexAleatoriu);
}

std::istream& operator>>(std::istream& in, Utilizator& utilizator) {
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

const std::string& Utilizator::getRol() const {
    return rol;
}

const std::string& Utilizator::getUsername() const {
    return username;
}

bool Utilizator::autentificare(const std::string& user, const std::string& pass) const {
    return username == user && parola == pass;
}

void Utilizator::adaugaBilet(const std::shared_ptr<Vacanta>& bilet) {
    bileteRezervate.push_back(bilet);
}

// void Utilizator::setParola(const std::string& newParola) {
//     parola = newParola;
// }
void Utilizator::setUsername(const std::string& newUsername) {
    username = newUsername;
}

void Utilizator::afiseazaBilete() const {
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
