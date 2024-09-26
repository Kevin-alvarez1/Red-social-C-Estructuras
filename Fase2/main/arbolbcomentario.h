#ifndef ARBOL_B_COMENTARIO_H
#define ARBOL_B_COMENTARIO_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "comentario.h"
#include <map>
// Definir el orden del árbol B
#define ORDEN 5

// Clase que define un nodo del Árbol B para los comentarios
class NodoArbolBComentario {
public:
    bool esHoja;
    std::vector<Comentario> comentarios;
    std::vector<NodoArbolBComentario*> hijos;

    NodoArbolBComentario(bool hoja);

    void obtenerComentariosPorPublicacion(int idPublicacion, std::vector<Comentario>& comentarios) const;
    void insertarNoLleno(const Comentario& comentario);
    void dividirHijo(int indice, NodoArbolBComentario* nodoLleno);
    void recorrer() const;
    NodoArbolBComentario* buscar(const std::string& fecha, const std::string& hora)const;
    static bool compararComentarios(const Comentario& c1, const Comentario& c2);
};

// Clase que define el Árbol B de comentarios
class ArbolBComentario {
public:
    ArbolBComentario();

    void insertar(const Comentario& comentario);
    std::vector<Comentario> getComentariosDePublicacion(int idPublicacion) const;
    void mostrarComentarios() const;
    NodoArbolBComentario* buscar(const std::string& fecha, const std::string& hora)const;
    void graficarArbolB(const std::string& nombreArchivo, int idPublicacion);
    void graficarNodo(NodoArbolBComentario* nodo, int& nodeCount, std::map<NodoArbolBComentario*, int>& nodeMap, std::ofstream& archivo, int idPublicacion);

private:
    NodoArbolBComentario* raiz;
    void obtenerComentariosRecursivos(NodoArbolBComentario* nodo, int idPublicacion, std::vector<Comentario>& comentarios) const;

};

#endif // ARBOL_B_COMENTARIO_H
