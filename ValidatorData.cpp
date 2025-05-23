#include "ValidatorData.h"
#include "Exceptii.h"
#include <stdexcept>

void ValidatorData::valideazaData(const std::string& data) {
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
