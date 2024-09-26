#ifndef CREARPUBLICACION_H
#define CREARPUBLICACION_H

#include <QDialog>
#include "listaDoblePublicacion.h"

namespace Ui {
class CrearPublicacion;
}
class Usuarios;

class CrearPublicacion : public QDialog
{
    Q_OBJECT

public:
    explicit CrearPublicacion(std::string correoUsuario, ListaDoblePublicacion *listaDoblePublicacion, QWidget *parent = nullptr);
    ~CrearPublicacion();

private slots:
    void on_crearPublicacion_boton_clicked();
    void on_seleccionarImagen_boton_clicked();
    void on_cancelar_boton_clicked();

private:
    Ui::CrearPublicacion *ui;
    ListaDoblePublicacion *listaDoblePublicacion;
    std::string correoActualUsuario_;
    Usuarios *usuarioWindow;
    int idPublicacionActual_;
};

#endif // CREARPUBLICACION_H
