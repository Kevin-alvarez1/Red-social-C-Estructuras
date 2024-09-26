#include "solicitud.h"

Solicitud::Solicitud(const std::string &emisor, const std::string &receptor, const std::string &estado)
    : emisor_(emisor), receptor_(receptor), estado_(estado)
{
    std::cout << "Depuración: Solicitud creada con emisor: " << emisor_
              << ", receptor: " << receptor_
              << ", estado: " << estado_ << std::endl;
}

std::string Solicitud::getEmisor() const {
    return emisor_;
}

void Solicitud::setEmisor(const std::string &emisor) {
    std::cout << "Depuración: Cambiando emisor de " << emisor_ << " a " << emisor << std::endl;
    emisor_ = emisor;
}

std::string Solicitud::getReceptor() const {
    return receptor_;
}

void Solicitud::setReceptor(const std::string &receptor) {
    std::cout << "Depuración: Cambiando receptor de " << receptor_ << " a " << receptor << std::endl;
    receptor_ = receptor;
}

std::string Solicitud::getEstado() const {
    return estado_;
}

void Solicitud::setEstado(const std::string &estado) {
    std::cout << "Depuración: Cambiando estado de " << estado_ << " a " << estado << std::endl;
    estado_ = estado;
}
