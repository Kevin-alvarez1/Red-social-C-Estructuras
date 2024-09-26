#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include "ListaUsuarios.h"
#include "listaDoblePublicacion.h"
#include "lista_solicitudes.h"

class Admin;
class Usuarios;
class Registrarse;

namespace Ui {
class Login;
}

class Login : public QMainWindow
{
    Q_OBJECT

public:
    explicit Login(ListaUsuarios *listaUsuarios, ListaDoblePublicacion *listadoblepublicacion, ListaSolicitudes *lista_solicitudes, QWidget *parent = nullptr);
    ~Login();

private slots:
    void on_InicioSesion_btn_clicked();
    void on_Registrarse_btn_clicked();

private:
    Ui::Login *ui;
    Admin *adminWindow;
    Usuarios *usuarioWindow;
    Registrarse *registrarseWindow;
    ListaUsuarios *listaUsuarios;
    ListaDoblePublicacion *listadoblepublicacion;
    ListaSolicitudes *lista_solicitudes;
};

#endif // LOGIN_H
