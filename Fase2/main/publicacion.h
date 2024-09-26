#ifndef PUBLICACION_H
#define PUBLICACION_H

#include <string>
#include "Comentario.h"
#include "arbolbcomentario.h"
extern ArbolBComentario arbolComentarios_;

class Publicacion {
public:
    // Constructor
    Publicacion(int id, const std::string& correo, const std::string& contenido, const std::string& fecha, const std::string& hora);

    // Destructor
    ~Publicacion();

    // Métodos de acceso a los atributos
    int getId() const;
    std::string getCorreo() const;
    std::string getContenido() const;
    std::string getFecha() const;
    std::string getHora() const;
    ArbolBComentario& getArbolComentarios();

    // Modificar el contenido
    void setContenido(const std::string& nuevoContenido);

    // Métodos relacionados con los comentarios
    void agregarComentario(const Comentario& comentario);
    void mostrarComentarios() const; // Método para mostrar comentarios
    void limpiarComentarios();

private:
    int id_;
    std::string correo_;
    std::string contenido_;
    std::string fecha_;
    std::string hora_;
    ArbolBComentario arbolComentarios_; // Miembro de la clase
};

#endif
