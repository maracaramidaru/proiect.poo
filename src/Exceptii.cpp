#include "Exceptii.h"

const char* BileteEpuizateException::what() const noexcept {
    return "Nu mai sunt bilete disponibile pentru aceasta vacanta!";
}

DataInvalidaException::DataInvalidaException(const std::string& msg) : mesaj(msg) {}

const char* DataInvalidaException::what() const noexcept {
    return mesaj.c_str();
}
