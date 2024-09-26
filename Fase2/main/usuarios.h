#ifndef USUARIOS_H
#define USUARIOS_H

#include <QDialog>
#include "ListaUsuarios.h"
#include "listaDoblePublicacion.h"
#include "lista_solicitudes.h"
#include "pilareceptor.h"
#include "crearpublicacion.h"
#include <QComboBox>
#include <QLabel>
#include "arbolabb.h"
#include "publicacion.h"
#include <QVBoxLayout>

class CrearPublicacion;

namespace Ui {
class Usuarios;
}
class Login;
class Usuarios : public QDialog
{
    Q_OBJECT

public:
    explicit Usuarios(std::string correoUsuario, ListaUsuarios *listaUsuarios, ListaDoblePublicacion *listadoblepublicacion, ListaSolicitudes *lista_solicitudes, QWidget *parent = nullptr);
    ~Usuarios();
    ListaSolicitudes& obtenerListaSolicitudesEnviadas();
    ListaSolicitudes& obtenerListaSolicitudesRecibidas();
    std::vector<std::string> obtenerAmigos() const;
private slots:
    void on_cerrar_sesion_btn_clicked();
    void on_buscar_correo_btn_clicked();
    void on_Eliminar_boton_clicked();
    void on_Modificar_boton_clicked();
    void on_btnEnviarSolicitud_clicked(const std::string& correo);
    void on_btnCancelar_clicked(const std::string& correo);
    void on_actualizar_tablas_clicked();
    void on_btnAceptar_clicked(const std::string& correo);
    void on_btnRechazar_clicked(const std::string& correo);
    void on_fecha_filtro_publis_boton_clicked();
    void on_crear_nueva_publi_boton_clicked();
    void on_aplicar_orden_publis_boton_clicked();
    void on_generar_bst_reporte_boton_clicked();
    void actualizarPanelConImagen(const QString& imagePath);
    void mostrarPublicacionesFiltradasPorFecha(const QString& fechaSeleccionada);
    void llenarComboBoxFechas();
    void llenarComboBoxFechas_BST();
    void mostrarComentariosDePublicacion(const Publicacion& publicacion);
    void llenarFechasCantidad(NodoABB* nodo, std::vector<std::pair<std::string, int>>& fechasCantidad);
    void obtenerCantidadComentariosDePublicaciones(NodoABB* nodo, std::vector<std::pair<int, int>>& publisConMasComentarios);
    void on_generar_reportes_usuario_boton_clicked();
    void obtenerDetallesComentariosDePublicaciones(NodoABB* nodo, std::vector<std::tuple<std::string, std::string, int>>& publisConMasComentarios);
private:
    Ui::Usuarios *ui;
    Login *login;
    std::string correoActualUsuario_;
    ListaUsuarios *listaUsuarios;
    ListaDoblePublicacion *listadoblepublicacion;
    ListaSolicitudes *lista_solicitudes;
    CrearPublicacion *CrearPublicacion;
    QFrame* publicaciones_frame;
};

class VentanaArbolComentarios : public QDialog {
    Q_OBJECT

public:
    VentanaArbolComentarios(QWidget *parent = nullptr) : QDialog(parent) {
        setWindowTitle("Árbol de Comentarios");
        setFixedSize(1280, 720); // Ajusta el tamaño según sea necesario

        // Layout para la ventana
        QVBoxLayout* layout = new QVBoxLayout(this);

        QLabel* imageLabel = new QLabel(this);
        layout->addWidget(imageLabel);

        // Cargar la imagen generada
        QPixmap pixmap("arbol_comentarios.png");
        imageLabel->setPixmap(pixmap);
        imageLabel->setScaledContents(true); // Para ajustar la imagen al tamaño del QLabel
    }
};

#endif // USUARIOS_H
