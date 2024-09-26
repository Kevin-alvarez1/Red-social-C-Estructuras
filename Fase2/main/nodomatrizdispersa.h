#ifndef NODOMATRIZDISPERSA_H
#define NODOMATRIZDISPERSA_H

#include <string>

class NodoMatrizDispersa
{
public:
    std::string nombreFila;
    std::string nombreColumna;
    NodoMatrizDispersa *derecha;
    NodoMatrizDispersa *abajo;
    NodoMatrizDispersa *arriba;
    NodoMatrizDispersa *izquierda;

    NodoMatrizDispersa(const std::string& fila, const std::string& columna);
};

#endif // NODOMATRIZDISPERSA_H
