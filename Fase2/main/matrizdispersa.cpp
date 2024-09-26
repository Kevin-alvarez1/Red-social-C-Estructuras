#include "MatrizDispersa.h"
MatrizDispersa matrizDispersa;

// Constructor
MatrizDispersa::MatrizDispersa() : cabeza(nullptr) {}

// Métodos privados
NodoMatrizDispersa* MatrizDispersa::buscarFila(const std::string& fila) const {
    NodoMatrizDispersa* temp = cabeza;
    while (temp && temp->nombreFila != fila) {
        temp = temp->abajo;
    }
    return temp;
}

NodoMatrizDispersa* MatrizDispersa::buscarColumna(NodoMatrizDispersa* filaNodo, const std::string& columna) const {
    NodoMatrizDispersa* temp = filaNodo;
    while (temp && temp->nombreColumna != columna) {
        temp = temp->derecha;
    }
    return temp;
}


std::string MatrizDispersa::escapeXml(const std::string& input) const {
    std::string escaped = input;
    std::string toReplace[][2] = {{"<", "&lt;"}, {">", "&gt;"}, {"&", "&amp;"}};
    for (const auto& pair : toReplace) {
        size_t pos = 0;
        while ((pos = escaped.find(pair[0], pos)) != std::string::npos) {
            escaped.replace(pos, pair[0].length(), pair[1]);
            pos += pair[1].length();
        }
    }
    return escaped;
}

// Métodos públicos
bool MatrizDispersa::existeNombre(const std::string& nombre) const {
    return std::find(nombres.begin(), nombres.end(), nombre) != nombres.end();
}

void MatrizDispersa::insertarNombre(const std::string& nombre) {
    if (!existeNombre(nombre)) {
        nombres.push_back(nombre);
    }
}

void MatrizDispersa::insertarRelacion(const std::string& nombreFila, const std::string& nombreColumna) {
    insertarNombre(nombreFila);
    insertarNombre(nombreColumna);

    NodoMatrizDispersa* filaNodo = buscarFila(nombreFila);

    if (!filaNodo) {
        NodoMatrizDispersa* nuevoNodoFila = new NodoMatrizDispersa(nombreFila, "");
        if (!cabeza) {
            cabeza = nuevoNodoFila;
        } else {
            NodoMatrizDispersa* temp = cabeza;
            while (temp->abajo) {
                temp = temp->abajo;
            }
            temp->abajo = nuevoNodoFila;
        }
        filaNodo = nuevoNodoFila;
    }

    NodoMatrizDispersa* columnaNodo = buscarColumna(filaNodo, nombreColumna);

    if (!columnaNodo) {
        NodoMatrizDispersa* nuevoNodoColumna = new NodoMatrizDispersa(nombreFila, nombreColumna);
        NodoMatrizDispersa* temp = filaNodo;
        while (temp->derecha) {
            temp = temp->derecha;
        }
        temp->derecha = nuevoNodoColumna;
    }
}

void MatrizDispersa::mostrarMatriz() const
{
    NodoMatrizDispersa *tempFila = cabeza;
    std::cout << "Matriz de relaciones:" << std::endl;
    while (tempFila)
    {
        NodoMatrizDispersa *tempColumna = tempFila->derecha;
        while (tempColumna)
        {
            std::cout << "Relacion: " << tempColumna->nombreFila << " - " << tempColumna->nombreColumna << std::endl;
            tempColumna = tempColumna->derecha;
        }
        tempFila = tempFila->abajo;
    }
}

std::vector<std::string> MatrizDispersa::obtenerAmigos(const std::string& correo) const {
    std::vector<std::string> amigos;
    NodoMatrizDispersa* filaNodo = buscarFila(correo);
    if (filaNodo) {
        NodoMatrizDispersa* columnaNodo = filaNodo->derecha;
        while (columnaNodo) {
            amigos.push_back(columnaNodo->nombreColumna);
            columnaNodo = columnaNodo->derecha;
        }
    }

    NodoMatrizDispersa* tempFila = cabeza;
    while (tempFila) {
        NodoMatrizDispersa* tempColumna = tempFila->derecha;
        while (tempColumna) {
            if (tempColumna->nombreColumna == correo) {
                amigos.push_back(tempFila->nombreFila);
            }
            tempColumna = tempColumna->derecha;
        }
        tempFila = tempFila->abajo;
    }

    std::sort(amigos.begin(), amigos.end());
    amigos.erase(std::unique(amigos.begin(), amigos.end()), amigos.end());

    return amigos;
}
