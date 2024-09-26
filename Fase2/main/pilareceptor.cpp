#include "pilareceptor.h"

// Definición del mapa global
std::unordered_map<std::string, PilaReceptor> pilasReceptores;

// Definición de los métodos de la clase PilaReceptor

NodoReceptor::NodoReceptor(const Receptor& receptor_) : receptor(receptor_), siguiente(nullptr) {}

PilaReceptor::PilaReceptor() : cima(nullptr) {}

PilaReceptor::~PilaReceptor() {
    while (!estaVacia()) {
        pop();
    }
}

bool PilaReceptor::estaVacia() const {
    return cima == nullptr;
}

void PilaReceptor::push(const Receptor& receptor) {
    NodoReceptor* nuevoNodo = new NodoReceptor(receptor);
    nuevoNodo->siguiente = cima;
    cima = nuevoNodo;
}

Receptor PilaReceptor::pop() {
    if (estaVacia()) {
        std::cerr << "Error: La pila está vacía, no se puede hacer pop." << std::endl;
        return Receptor("", "", "");
    }
    NodoReceptor* nodoAEliminar = cima;
    Receptor receptor = nodoAEliminar->receptor;
    cima = cima->siguiente;
    delete nodoAEliminar;
    return receptor;
}

Receptor PilaReceptor::peek() const {
    if (estaVacia()) {
        std::cerr << "Error: La pila está vacía, no se puede hacer peek." << std::endl;
        return Receptor("", "", "");
    }
    return cima->receptor;
}

void PilaReceptor::mostrarPila() const {
    NodoReceptor* actual = cima;
    while (actual != nullptr) {
        std::cout << "Emisor: " << actual->receptor.getEmisor()
        << ", Receptor: " << actual->receptor.getReceptor()
        << ", Estado: " << actual->receptor.getEstado() << std::endl;
        actual = actual->siguiente;
    }
}

// Definición de la función global
PilaReceptor& obtenerPilaReceptor(const std::string &correoReceptor) {
    // Si el receptor no tiene una pila, crea una nueva
    if (pilasReceptores.find(correoReceptor) == pilasReceptores.end()) {
        pilasReceptores[correoReceptor] = PilaReceptor();
    }
    return pilasReceptores[correoReceptor];
}

bool PilaReceptor::actualizarEstadoSolicitud(const std::string& emisor, const std::string& receptor, const std::string& nuevoEstado) {
    NodoReceptor* actual = cima;
    while (actual != nullptr) {
        if (actual->receptor.getEmisor() == emisor &&
            actual->receptor.getReceptor() == receptor &&
            actual->receptor.getEstado() == "PENDIENTE") {
            // Encontró la solicitud pendiente
            actual->receptor.setEstado(nuevoEstado);
            return true;  // Indica que la actualización fue exitosa
        }
        actual = actual->siguiente;
    }
    // Si no se encontró la solicitud
    std::cerr << "No se encontró la solicitud pendiente de " << emisor << " a " << receptor << std::endl;
    return false;  // Indica que la solicitud no fue encontrada
}

PilaReceptor::PilaReceptor(const PilaReceptor& otra) : cima(nullptr) {
    NodoReceptor* actual = otra.cima;
    NodoReceptor* ultimo = nullptr;
    while (actual) {
        NodoReceptor* nuevoNodo = new NodoReceptor(*actual);
        if (!cima) {
            cima = nuevoNodo;
        } else {
            ultimo->siguiente = nuevoNodo;
        }
        ultimo = nuevoNodo;
        actual = actual->siguiente;
    }
}


PilaReceptor& PilaReceptor::operator=(const PilaReceptor& otra) {
    if (this == &otra) return *this; // Autocopia

    // Limpiar pila actual
    while (!estaVacia()) {
        pop();
    }

    NodoReceptor* actual = otra.cima;
    NodoReceptor* ultimo = nullptr;
    while (actual) {
        NodoReceptor* nuevoNodo = new NodoReceptor(*actual);
        if (!cima) {
            cima = nuevoNodo;
        } else {
            ultimo->siguiente = nuevoNodo;
        }
        ultimo = nuevoNodo;
        actual = actual->siguiente;
    }

    return *this;
}

