#include "NodoAVL.h"

NodoAVL::NodoAVL(const Usuario& user)
    : usuario(user), izquierdo(nullptr), derecho(nullptr), altura(1) {}
