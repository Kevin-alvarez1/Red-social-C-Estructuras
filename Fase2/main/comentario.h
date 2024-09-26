#ifndef COMENTARIO_H
#define COMENTARIO_H

#include <string>

class Comentario {
public:
    // Constructor
    Comentario() : contenido_(""), idPublicacion_(0), correo_(""), fecha_(""), hora_("") {}

    Comentario(const std::string& contenido, int idPublicacion, const std::string& correo, const std::string& fecha, const std::string& hora);

    // Getters
    std::string getCorreo() const;
    std::string getComentario() const;
    std::string getFecha() const;
    std::string getHora() const;
    int getIdPublicacion() const;
    bool esVacio() const {
        return contenido_.empty();
    }

private:
    std::string contenido_;
    std::string correo_;
    int idPublicacion_;
    std::string fecha_;
    std::string hora_;
};

#endif // COMENTARIO_H
