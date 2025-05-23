
#ifndef AUTENTIFICATORMANAGER_H
#define AUTENTIFICATORMANAGER_H

#include <memory>
#include <iostream>
#include <string>
#include <limits>
#include "Utilizator.h"

class AutentificareManager {
private:
    std::shared_ptr<Utilizator> utilizatorCurent;

public:
    AutentificareManager() = default;

    void login();

    std::shared_ptr<Utilizator> getUtilizatorCurent() const;

    bool esteAutentificat() const;

    void logout();
};

#endif //AUTENTIFICATORMANAGER_H
