#ifndef MATRIZ_DISPERSA_H
#define MATRIZ_DISPERSA_H

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include "nodomatrizdispersa.h"

class MatrizDispersa {
private:
    std::vector<std::string> nombres;
    NodoMatrizDispersa* cabeza;
    NodoMatrizDispersa* buscarFila(const std::string& fila) const;
    NodoMatrizDispersa* buscarColumna(NodoMatrizDispersa* filaNodo, const std::string& columna) const;

    std::string escapeXml(const std::string& input) const;

public:
    // Constructor
    MatrizDispersa();

    // Métodos públicos
    bool existeNombre(const std::string& nombre) const;
    void insertarNombre(const std::string& nombre);
    void insertarRelacion(const std::string& nombreFila, const std::string& nombreColumna);

    std::vector<std::string> obtenerAmigos(const std::string& correo) const;
    void mostrarMatriz() const;

};

#endif // MATRIZ_DISPERSA_H
