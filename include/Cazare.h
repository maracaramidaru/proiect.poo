#ifndef CAZARE_H
#define CAZARE_H

#include <string>
#include <vector>

class Cazare {
public:
    enum class TipCazare {
        hotel,
        pensiune,
        apartament,
        vila,
        resort
    };

protected:
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
           double pretPerNoapte, int camereDisponibile);

    virtual ~Cazare() = default;

    virtual double calculeazaPret(int nrNopti, int nrPersoane);

    bool verificaDisponibilitate(int nrCamere) const;

    void adaugaFacilitate(const std::string& facilitate);

    const std::vector<std::string>& getFacilitati() const;

    const std::string& getNume() const;
    double getPretPerNoapte() const;
    int getStele() const;

    void setMicDejun(bool include);
    void setDemipensiune(bool include);
    void setAllInclusive(bool include);
};

#endif // CAZARE_H
