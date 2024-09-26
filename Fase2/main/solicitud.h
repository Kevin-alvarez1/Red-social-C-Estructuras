#ifndef SOLICITUD_H
#define SOLICITUD_H

#include <string>
#include <iostream>

class Solicitud {
public:
    Solicitud(const std::string &emisor, const std::string &receptor, const std::string &estado);

    std::string getEmisor() const;
    void setEmisor(const std::string &emisor);

    std::string getReceptor() const;
    void setReceptor(const std::string &receptor);

    std::string getEstado() const;
    void setEstado(const std::string &estado);

private:
    std::string emisor_;
    std::string receptor_;
    std::string estado_;
};

#endif // SOLICITUD_H
