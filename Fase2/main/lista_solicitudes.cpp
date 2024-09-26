#include "lista_solicitudes.h"
#include "solicitud.h"
#include "PilaReceptor.h"
#include <iostream>
#include "json.hpp"
#include <fstream>
#include <unordered_set>


// Aquí se debe tener un método para obtener la pila del receptor
PilaReceptor& obtenerPilaReceptor(const std::string &correoReceptor);  // Debes implementar esta función

// Constructor y Destructor
ListaSolicitudes::ListaSolicitudes() : cabeza(nullptr) {}

ListaSolicitudes::~ListaSolicitudes() {
    NodoSolicitud* actual = cabeza;
    while (actual != nullptr) {
        NodoSolicitud* temp = actual;
        actual = actual->siguiente;
        delete temp;
    }
}

void ListaSolicitudes::agregarRelacionesAceptadasAMatriz(MatrizDispersa &matriz)
{
    NodoSolicitud *actual = cabeza;

    while (actual)
    {
        if (actual->solicitud.getEstado() == "ACEPTADA")
        {
            matriz.insertarRelacion(actual->solicitud.getEmisor(), actual->solicitud.getReceptor());
        }
        actual = actual->siguiente;
    }
}

ListaSolicitudes::NodoSolicitud::NodoSolicitud(const Solicitud &solicitud)
    : solicitud(solicitud), siguiente(nullptr) {}

// Función para agregar solicitud
void ListaSolicitudes::agregarSolicitud(const Solicitud &solicitud) {
    NodoSolicitud* nuevoNodo = new NodoSolicitud(solicitud);
    nuevoNodo->siguiente = cabeza;
    cabeza = nuevoNodo;
}

// Función para eliminar solicitud
void ListaSolicitudes::eliminarSolicitud(const std::string &emisor, const std::string &receptor) {
    NodoSolicitud* actual = cabeza;
    NodoSolicitud* anterior = nullptr;

    while (actual != nullptr && (actual->solicitud.getEmisor() != emisor || actual->solicitud.getReceptor() != receptor)) {
        anterior = actual;
        actual = actual->siguiente;
    }

    if (actual != nullptr) {
        if (anterior != nullptr) {
            anterior->siguiente = actual->siguiente;
        } else {
            cabeza = actual->siguiente;
        }
        delete actual;
    }
}

bool ListaSolicitudes::actualizarEstadoSolicitud(const std::string &emisor, const std::string &receptor, const std::string &nuevoEstado) {
    NodoSolicitud* actual = cabeza;

    while (actual != nullptr) {
        // Buscar la solicitud que coincida con el emisor y receptor dados
        if ((actual->solicitud.getEmisor() == emisor && actual->solicitud.getReceptor() == receptor) ||
            (actual->solicitud.getEmisor() == receptor && actual->solicitud.getReceptor() == emisor)) {
            // Actualizar el estado de la solicitud
            actual->solicitud.setEstado(nuevoEstado);
            return true; // La actualización fue exitosa
        }
        actual = actual->siguiente;
    }

    // Solicitud no encontrada
    return false;
}

// Función para mostrar solicitudes
void ListaSolicitudes::mostrarSolicitudes() const {
    NodoSolicitud* actual = cabeza;
    while (actual != nullptr) {
        std::cout << "Emisor: " << actual->solicitud.getEmisor()
        << ", Receptor: " << actual->solicitud.getReceptor()
        << ", Estado: " << actual->solicitud.getEstado() << std::endl;
        actual = actual->siguiente;
    }
}

// Función para enviar solicitud
void ListaSolicitudes::enviarSolicitud(const std::string &emisor, const std::string &receptor) {
    // Crear una nueva solicitud con estado "PENDIENTE"
    Solicitud nuevaSolicitud(emisor, receptor, "PENDIENTE");

    // Verificar si ya existe una solicitud entre el mismo emisor y receptor
    NodoSolicitud* actual = cabeza;
    while (actual != nullptr) {
        if (actual->solicitud.getEmisor() == emisor && actual->solicitud.getReceptor() == receptor) {
            std::cerr << "Ya existe una solicitud entre " << emisor << " y " << receptor << std::endl;
            return;
        }
        actual = actual->siguiente;
    }

    // Agregar la nueva solicitud a la lista de solicitudes
    agregarSolicitud(nuevaSolicitud);

    // Agregar la solicitud a la pila de solicitudes recibidas del receptor
    PilaReceptor& pilaReceptor = obtenerPilaReceptor(receptor);
    Receptor nuevaSolicitudReceptor(emisor, receptor, "PENDIENTE");
    pilaReceptor.push(nuevaSolicitudReceptor);

    std::cout << "Solicitud enviada de " << emisor << " a " << receptor << " con estado PENDIENTE." << std::endl;
}



// Función para verificar solicitudes en estado
bool ListaSolicitudes::existeSolicitudEnEstado(const std::string &emisor, const std::string &receptor, const std::string &estado) {
    NodoSolicitud* actual = cabeza;

    while (actual != nullptr) {
        const Solicitud& solicitud = actual->solicitud;
        if ((solicitud.getEmisor() == emisor && solicitud.getReceptor() == receptor) ||
            (solicitud.getEmisor() == receptor && solicitud.getReceptor() == emisor)) {
            if (solicitud.getEstado() == estado) {
                return true;
            }
        }
        actual = actual->siguiente;
    }

    return false;
}

void ListaSolicitudes::cargarRelacionesDesdeJson(const std::string &nombreArchivo)
{    try {
    std::ifstream archivo(nombreArchivo);
    if (archivo.is_open())
    {
        nlohmann::json jsonData;
        archivo >> jsonData;
        archivo.close();

        for (const auto &item : jsonData)
        {
            std::string emisor = item["emisor"];
            std::string receptor = item["receptor"];
            std::string estado = item["estado"];

            Solicitud nuevaSolicitud(emisor, receptor, estado);
            agregarSolicitud(nuevaSolicitud);
        }

        std::cout << "Depuración: Finalizada la carga de relaciones desde " << nombreArchivo << std::endl;
        std::cout << "===============================\n";
        std::cout << "Relaciones cargadas exitosamente.\n";
        std::cout << "===============================\n";
    }

    else
    {
        std::cerr << "Error al abrir el archivo JSON." << std::endl;
    }
    } catch (const nlohmann::json::exception& e) {
        std::cerr << "Error al procesar el archivo JSON: " << e.what() << std::endl;
    }
}

std::vector<Solicitud> ListaSolicitudes::obtenerSolicitudesPorReceptor(const std::string& correoReceptor) const {
    std::vector<Solicitud> solicitudesPorReceptor;
    NodoSolicitud* actual = cabeza;
    while (actual != nullptr) {
        if (actual->solicitud.getReceptor() == correoReceptor) {
            solicitudesPorReceptor.push_back(actual->solicitud);
        }
        actual = actual->siguiente;
    }
    return solicitudesPorReceptor;
}

void ListaSolicitudes::buscarYApilarPendientes(const std::string &correo, const ListaSolicitudes &listaSolicitudes) {
    std::unordered_map<std::string, bool> usuariosConAceptada;
    std::unordered_set<std::string> emisoresYaApilados; // Conjunto para rastrear emisores ya apilados

    // Recoger todas las solicitudes para el receptor especificado
    std::vector<Solicitud> solicitudes = listaSolicitudes.obtenerSolicitudesPorReceptor(correo);

    // Identificar usuarios con solicitudes aceptadas
    for (const auto& solicitud : solicitudes) {
        if ((solicitud.getEmisor() == correo || solicitud.getReceptor() == correo) &&
            solicitud.getEstado() == "ACEPTADA") {
            usuariosConAceptada[solicitud.getEmisor()] = true;
            usuariosConAceptada[solicitud.getReceptor()] = true;
        }
    }

    // Apilar solicitudes pendientes
    for (const auto& solicitud : solicitudes) {
        if (solicitud.getReceptor() == correo && solicitud.getEstado() == "PENDIENTE") {
            if (usuariosConAceptada.find(solicitud.getEmisor()) == usuariosConAceptada.end()) {
                // Verificar si el emisor ya ha sido apilado
                if (emisoresYaApilados.find(solicitud.getEmisor()) == emisoresYaApilados.end()) {
                    obtenerPilaReceptor(correo).push(Receptor(solicitud.getEmisor(), correo, "PENDIENTE"));
                    emisoresYaApilados.insert(solicitud.getEmisor()); // Marcar emisor como apilado
                }
            }
        }
    }

    // Mostrar mensaje final
    if (obtenerPilaReceptor(correo).estaVacia()) {
        std::cout << "No se encontraron solicitudes pendientes para el correo: " << correo << std::endl;
    } else {
        std::cout << "Solicitudes pendientes apiladas para el correo: " << correo << std::endl;
    }
}

std::vector<std::string> ListaSolicitudes::obtenerSolicitudesEnviadas(const std::string &correoEmisor) const {
    std::vector<std::string> receptores;
    NodoSolicitud* actual = cabeza;

    while (actual != nullptr) {
        if (actual->solicitud.getEmisor() == correoEmisor) {
            receptores.push_back(actual->solicitud.getReceptor());
        }
        actual = actual->siguiente;
    }

    return receptores;
}
