#include "ListaUsuarios.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <functional>

ListaUsuarios::ListaUsuarios() : raiz(nullptr) {}

ListaUsuarios::~ListaUsuarios() {
    eliminarArbol(raiz);
}

void ListaUsuarios::eliminarArbol(NodoAVL* nodo) {
    if (nodo != nullptr) {
        eliminarArbol(nodo->izquierdo);
        eliminarArbol(nodo->derecho);
        delete nodo;
    }
}

int ListaUsuarios::altura(NodoAVL* nodo) const {
    return (nodo ? nodo->altura : 0);
}

int ListaUsuarios::balance(NodoAVL* nodo) const {
    return (nodo ? altura(nodo->izquierdo) - altura(nodo->derecho) : 0);
}

NodoAVL* ListaUsuarios::rotarDerecha(NodoAVL* y) {
    NodoAVL* x = y->izquierdo;
    NodoAVL* T2 = x->derecho;

    x->derecho = y;
    y->izquierdo = T2;

    y->altura = std::max(altura(y->izquierdo), altura(y->derecho)) + 1;
    x->altura = std::max(altura(x->izquierdo), altura(x->derecho)) + 1;

    return x;
}

NodoAVL* ListaUsuarios::rotarIzquierda(NodoAVL* x) {
    NodoAVL* y = x->derecho;
    NodoAVL* T2 = y->izquierdo;

    y->izquierdo = x;
    x->derecho = T2;

    x->altura = std::max(altura(x->izquierdo), altura(x->derecho)) + 1;
    y->altura = std::max(altura(y->izquierdo), altura(y->derecho)) + 1;

    return y;
}

NodoAVL* ListaUsuarios::balancear(NodoAVL* nodo) {
    int balanceFactor = balance(nodo);

    if (balanceFactor > 1) {
        if (balance(nodo->izquierdo) >= 0) {
            return rotarDerecha(nodo);
        } else {
            nodo->izquierdo = rotarIzquierda(nodo->izquierdo);
            return rotarDerecha(nodo);
        }
    }
    if (balanceFactor < -1) {
        if (balance(nodo->derecho) <= 0) {
            return rotarIzquierda(nodo);
        } else {
            nodo->derecho = rotarDerecha(nodo->derecho);
            return rotarIzquierda(nodo);
        }
    }

    return nodo;
}

NodoAVL* ListaUsuarios::insertar(NodoAVL* nodo, const Usuario& usuario) {
    if (nodo == nullptr) {
        return new NodoAVL(usuario);
    }

    if (usuario.getCorreo() < nodo->usuario.getCorreo()) {
        nodo->izquierdo = insertar(nodo->izquierdo, usuario);
    } else if (usuario.getCorreo() > nodo->usuario.getCorreo()) {
        nodo->derecho = insertar(nodo->derecho, usuario);
    } else {
        // Usuario ya existe, no se permite duplicados
        return nodo;
    }

    nodo->altura = std::max(altura(nodo->izquierdo), altura(nodo->derecho)) + 1;
    return balancear(nodo);
}

NodoAVL* ListaUsuarios::buscar(NodoAVL* nodo, const std::string& correo) const {
    if (nodo == nullptr || nodo->usuario.getCorreo() == correo) {
        return nodo;
    }

    if (correo < nodo->usuario.getCorreo()) {
        return buscar(nodo->izquierdo, correo);
    }

    return buscar(nodo->derecho, correo);
}

Usuario ListaUsuarios::mostrarDatosPorCorreo(const std::string& correo) const {
    NodoAVL* nodo = buscar(raiz, correo);
    if (nodo) {
        return nodo->usuario;
    }
    return Usuario("", "", "", "", ""); // Usuario vacío si no se encuentra
}

bool ListaUsuarios::usuarioDuplicado(const std::string& correo) const {
    return buscar(raiz, correo) != nullptr;
}

NodoAVL* ListaUsuarios::minimoNodo(NodoAVL* nodo) {
    NodoAVL* actual = nodo;
    while (actual && actual->izquierdo) {
        actual = actual->izquierdo;
    }
    return actual;
}

NodoAVL* ListaUsuarios::eliminar(NodoAVL* nodo, const std::string& correo) {
    if (nodo == nullptr) {
        return nodo;
    }

    if (correo < nodo->usuario.getCorreo()) {
        nodo->izquierdo = eliminar(nodo->izquierdo, correo);
    } else if (correo > nodo->usuario.getCorreo()) {
        nodo->derecho = eliminar(nodo->derecho, correo);
    } else {
        if (nodo->izquierdo == nullptr) {
            NodoAVL* temp = nodo->derecho;
            delete nodo;
            return temp;
        } else if (nodo->derecho == nullptr) {
            NodoAVL* temp = nodo->izquierdo;
            delete nodo;
            return temp;
        }

        NodoAVL* temp = minimoNodo(nodo->derecho);
        nodo->usuario = temp->usuario;
        nodo->derecho = eliminar(nodo->derecho, temp->usuario.getCorreo());
    }

    if (nodo == nullptr) {
        return nodo;
    }

    nodo->altura = std::max(altura(nodo->izquierdo), altura(nodo->derecho)) + 1;
    return balancear(nodo);
}

void ListaUsuarios::borrarUsuarioPorCorreo(const std::string& correo) {
    raiz = eliminar(raiz, correo);
}

void ListaUsuarios::cargarUsuariosDesdeJson(const std::string& nombreArchivo) {
    std::ifstream archivo(nombreArchivo);

    if (!archivo.is_open()) {
        std::cerr << "Error al abrir el archivo JSON." << std::endl;
        return;
    }

    try {
        nlohmann::json jsonData;
        archivo >> jsonData;

        for (const auto& item : jsonData) {
            std::string nombre = item.at("nombres").get<std::string>();
            std::string apellido = item.at("apellidos").get<std::string>();
            std::string fecha_de_nacimiento = item.at("fecha_de_nacimiento").get<std::string>();
            std::string correo = item.at("correo").get<std::string>();
            std::string contrasena = item.at("contraseña").get<std::string>();

            Usuario usuario(nombre, apellido, fecha_de_nacimiento, correo, contrasena);
            agregarUsuario(usuario);
        }
    } catch (const nlohmann::json::exception& e) {
        std::cerr << "Error al procesar el archivo JSON: " << e.what() << std::endl;
    }

    archivo.close();
}


void ListaUsuarios::agregarUsuario(const Usuario& usuario) {
    if (!usuarioDuplicado(usuario.getCorreo())) {
        raiz = insertar(raiz, usuario);
    } else {
        std::cerr << "El usuario con correo " << usuario.getCorreo() << " ya existe." << std::endl;
    }
}

Usuario* ListaUsuarios::buscarUsuarioPorCorreo(const std::string& correo) {
    NodoAVL* nodo = buscar(raiz, correo);
    if (nodo != nullptr) {
        return &nodo->usuario; // Retorna el puntero al usuario dentro del nodo
    }
    return nullptr; // Retorna nullptr si el usuario no se encuentra
}



bool ListaUsuarios::buscarUsuarioPorCorreoyContrasena(const std::string& correo, const std::string& contrasena) const {
    NodoAVL* nodo = buscar(raiz, correo);
    return nodo && nodo->usuario.getContrasena() == contrasena;
}

// PreOrder
void ListaUsuarios::preOrder(NodoAVL* nodo, std::vector<Usuario>& usuarios) const {
    if (nodo != nullptr) {
        usuarios.push_back(nodo->usuario);  // Visitar el nodo
        preOrder(nodo->izquierdo, usuarios);  // Recorrer el subárbol izquierdo
        preOrder(nodo->derecho, usuarios);   // Recorrer el subárbol derecho
    }
}

// InOrder
void ListaUsuarios::inOrder(NodoAVL* nodo, std::vector<Usuario>& usuarios) const {
    if (nodo != nullptr) {
        inOrder(nodo->izquierdo, usuarios);  // Recorrer el subárbol izquierdo
        usuarios.push_back(nodo->usuario);  // Visitar el nodo
        inOrder(nodo->derecho, usuarios);   // Recorrer el subárbol derecho
    }
}

// PostOrder
void ListaUsuarios::postOrder(NodoAVL* nodo, std::vector<Usuario>& usuarios) const {
    if (nodo != nullptr) {
        postOrder(nodo->izquierdo, usuarios);  // Recorrer el subárbol izquierdo
        postOrder(nodo->derecho, usuarios);   // Recorrer el subárbol derecho
        usuarios.push_back(nodo->usuario);  // Visitar el nodo
    }
}

// Método para obtener usuarios en un vector según el recorrido
std::vector<Usuario> ListaUsuarios::obtenerUsuariosEnOrden(const std::string& tipoOrden) const {
    std::vector<Usuario> usuarios;

    if (tipoOrden == "PreOrder") {
        preOrder(raiz, usuarios);
    } else if (tipoOrden == "InOrder") {
        inOrder(raiz, usuarios);
    } else if (tipoOrden == "PostOrder") {
        postOrder(raiz, usuarios);
    }

    return usuarios;
}

void ListaUsuarios::generateDot(const std::string& filename) const {
    std::ofstream file(filename + ".dot");

    if (!file.is_open()) {
        std::cerr << "No se pudo abrir el archivo para escribir." << std::endl;
        return;
    }

    file << "digraph AVLTree {" << std::endl;
    file << "    node [shape=record];" << std::endl;

    generateDotRec(raiz, file);

    file << "}" << std::endl;

    file.close();

    // Convertir el archivo DOT a PNG usando Graphviz
    std::string command = "dot -Tpng " + filename + ".dot -o " + filename + ".png";
    int result = system(command.c_str());

    if (result != 0) {
        std::cerr << "Error al convertir el archivo DOT a PNG." << std::endl;
    }
}

void ListaUsuarios::generateDotRec(NodoAVL* nodo, std::ofstream& file) const {
    if (nodo != nullptr) {
        if (nodo->izquierdo != nullptr) {
            file << "    \"" << nodo->usuario.getCorreo() << "\" -> \"" << nodo->izquierdo->usuario.getCorreo() << "\";" << std::endl;
        }
        if (nodo->derecho != nullptr) {
            file << "    \"" << nodo->usuario.getCorreo() << "\" -> \"" << nodo->derecho->usuario.getCorreo() << "\";" << std::endl;
        }
        generateDotRec(nodo->izquierdo, file);
        generateDotRec(nodo->derecho, file);
    }
}
