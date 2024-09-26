#include "nodomatrizdispersa.h"

NodoMatrizDispersa::NodoMatrizDispersa(const std::string& fila, const std::string& columna)
    : nombreFila(fila), nombreColumna(columna), derecha(nullptr), abajo(nullptr), arriba(nullptr), izquierda(nullptr)
{

}
