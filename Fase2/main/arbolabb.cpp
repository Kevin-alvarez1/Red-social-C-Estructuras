#include "arbolabb.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <cstdlib>

ArbolABB arbolABB;
// Constructor
ArbolABB::ArbolABB() : raiz(nullptr) {}

// Destructor
ArbolABB::~ArbolABB() {
}

// En arbolabb.cpp
NodoABB::NodoABB(int id_, const std::string& fecha_)
    : id(id_), fecha(fecha_), izquierda(nullptr), derecha(nullptr), altura(1) {} // Inicializar altura a 1

// Insertar una publicación en el ABB
void ArbolABB::insertarPublicacion(const Publicacion& publicacion) {
    raiz = insertarNodo(raiz, publicacion);
}

NodoABB* ArbolABB::insertarNodo(NodoABB* nodo, const Publicacion& publicacion) {
    std::string fechaConvertida = convertirFecha(publicacion.getFecha());

    if (!nodo) {
        int nuevoId = publicacion.getId();
        NodoABB* nuevoNodo = new NodoABB(nuevoId, fechaConvertida);
        nuevoNodo->publicaciones.push_back(publicacion);
        return nuevoNodo;
    }

    if (fechaConvertida < nodo->fecha) {
        nodo->izquierda = insertarNodo(nodo->izquierda, publicacion);
    } else if (fechaConvertida > nodo->fecha) {
        nodo->derecha = insertarNodo(nodo->derecha, publicacion);
    } else {
        nodo->publicaciones.push_back(publicacion);
    }

    // Actualizar la altura del nodo
    actualizarAltura(nodo);

    // Obtener el factor de equilibrio
    int balance = obtenerBalance(nodo);

    // Rotaciones para mantener el equilibrio
    if (balance > 1 && fechaConvertida < nodo->izquierda->fecha) {
        return rotarDerecha(nodo);
    }
    if (balance < -1 && fechaConvertida > nodo->derecha->fecha) {
        return rotarIzquierda(nodo);
    }
    if (balance > 1 && fechaConvertida > nodo->izquierda->fecha) {
        nodo->izquierda = rotarIzquierda(nodo->izquierda);
        return rotarDerecha(nodo);
    }
    if (balance < -1 && fechaConvertida < nodo->derecha->fecha) {
        nodo->derecha = rotarDerecha(nodo->derecha);
        return rotarIzquierda(nodo);
    }

    return nodo;
}

int ArbolABB::obtenerAltura(NodoABB* nodo) {
    return nodo ? nodo->altura : 0;
}

int ArbolABB::obtenerBalance(NodoABB* nodo) {
    return nodo ? obtenerAltura(nodo->izquierda) - obtenerAltura(nodo->derecha) : 0;
}

void ArbolABB::actualizarAltura(NodoABB* nodo) {
    nodo->altura = 1 + std::max(obtenerAltura(nodo->izquierda), obtenerAltura(nodo->derecha));
}

NodoABB* ArbolABB::rotarDerecha(NodoABB* y) {
    NodoABB* x = y->izquierda;
    NodoABB* T2 = x->derecha;

    // Realizar la rotación
    x->derecha = y;
    y->izquierda = T2;

    // Actualizar alturas
    actualizarAltura(y);
    actualizarAltura(x);

    return x; // Nueva raíz
}

bool NodoArbolBComentario::compararComentarios(const Comentario& c1, const Comentario& c2) {
    if (c1.getFecha() < c2.getFecha()) {
        return true;
    } else if (c1.getFecha() == c2.getFecha()) {
        return c1.getHora() < c2.getHora();
    }
    return false;
}

NodoABB* ArbolABB::rotarIzquierda(NodoABB* x) {
    NodoABB* y = x->derecha;
    NodoABB* T2 = y->izquierda;

    // Realizar la rotación
    y->izquierda = x;
    x->derecha = T2;

    // Actualizar alturas
    actualizarAltura(x);
    actualizarAltura(y);

    return y; // Nueva raíz
}

// Buscar un nodo por fecha
NodoABB* ArbolABB::buscarNodo(NodoABB* nodo, const std::string& fecha) const {
    std::string fechaConvertida = convertirFecha(fecha);

    if (!nodo || nodo->fecha == fechaConvertida) {
        return nodo;
    }

    if (fechaConvertida < nodo->fecha) {
        return buscarNodo(nodo->izquierda, fecha);
    } else {
        return buscarNodo(nodo->derecha, fecha);
    }
}

void ArbolABB::recorrerPreOrder(NodoABB* nodo, std::vector<Publicacion>& publicaciones) const {
    if (nodo) {
        publicaciones.insert(publicaciones.end(), nodo->publicaciones.begin(), nodo->publicaciones.end());
        recorrerPreOrder(nodo->izquierda, publicaciones);
        recorrerPreOrder(nodo->derecha, publicaciones);
    }
}

void ArbolABB::recorrerInOrder(NodoABB* nodo, std::vector<Publicacion>& publicaciones) const {
    if (nodo) {
        recorrerInOrder(nodo->izquierda, publicaciones);
        publicaciones.insert(publicaciones.end(), nodo->publicaciones.begin(), nodo->publicaciones.end());
        recorrerInOrder(nodo->derecha, publicaciones);
    }
}

void ArbolABB::recorrerPostOrder(NodoABB* nodo, std::vector<Publicacion>& publicaciones) const {
    if (nodo) {
        recorrerPostOrder(nodo->izquierda, publicaciones);
        recorrerPostOrder(nodo->derecha, publicaciones);
        publicaciones.insert(publicaciones.end(), nodo->publicaciones.begin(), nodo->publicaciones.end());
    }
}


// Métodos públicos que llaman a los métodos privados
void ArbolABB::recorrerPreOrder(std::vector<Publicacion>& publicaciones) const {
    recorrerPreOrder(raiz, publicaciones);
}

void ArbolABB::recorrerInOrder(std::vector<Publicacion>& publicaciones) const {
    recorrerInOrder(raiz, publicaciones);
}

void ArbolABB::recorrerPostOrder(std::vector<Publicacion>& publicaciones) const {
    recorrerPostOrder(raiz, publicaciones);
}


void ArbolABB::generateDotFile(const std::string& filename, const std::string& fechaBuscada) const {
    std::ofstream file(filename + ".dot");
    if (!file.is_open()) {
        std::cerr << "No se pudo abrir el archivo DOT para escribir." << std::endl;
        return;
    }

    file << "digraph BSTTree {\n";
    file << "node [shape=circle];\n";

    generateDot(raiz, file, fechaBuscada);

    file << "}\n";
    file.close();

    // Convertir el archivo DOT a PNG usando el comando dot
    std::string comando = "dot -Tpng " + filename + ".dot -o " + filename + ".png";
    int resultado = std::system(comando.c_str());

    if (resultado != 0) {
        std::cerr << "Error al convertir el archivo DOT a PNG." << std::endl;
    }
}

void ArbolABB::generateDot(NodoABB* nodo, std::ofstream& file, const std::string& fechaBuscada) const {
    if (nodo) {
        // Conectar los nodos del árbol con sus hijos
        if (nodo->izquierda) {
            file << "\"" << nodo->fecha << "\" -> \"" << nodo->izquierda->fecha << "\";\n";
            generateDot(nodo->izquierda, file, fechaBuscada);
        }
        if (nodo->derecha) {
            file << "\"" << nodo->fecha << "\" -> \"" << nodo->derecha->fecha << "\";\n";

            generateDot(nodo->derecha, file, fechaBuscada);
        }

        // Verificar si la fecha del nodo coincide con la fecha buscada
        if (nodo->fecha == fechaBuscada) {
            file << "\"" << nodo->fecha << "\" [style=filled, fillcolor=green];\n";

            std::string previousNodeId = ""; // Mantendrá el ID del nodo de la publicación anterior

            // Iterar sobre todas las publicaciones del nodo y crear un nodo diferente para cada publicación
            for (const auto& publicacion : nodo->publicaciones) {
                std::string currentNodeId = "pub_" + std::to_string(publicacion.getId());  // Generar un ID único para el nodo de la publicación

                // Crear un nodo para la publicación con su información
                file << "\"" << currentNodeId << "\" [shape=rectangle, label=\"ID: " << publicacion.getId()
                     << "\\nFecha: " << publicacion.getFecha()
                     << "\\nHora: " << publicacion.getHora()
                     << "\\nCorreo: " << publicacion.getCorreo()
                     << "\\nContenido: " << publicacion.getContenido() << "\"];\n";

                // Si es la primera publicación, conectarla al nodo de la fecha
                if (previousNodeId.empty()) {
                    file << "\"" << nodo->fecha << "\" -> \"" << currentNodeId << "\";\n";
                } else {
                    // Conectar la publicación actual a la publicación anterior
                    file << "\"" << previousNodeId << "\" -> \"" << currentNodeId << "\";\n";
                }

                // Actualizar el nodo anterior para la próxima iteración
                previousNodeId = currentNodeId;
            }
        }
    }
}



void ArbolABB::eliminarPublicacion(int id) {
    raiz = eliminarNodo(raiz, id);
}

NodoABB* ArbolABB::eliminarNodo(NodoABB* nodo, int id) {
    if (!nodo) {
        return nodo; // El árbol está vacío o el nodo no se encontró
    }

    // Buscar el nodo que se quiere eliminar por ID
    if (id < nodo->id) {
        nodo->izquierda = eliminarNodo(nodo->izquierda, id);
    } else if (id > nodo->id) {
        nodo->derecha = eliminarNodo(nodo->derecha, id);
    } else {
        // Nodo encontrado

        // Caso 1: Nodo con un solo hijo o sin hijos
        if (!nodo->izquierda) {
            NodoABB* temp = nodo->derecha;
            delete nodo; // Liberar la memoria
            return temp; // Retornar el hijo
        } else if (!nodo->derecha) {
            NodoABB* temp = nodo->izquierda;
            delete nodo; // Liberar la memoria
            return temp; // Retornar el hijo
        }

        // Caso 2: Nodo con dos hijos
        NodoABB* temp = buscarMinimo(nodo->derecha); // Encontrar el sucesor
        nodo->id = temp->id; // Copiar el valor del sucesor
        nodo->fecha = temp->fecha; // Copiar la fecha del sucesor (opcional)
        nodo->publicaciones = temp->publicaciones; // Copiar las publicaciones del sucesor
        nodo->derecha = eliminarNodo(nodo->derecha, temp->id); // Eliminar el sucesor
    }

    // Actualizar la altura del nodo
    actualizarAltura(nodo);

    // Obtener el factor de equilibrio
    int balance = obtenerBalance(nodo);

    // Rotaciones para mantener el equilibrio
    if (balance > 1 && obtenerBalance(nodo->izquierda) >= 0) {
        return rotarDerecha(nodo);
    }
    if (balance > 1 && obtenerBalance(nodo->izquierda) < 0) {
        nodo->izquierda = rotarIzquierda(nodo->izquierda);
        return rotarDerecha(nodo);
    }
    if (balance < -1 && obtenerBalance(nodo->derecha) <= 0) {
        return rotarIzquierda(nodo);
    }
    if (balance < -1 && obtenerBalance(nodo->derecha) > 0) {
        nodo->derecha = rotarDerecha(nodo->derecha);
        return rotarIzquierda(nodo);
    }

    return nodo; // Retornar el nodo (sin cambios)
}

NodoABB* ArbolABB::buscarMinimo(NodoABB* nodo) const {
    NodoABB* actual = nodo;
    while (actual->izquierda != nullptr) {
        actual = actual->izquierda;
    }
    return actual;
}


// Convertir la fecha a un formato ordenado (YYYY/MM/DD)
std::string ArbolABB::convertirFecha(const std::string& fechaStr) const {
    int dia, mes, ano;
    char separador;
    std::istringstream iss(fechaStr);
    iss >> dia >> separador >> mes >> separador >> ano;

    std::ostringstream oss;
    oss << std::setw(4) << std::setfill('0') << ano << "/"
        << std::setw(2) << std::setfill('0') << mes << "/"
        << std::setw(2) << std::setfill('0') << dia;

    return oss.str();
}

// Destruir el árbol
void ArbolABB::destruirArbol(NodoABB* nodo) {
    if (nodo) {
        destruirArbol(nodo->izquierda);
        destruirArbol(nodo->derecha);
        delete nodo;
    }
}
