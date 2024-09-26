#ifndef ARBOLABB_H
#define ARBOLABB_H

#include <string>
#include <list>
#include "Publicacion.h"
#include <vector>
#include <sstream>
#include <iomanip>

class NodoABB {
public:
    std::string fecha;
    std::vector<Publicacion> publicaciones;
    NodoABB* izquierda;
    NodoABB* derecha;
    int altura;
    int id;
    NodoABB(int id_, const std::string& fecha_);
};

class ArbolABB {
public:
    ArbolABB();
    ~ArbolABB();

    void eliminarPublicacion(int id);
    void insertarPublicacion(const Publicacion& publicacion);
    void mostrarPublicaciones(const std::string& fecha) const;
    void mostrarPublicacionesCronologicas() const;
    std::vector<Publicacion> obtenerPublicacionesEnOrden(const std::string& tipoOrden) const;
    void generateDotFile(const std::string& filename, const std::string& fechaBuscada) const;
    void generateDot(NodoABB* nodo, std::ofstream& file, const std::string& fechaBuscada) const;
    void graficar(const std::string& archivoImagen) const;
    void preOrder(NodoABB* nodo, std::vector<Publicacion>& publicaciones) const;
    void inOrder(NodoABB* nodo, std::vector<Publicacion>& publicaciones) const;
    void postOrder(NodoABB* nodo, std::vector<Publicacion>& publicaciones) const;
    std::string inOrder(NodoABB* nodo) const;
    void mostrarPublicacionesCronologicas(const std::string& orden) const;
    void recorrerPreOrder(std::vector<Publicacion>& publicaciones) const;
    void recorrerInOrder(std::vector<Publicacion>& publicaciones) const;
    void recorrerPostOrder(std::vector<Publicacion>& publicaciones) const;
    NodoABB* getRaiz() const {
        return raiz;
    }

private:
    NodoABB* raiz;
    NodoABB* insertarNodo(NodoABB* nodo, const Publicacion& publicacion);
    NodoABB* buscarNodo(NodoABB* nodo, const std::string& fecha) const;
    void destruirArbol(NodoABB* nodo);
    int obtenerAltura(NodoABB* nodo);
    int obtenerBalance(NodoABB* nodo);
    void actualizarAltura(NodoABB* nodo);
    NodoABB* rotarDerecha(NodoABB* y);
    NodoABB* rotarIzquierda(NodoABB* x);
    NodoABB* eliminarNodo(NodoABB* nodo, int id);
    NodoABB* buscarMinimo(NodoABB* nodo) const;
    std::string convertirFecha(const std::string& fechaStr) const;
    void generateDot(NodoABB* nodo, std::ofstream& file) const;
    void recorrerPreOrder(NodoABB* nodo, std::vector<Publicacion>& publicaciones) const;
    void recorrerInOrder(NodoABB* nodo, std::vector<Publicacion>& publicaciones) const;
    void recorrerPostOrder(NodoABB* nodo, std::vector<Publicacion>& publicaciones) const;
};

#endif // ARBOLABB_H
