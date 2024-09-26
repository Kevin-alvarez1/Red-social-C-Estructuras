#include "receptor.h"

Receptor::Receptor(const std::string& emisor_, const std::string& receptor_, const std::string& estado_)
    : emisor(emisor_), receptor(receptor_), estado(estado_) {}

// Getters
std::string Receptor::getEmisor() const {
    return emisor;
}

std::string Receptor::getReceptor() const {
    return receptor;
}

std::string Receptor::getEstado() const {
    return estado;
}

// Setters
void Receptor::setEmisor(const std::string& emisor_) {
    emisor = emisor_;
}

void Receptor::setReceptor(const std::string& receptor_) {
    receptor = receptor_;
}

void Receptor::setEstado(const std::string& estado_) {
    estado = estado_;
}
