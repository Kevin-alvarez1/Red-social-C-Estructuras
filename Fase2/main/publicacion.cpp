#include "publicacion.h"

// Constructor
Publicacion::Publicacion(int id, const std::string& correo, const std::string& contenido, const std::string& fecha, const std::string& hora)
    : id_(id), correo_(correo), contenido_(contenido), fecha_(fecha), hora_(hora) {}

// Destructor
Publicacion::~Publicacion() {
    // No es necesario liberar memoria aquí, ya que arbolComentarios_ es global
}

// Obtener los valores de los atributos
int Publicacion::getId() const { return id_; }
std::string Publicacion::getCorreo() const { return correo_; }
std::string Publicacion::getContenido() const { return contenido_; }
std::string Publicacion::getFecha() const { return fecha_; }
std::string Publicacion::getHora() const { return hora_; }

// Modificar el contenido
void Publicacion::setContenido(const std::string& nuevoContenido) {
    contenido_ = nuevoContenido;
}

// Agregar un comentario al árbol de comentarios global
void Publicacion::agregarComentario(const Comentario& comentario) {
    arbolComentarios_.insertar(comentario);
}

ArbolBComentario& Publicacion::getArbolComentarios() {
    return arbolComentarios_;
}

// Limpiar todos los comentarios de la publicación
void Publicacion::limpiarComentarios() {
}
