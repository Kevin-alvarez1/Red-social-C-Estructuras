#ifndef NODOAVL_H
#define NODOAVL_H

#include "usuario.h"

class NodoAVL {
public:
    Usuario usuario;
    NodoAVL* izquierdo;
    NodoAVL* derecho;
    int altura;

    NodoAVL(const Usuario& user);

};

#endif // NODOAVL_H
