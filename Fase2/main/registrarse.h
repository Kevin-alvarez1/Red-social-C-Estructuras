#ifndef REGISTRARSE_H
#define REGISTRARSE_H

#include <QDialog>
#include "ListaUsuarios.h"
#include "listaDoblePublicacion.h"
#include "lista_solicitudes.h"

class Login;

namespace Ui {
class Registrarse;
}

class Registrarse : public QDialog
{
    Q_OBJECT

public:
    explicit Registrarse(ListaUsuarios *listaUsuarios, ListaDoblePublicacion *listadoblepublicacion, ListaSolicitudes *lista_solicitudes, QWidget *parent = nullptr);
    ~Registrarse();

private slots:
    void on_Registrar_boton_clicked();

    void on_cancelar_boton_clicked();

private:
    Ui::Registrarse *ui;
    ListaUsuarios *listaUsuarios;
    ListaDoblePublicacion *listadoblepublicacion;
    ListaSolicitudes *lista_solicitudes;
    Login *login;
};

#endif // REGISTRARSE_H
