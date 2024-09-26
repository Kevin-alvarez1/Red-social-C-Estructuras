#include "usuario.h"
#include <iostream>
// Constructor
Usuario::Usuario(const std::string &nombre, const std::string &apellido, const std::string &fecha_de_nacimiento,
                 const std::string &correo, const std::string &contrasena)
    : nombre_(nombre), apellido_(apellido), fecha_de_nacimiento_(fecha_de_nacimiento), correo_(correo), contrasena_(contrasena) {}

// Getters
std::string Usuario::getNombre() const
{
    return nombre_;
}

std::string Usuario::getApellido() const
{
    return apellido_;
}

std::string Usuario::getFechaDeNacimiento() const
{
    return fecha_de_nacimiento_;
}

std::string Usuario::getCorreo() const
{
    return correo_;
}

std::string Usuario::getContrasena() const
{
    return contrasena_;
}

// Setters
void Usuario::setNombre(const std::string &nombre)
{
    nombre_ = nombre;
}

void Usuario::setApellido(const std::string &apellido)
{
    apellido_ = apellido;
}

void Usuario::setFechaDeNacimiento(const std::string &fecha_de_nacimiento)
{
    fecha_de_nacimiento_ = fecha_de_nacimiento;
}

void Usuario::setCorreo(const std::string &correo)
{
    correo_ = correo;
}

void Usuario::setContrasena(const std::string &contrasena)
{
    contrasena_ = contrasena;
}

void Usuario::mostrarInformacion() const
{
    std::cout << "Nombre: " << nombre_ << std::endl;
    std::cout << "Apellido: " << apellido_ << std::endl;
    std::cout << "Fecha de Nacimiento: " << fecha_de_nacimiento_ << std::endl;
    std::cout << "Correo: " << correo_ << std::endl;
}
