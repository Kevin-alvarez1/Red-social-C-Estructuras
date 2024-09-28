#include "arbolbcomentario.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <queue>
#include <cstdlib>
#include <map>
#include <algorithm>

ArbolBComentario arbolComentarios_;
// Constructor de la clase NodoArbolBComentario
NodoArbolBComentario::NodoArbolBComentario(bool hoja) : esHoja(hoja) {}

// Constructor de la clase ArbolBComentario
ArbolBComentario::ArbolBComentario() : raiz(nullptr) {}

// Método para eliminar comentarios de un correo específico en el NodoArbolBComentario
void NodoArbolBComentario::eliminarComentariosPorCorreo(const std::string& correo) {
    // Eliminar todos los comentarios de este nodo que coincidan con el correo dado
    comentarios.erase(
        std::remove_if(comentarios.begin(), comentarios.end(),
                       [&correo](const Comentario& c) { return c.getCorreo() == correo; }),
        comentarios.end()
        );

    // Recursivamente llamar a los hijos para eliminar los comentarios en ellos
    if (!esHoja) {
        for (auto hijo : hijos) {
            hijo->eliminarComentariosPorCorreo(correo);
        }
    }
}

// Método para eliminar comentarios de un correo específico en todo el árbol B
void ArbolBComentario::eliminarComentariosPorCorreo(const std::string& correo) {
    if (raiz != nullptr) {
        raiz->eliminarComentariosPorCorreo(correo);
    }
}



// Método para comparar comentarios por fecha y hora
bool compararFechas(const std::string& fecha1, const std::string& fecha2) {
    // Dividir las fechas en día, mes y año
    std::istringstream iss1(fecha1);
    std::istringstream iss2(fecha2);
    int dia1, mes1, anio1;
    int dia2, mes2, anio2;

    iss1 >> dia1 >> mes1 >> anio1;
    iss2 >> dia2 >> mes2 >> anio2;

    // Comparar primero por año, luego por mes y finalmente por día
    if (anio1 != anio2) {
        return anio1 < anio2; // Comparar años
    }
    if (mes1 != mes2) {
        return mes1 < mes2; // Comparar meses
    }
    return dia1 < dia2; // Comparar días
}

bool compararComentarios(const Comentario& c1, const Comentario& c2) {
    // Comparar fechas
    if (c1.getFecha() != c2.getFecha()) {
        return compararFechas(c1.getFecha(), c2.getFecha());
    }

    // Si las fechas son iguales, comparar horas
    return c1.getHora() < c2.getHora(); // Asumiendo que las horas están en formato HH:MM
}

// Obtener comentarios de una publicación
std::vector<Comentario> ArbolBComentario::getComentariosDePublicacion(int idPublicacion) const {
    std::vector<Comentario> comentarios;
    if (raiz != nullptr) {
        raiz->obtenerComentariosPorPublicacion(idPublicacion, comentarios);
    }
    return comentarios;
}

void ArbolBComentario::graficarArbolB(const std::string& nombreArchivo, int idPublicacion) {
    std::ofstream archivo(nombreArchivo);
    archivo << "digraph G {\n";
    archivo << "node [shape=record];\n";

    int nodeCount = 0;
    std::map<NodoArbolBComentario*, int> nodeMap;

    // Llamar a la función recursiva para procesar el árbol
    graficarNodo(raiz, nodeCount, nodeMap, archivo, idPublicacion);

    archivo << "}\n";
    archivo.close();
}

void ArbolBComentario::graficarNodo(NodoArbolBComentario* nodo, int& nodeCount, std::map<NodoArbolBComentario*, int>& nodeMap, std::ofstream& archivo, int idPublicacion) {
    if (nodo == nullptr) return;

    // Crear un identificador para el nodo actual
    int currentNodeId = nodeCount++;
    nodeMap[nodo] = currentNodeId;

    // Crear la etiqueta del nodo con los comentarios que coinciden con idPublicacion
    std::stringstream label;
    label << "{";

    // Agregar los campos a la etiqueta del nodo
    for (size_t i = 0; i < nodo->comentarios.size(); ++i) {
        if (nodo->comentarios[i].getIdPublicacion() == idPublicacion) {
            // Solo agregar comentarios que coinciden con idPublicacion
            label << "<f" << i << "> " << nodo->comentarios[i].getFecha() << " " << nodo->comentarios[i].getHora();

            if (i < nodo->comentarios.size() - 1) {
                label << " | ";  // Separador entre campos
            }
        }
    }
    label << "}";

    // Escribir el nodo con su etiqueta
    archivo << "node" << currentNodeId << " [label=\"" << label.str() << "\"];\n";

    // Procesar los hijos
    for (size_t i = 0; i < nodo->hijos.size(); ++i) {
        // Graficar el hijo
        graficarNodo(nodo->hijos[i], nodeCount, nodeMap, archivo, idPublicacion);

        // Crear un enlace entre el nodo actual y su hijo
        archivo << "node" << currentNodeId << " -> node" << nodeMap[nodo->hijos[i]] << ";\n";
    }
}

// Insertar un nuevo comentario en el árbol B
void ArbolBComentario::insertar(const Comentario& comentario) {
    if (raiz == nullptr) {
        // Si no existe raíz, crearla y agregar el comentario
        raiz = new NodoArbolBComentario(true);
        raiz->comentarios.push_back(comentario);
    } else {
        // Si la raíz está llena, dividirla
        if (raiz->comentarios.size() == 2 * ORDEN - 1) {
            NodoArbolBComentario* nuevoNodo = new NodoArbolBComentario(false);
            nuevoNodo->hijos.push_back(raiz);
            nuevoNodo->dividirHijo(0, raiz);

            // Determinar en cuál de los hijos insertar el nuevo comentario
            int i = 0;
            if (compararComentarios(nuevoNodo->comentarios[0], comentario)) {
                i++;
            }
            nuevoNodo->hijos[i]->insertarNoLleno(comentario);

            raiz = nuevoNodo; // Actualizar la raíz
        } else {
            // Si no está llena, insertar directamente en la raíz
            raiz->insertarNoLleno(comentario);
        }
    }
}

void NodoArbolBComentario::dividirHijo(int indice, NodoArbolBComentario* nodoLleno) {
    NodoArbolBComentario* nuevoNodo = new NodoArbolBComentario(nodoLleno->esHoja);
    int posicionMedia = ORDEN - 1; // Cambié a ORDEN - 1 para obtener el índice correcto

    // Mover los comentarios del nodo lleno al nuevo nodo
    for (int i = posicionMedia + 1; i < nodoLleno->comentarios.size(); i++) {
        nuevoNodo->comentarios.push_back(nodoLleno->comentarios[i]);
    }
    nodoLleno->comentarios.resize(posicionMedia); // Redimensionar el nodo lleno

    // Mover los hijos del nodo lleno al nuevo nodo si no es hoja
    if (!nodoLleno->esHoja) {
        for (int i = posicionMedia + 1; i < nodoLleno->hijos.size(); i++) {
            nuevoNodo->hijos.push_back(nodoLleno->hijos[i]);
        }
        nodoLleno->hijos.resize(posicionMedia + 1); // Redimensionar los hijos
    }

    // Insertar el comentario medio en el nodo padre
    comentarios.insert(comentarios.begin() + indice, nodoLleno->comentarios[posicionMedia]);
    hijos.insert(hijos.begin() + indice + 1, nuevoNodo); // Insertar el nuevo nodo como hijo
}

void NodoArbolBComentario::insertarNoLleno(const Comentario& comentario) {
    int i = comentarios.size() - 1;
    std::string fechaHoraComentario = comentario.getFecha() + " " + comentario.getHora();

    // Si el nodo es hoja, insertar directamente en el arreglo de comentarios
    if (esHoja) {
        comentarios.push_back(comentario); // Agregar el comentario al final

        // Mover los comentarios mayores a la derecha para hacer espacio
        while (i >= 0 && compararComentarios(comentarios[i], comentario)) {
            comentarios[i + 1] = comentarios[i];
            i--;
        }

        // Insertar el comentario en su posición correcta
        comentarios[i + 1] = comentario;
    } else {
        // Si el nodo no es hoja, encontrar el hijo adecuado para insertar
        while (i >= 0 && compararComentarios(comentarios[i], comentario)) {
            i--;
        }
        i++; // Mover el índice al hijo adecuado

        // Si el hijo está lleno, dividirlo
        if (hijos[i]->comentarios.size() == 2 * ORDEN - 1) {
            dividirHijo(i, hijos[i]);

            // Después de dividir, decidir si insertar en el nuevo nodo
            if (compararComentarios(comentarios[i], comentario)) {
                i++;
            }
        }

        // Insertar en el hijo adecuado
        hijos[i]->insertarNoLleno(comentario);
    }
}


void NodoArbolBComentario::recorrer() const {
    int i;
    for (i = 0; i < comentarios.size(); i++) {
        if (!esHoja) {
            hijos[i]->recorrer();
        }
        std::cout << comentarios[i].getFecha() << " " << comentarios[i].getHora() << " - " << comentarios[i].getComentario() << std::endl;
    }
    if (!esHoja) {
        hijos[i]->recorrer();
    }
}

// Obtener comentarios de una publicación
void NodoArbolBComentario::obtenerComentariosPorPublicacion(int idPublicacion, std::vector<Comentario>& comentarios) const {
    for (const auto& comentario : this->comentarios) {
        if (comentario.getIdPublicacion() == idPublicacion) {
            comentarios.push_back(comentario);
        }
    }

    if (!esHoja) {
        for (const auto& hijo : this->hijos) {
            hijo->obtenerComentariosPorPublicacion(idPublicacion, comentarios);
        }
    }
}

NodoArbolBComentario* ArbolBComentario::buscar(const std::string& fecha, const std::string& hora) const {
    return (raiz == nullptr) ? nullptr : raiz->buscar(fecha, hora);
}

// Función para buscar un comentario por fecha y hora
NodoArbolBComentario* NodoArbolBComentario::buscar(const std::string& fecha, const std::string& hora) const {
    std::string fechaHora = fecha + " " + hora;
    int i = 0;
    while (i < comentarios.size() && (comentarios[i].getFecha() + " " + comentarios[i].getHora()) < fechaHora) {
        i++;
    }

    if (i < comentarios.size() && (comentarios[i].getFecha() + " " + comentarios[i].getHora()) == fechaHora) {
        return const_cast<NodoArbolBComentario*>(this); // Encontrado
    }

    if (esHoja) {
        return nullptr; // No encontrado
    }

    return hijos[i]->buscar(fecha, hora);
}


// Método recursivo para obtener comentarios
void ArbolBComentario::obtenerComentariosRecursivos(NodoArbolBComentario* nodo, int idPublicacion, std::vector<Comentario>& comentarios) const {
    if (nodo != nullptr) {
        for (const auto& comentario : nodo->comentarios) {
            if (comentario.getIdPublicacion() == idPublicacion) {
                comentarios.push_back(comentario);
            }
        }

        if (!nodo->esHoja) {
            for (size_t i = 0; i < nodo->hijos.size(); ++i) {
                nodo->hijos[i]->obtenerComentariosPorPublicacion(idPublicacion, comentarios);
            }
        }
    }
}
