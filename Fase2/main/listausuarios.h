// ListaUsuarios.h
#ifndef LISTAUSUARIOS_H
#define LISTAUSUARIOS_H

#include "NodoAVL.h"
#include <string>
#include "usuario.h"
#include <iostream>
#include <fstream>
#include "json.hpp"
#include <vector>

class ListaUsuarios {
private:

    NodoAVL* raiz;
    NodoAVL* balancear(NodoAVL* nodo);

    int altura(NodoAVL* nodo) const;
    int balance(NodoAVL* nodo) const;
    NodoAVL* rotarDerecha(NodoAVL* y);
    NodoAVL* rotarIzquierda(NodoAVL* x);
    NodoAVL* insertar(NodoAVL* nodo, const Usuario& usuario);
    NodoAVL* buscar(NodoAVL* nodo, const std::string& correo) const;
    NodoAVL* eliminar(NodoAVL* nodo, const std::string& correo);
    NodoAVL* minimoNodo(NodoAVL* nodo);
    void eliminarArbol(NodoAVL* nodo);
    int obtenerAltura(NodoAVL* nodo);
    int obtenerBalance(NodoAVL* nodo);
    void generateDotRec(NodoAVL* nodo, std::ofstream& file) const;

public:
    ListaUsuarios();
    ~ListaUsuarios();

    void agregarUsuario(const Usuario& usuario);
    Usuario* buscarUsuarioPorCorreo(const std::string& correo);
    void borrarUsuarioPorCorreo(const std::string& correo);
    void cargarUsuariosDesdeJson(const std::string& nombreArchivo);
    bool buscarUsuarioPorCorreoyContrasena(const std::string& correo, const std::string& contrasena) const;
    bool usuarioDuplicado(const std::string &correo) const;
    Usuario mostrarDatosPorCorreo(const std::string& correo) const;
    void preOrder(NodoAVL* nodo, std::vector<Usuario>& usuarios) const;
    void inOrder(NodoAVL* nodo, std::vector<Usuario>& usuarios) const;
    void postOrder(NodoAVL* nodo, std::vector<Usuario>& usuarios) const;
    std::vector<Usuario> obtenerUsuariosEnOrden(const std::string& tipoOrden) const;
    void generateDot(const std::string& filename) const;

};

#endif // LISTAUSUARIOS_H
