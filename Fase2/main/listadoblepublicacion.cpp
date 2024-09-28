#include "listaDoblePublicacion.h"
#include <iostream>
#include <fstream>
#include "json.hpp"
#include "arbolabb.h"
#include "matrizdispersa.h"


ListaDoblePublicacion::ListaDoblePublicacion() : cabeza(nullptr), cola(nullptr), siguienteId(1)
{
    std::cout << "Depuración: Lista de publicaciones creada." << std::endl;
}

ListaDoblePublicacion::~ListaDoblePublicacion()
{
    NodoPublicacion *actual = cabeza;
    while (actual != nullptr)
    {
        NodoPublicacion *temp = actual;
        actual = actual->siguiente;
        std::cout << "Depuración: Eliminando nodo de publicación con correo: " << temp->publicacion.getCorreo() << std::endl;
        delete temp;
    }
}

void ListaDoblePublicacion::cargarPublicacionesDesdeJson(const std::string &filename) {
    try {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "No se pudo abrir el archivo JSON: " << filename << std::endl;
            return;
        }

        nlohmann::json jsonData;
        file >> jsonData;

        for (const auto &item : jsonData) {
            // Extraer datos de la publicación
            std::string correo = item["correo"];
            std::string contenido = item["contenido"];
            std::string fecha = item["fecha"];
            std::string hora = item["hora"];

            // Crear nueva publicación
            Publicacion nuevaPublicacion(siguienteId++, correo, contenido, fecha, hora);

            // Agregar comentarios
            if (item.contains("comentarios")) {
                for (const auto &comentarioItem : item["comentarios"]) {

                    // Extraer datos del comentario
                    std::string comentarioCorreo = comentarioItem["correo"];
                    std::string comentarioContenido = comentarioItem["comentario"];
                    std::string comentarioFecha = comentarioItem["fecha"];
                    std::string comentarioHora = comentarioItem["hora"];

                    // Crear nuevo comentario usando el ID de la publicación actual
                    Comentario nuevoComentario(comentarioContenido, nuevaPublicacion.getId(), comentarioCorreo, comentarioFecha, comentarioHora);

                    // Agregar comentario a la publicación
                    arbolComentarios_.insertar(nuevoComentario);
                    std::cout << "Obteniendo comentarios para la publicación ID: " << nuevaPublicacion.getId() << "comentario añadido con el id: " << nuevoComentario.getIdPublicacion() << std::endl;
                }
            }

            // Agregar la nueva publicación a la lista
            agregarPublicacion(nuevaPublicacion);
        }

        file.close();
    }
    catch (const nlohmann::json::exception& e) {
        std::cerr << "Error al procesar el archivo JSON: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error general: " << e.what() << std::endl;
    }
}

void ListaDoblePublicacion::eliminarPublicacionPorId(int id) {
    NodoPublicacion* actual = cabeza; // Comenzamos desde la cabeza

    while (actual != nullptr) {
        if (actual->publicacion.getId() == id) { // Verificamos si el ID coincide
            if (actual->anterior != nullptr) { // Si no es el primer nodo
                actual->anterior->siguiente = actual->siguiente; // Actualizamos el siguiente del nodo anterior
            } else {
                cabeza = actual->siguiente; // Si es el primer nodo, actualizamos la cabeza
            }

            if (actual->siguiente != nullptr) { // Si no es el último nodo
                actual->siguiente->anterior = actual->anterior; // Actualizamos el anterior del nodo siguiente
            } else {
                cola = actual->anterior; // Si es el último nodo, actualizamos la cola
            }

            std::cout << "Depuración: Eliminando nodo de publicación con ID: " << actual->publicacion.getId() << std::endl;
            delete actual; // Liberamos la memoria del nodo
            return; // Salimos del método
        }
        actual = actual->siguiente; // Avanzamos al siguiente nodo
    }

    std::cout << "No se encontró publicación con ID: " << id << std::endl; // Mensaje si no se encuentra el ID
}

void ListaDoblePublicacion::eliminarPublicacionesPorCorreo(const std::string& correo) {
    NodoPublicacion* actual = cabeza; // Comenzamos desde la cabeza de la lista

    while (actual != nullptr) {
        if (actual->publicacion.getCorreo() == correo) {
            NodoPublicacion* nodoAEliminar = actual;

            if (nodoAEliminar->anterior != nullptr) {
                nodoAEliminar->anterior->siguiente = nodoAEliminar->siguiente;
            } else {
                cabeza = nodoAEliminar->siguiente;
            }

            if (nodoAEliminar->siguiente != nullptr) {
                nodoAEliminar->siguiente->anterior = nodoAEliminar->anterior;
            } else {
                cola = nodoAEliminar->anterior;
            }

            actual = nodoAEliminar->siguiente;

            std::cout << "Depuración: Eliminando publicación con ID: " << nodoAEliminar->publicacion.getId() << " y correo: " << nodoAEliminar->publicacion.getCorreo() << std::endl;
            delete nodoAEliminar;
        } else {
            actual = actual->siguiente;
        }
    }
}


int ListaDoblePublicacion::obtenerNuevoId() const
{
    int maxId = 0;
    NodoPublicacion* actual = cabeza;
    while (actual != nullptr)
    {
        if (actual->publicacion.getId() > maxId)
        {
            maxId = actual->publicacion.getId();
        }
        actual = actual->siguiente;
    }
    return maxId + 1;
}


void ListaDoblePublicacion::generateDot(const std::string &filename) {
    std::string dotFilename = filename + ".dot";

    std::ofstream file(dotFilename);
    if (!file.is_open()) {
        std::cerr << "No se pudo abrir el archivo para escribir el DOT." << std::endl;
        return;
    }

    file << "digraph G {\n";
    file << "  node [shape=record];\n";

    NodoPublicacion* actual = cabeza;
    int nodoId = 0;

    while (actual != nullptr) {
        file << "  nodo" << nodoId << " [label=\"{"
             << actual->publicacion.getCorreo() << " | "
             << actual->publicacion.getContenido() << "}\"];\n";

        if (actual->siguiente != nullptr) {
            file << "  nodo" << nodoId << " -> nodo" << nodoId + 1 << ";\n";
            file << "  nodo" << nodoId + 1 << " -> nodo" << nodoId << ";\n"; // Doble enlace
        }

        actual = actual->siguiente;
        nodoId++;
    }

    file << "}\n";
    file.close();

    std::string pngFilename = filename + ".png";
    crearPNG(dotFilename, pngFilename);
}

void ListaDoblePublicacion::crearPNG(const std::string &dotFilename, const std::string &pngFilename) {
    std::string command = "dot -Tpng " + dotFilename + " -o " + pngFilename;
    int result = system(command.c_str());
    if (result != 0) {
        std::cerr << "Error al ejecutar el comando Graphviz para generar el PNG." << std::endl;
    }
}

std::vector<Publicacion> ListaDoblePublicacion::mostrarPublicacionesYAmigos(
    const std::string &correo,
    const MatrizDispersa &matriz,
    ArbolABB &arbol,
    const std::string &orden) {

    std::vector<Publicacion> publicaciones_arbol; // Vector para almacenar las publicaciones
    publicaciones_arbol.clear();
    try {
        arbol = ArbolABB();

        // Obtener la lista de amigos del usuario
        std::vector<std::string> amigos = matriz.obtenerAmigos(correo);
        amigos.push_back(correo); // Agregar el correo del usuario a la lista de amigos

        NodoPublicacion* actual = cabeza;
        while (actual) {
            // Comprobar si la publicación es del usuario o de un amigo
            if (std::find(amigos.begin(), amigos.end(), actual->publicacion.getCorreo()) != amigos.end()) {
                arbol.insertarPublicacion(actual->publicacion);
            }
            actual = actual->siguiente;
        }

        // Almacenar las publicaciones en el orden deseado
        if (orden == "PreOrder") {
            std::vector<Publicacion> publicaciones_temp;
            arbol.recorrerPreOrder(publicaciones_temp);
            publicaciones_arbol.swap(publicaciones_temp);
        } else if (orden == "InOrder") {
            std::vector<Publicacion> publicaciones_temp;
            arbol.recorrerInOrder(publicaciones_temp);
            publicaciones_arbol.swap(publicaciones_temp);
        } else if (orden == "PostOrder") {
            std::vector<Publicacion> publicaciones_temp;
            arbol.recorrerPostOrder(publicaciones_temp);
            publicaciones_arbol.swap(publicaciones_temp);
        } else {
            std::cerr << "Orden no válido: " << orden << std::endl;
        }
    } catch (const std::exception &e) {
        std::cerr << "Excepción capturada: " << e.what() << std::endl;
    }

    return publicaciones_arbol;
}

void ListaDoblePublicacion::mostrarPublicacionesPorCorreo(const std::string& correo) const
{
    NodoPublicacion *actual = cabeza;
    bool encontrado = false;

    while (actual != nullptr)
    {
        if (actual->publicacion.getCorreo() == correo)
        {
            std::cout << "ID: " << actual->publicacion.getId() << std::endl;
            std::cout << "Contenido: " << actual->publicacion.getContenido() << std::endl;
            std::cout << "Fecha: " << actual->publicacion.getFecha() << std::endl;
            std::cout << "Hora: " << actual->publicacion.getHora() << std::endl;
            std::cout << "--------------------------" << std::endl;
            encontrado = true;
        }
        actual = actual->siguiente;
    }

    if (!encontrado)
    {
        std::cout << "No se encontraron publicaciones para el usuario con correo: " << correo << std::endl;
    }
}

void ListaDoblePublicacion::mostrarPublicacion(const Publicacion& publicacion) const
{
    std::cout << "ID: " << publicacion.getId() << " - Correo: " << publicacion.getCorreo()
    << ", Contenido: " << publicacion.getContenido()
    << ", Fecha: " << publicacion.getFecha()
    << ", Hora: " << publicacion.getHora() << std::endl;

    // Mostrar comentarios de la publicación
}

void ListaDoblePublicacion::mostrarTodasLasPublicaciones() const {
    NodoPublicacion *actual = cabeza;
    while (actual != nullptr) {
        std::cout << "ID: " << actual->publicacion.getId()
        << " - Correo: " << actual->publicacion.getCorreo()
        << ", Contenido: " << actual->publicacion.getContenido()
        << ", Fecha: " << actual->publicacion.getFecha()
        << ", Hora: " << actual->publicacion.getHora()
        << std::endl;

        actual = actual->siguiente;
    }
}

void ListaDoblePublicacion::agregarPublicacion(const Publicacion &publicacion)
{
    NodoPublicacion *nuevoNodo = new NodoPublicacion(publicacion);

    if (cabeza == nullptr)
    {
        cabeza = nuevoNodo;
        cola = nuevoNodo;
    }
    else
    {
        cola->siguiente = nuevoNodo;
        nuevoNodo->anterior = cola;
        cola = nuevoNodo;
    }
}



ListaDoblePublicacion::NodoPublicacion::NodoPublicacion(const Publicacion &publicacion) : publicacion(publicacion), siguiente(nullptr), anterior(nullptr) {}
