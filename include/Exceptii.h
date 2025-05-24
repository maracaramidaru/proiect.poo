#ifndef EXCEPTII_H
#define EXCEPTII_H

#include <exception>
#include <string>

class BileteEpuizateException : public std::exception {
public:
    const char* what() const noexcept override;
};

class DataInvalidaException : public std::exception {
private:
    std::string mesaj;
public:
    explicit DataInvalidaException(const std::string& msg);
    const char* what() const noexcept override;
};

class RezervareInvalidaException : public std::exception {
private:
    std::string mesaj;
public:
    explicit RezervareInvalidaException(const std::string& msg);
    const char* what() const noexcept override;
};

#endif