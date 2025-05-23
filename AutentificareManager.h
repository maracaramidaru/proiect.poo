
#ifndef AUTENTIFICATORMANAGER_H
#define AUTENTIFICATORMANAGER_H

#include <memory>
#include <iostream>
#include <string>
#include <limits>

#include "Utilizator.h"
#include "Vizitator.h"
#include "Organizator.h"

class AutentificareManager {
private:
    std::shared_ptr<Utilizator> utilizatorCurent;
    AutentificareManager();

public:
    static std::shared_ptr<AutentificareManager> getInstance();

    void login();

    std::shared_ptr<Utilizator> getUtilizatorCurent() const;

    bool esteAutentificat() const;

    void logout();
};

#endif //AUTENTIFICATORMANAGER_H
