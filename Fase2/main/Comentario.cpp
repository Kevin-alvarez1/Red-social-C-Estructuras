#include "comentario.h"

// Constructor con parámetros
Comentario::Comentario(const std::string& contenido, int idPublicacion, const std::string& correo, const std::string& fecha, const std::string& hora)
    : contenido_(contenido), idPublicacion_(idPublicacion), correo_(correo), fecha_(fecha), hora_(hora) {}

// Getters
std::string Comentario::getCorreo() const {
    return correo_;
}

std::string Comentario::getComentario() const {
    return contenido_;
}

std::string Comentario::getFecha() const {
    return fecha_;
}

std::string Comentario::getHora() const {
    return hora_;
}

int Comentario::getIdPublicacion() const {
    return idPublicacion_;
}
