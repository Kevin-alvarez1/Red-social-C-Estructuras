#ifndef LISTA_SOLICITUDES_H
#define LISTA_SOLICITUDES_H

#include "solicitud.h"
#include <string>
#include <vector>
#include "matrizdispersa.h"
#include "arbolabb.h"

extern MatrizDispersa matrizDispersa;

class ListaSolicitudes {
private:
    class NodoSolicitud {
    public:
        Solicitud solicitud;
        NodoSolicitud* siguiente;

        NodoSolicitud(const Solicitud &solicitud);
    };

    NodoSolicitud* cabeza;

public:
    ListaSolicitudes();
    ~ListaSolicitudes();

    void agregarSolicitud(const Solicitud &solicitud);
    void eliminarSolicitud(const std::string &emisor, const std::string &receptor);
    void mostrarSolicitudes() const;
    void cargarRelacionesDesdeJson(const std::string &filename);
    void enviarSolicitud(const std::string &emisor, const std::string &receptor);
    std::vector<std::string> obtenerSolicitudesEnviadas(const std::string &correoEmisor) const;
    bool existeSolicitudEnEstado(const std::string &emisor, const std::string &receptor, const std::string &estado);
    std::vector<Solicitud> obtenerSolicitudesPorReceptor(const std::string &correoReceptor) const;
    void buscarYApilarPendientes(const std::string &correo, const ListaSolicitudes &listaSolicitudes);
    bool actualizarEstadoSolicitud(const std::string& emisor, const std::string& receptor, const std::string& nuevoEstado);
    void agregarRelacionesAceptadasAMatriz(MatrizDispersa &matriz);

};

#endif // LISTA_SOLICITUDES_H
