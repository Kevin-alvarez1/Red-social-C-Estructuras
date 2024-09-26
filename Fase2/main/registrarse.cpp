#include "registrarse.h"
#include "ui_registrarse.h"
#include <QMessageBox>
#include "usuarios.h"
#include "./ui_usuarios.h"
#include "login.h"


Registrarse::Registrarse(ListaUsuarios *listaUsuarios, ListaDoblePublicacion *listadoblepublicacion, ListaSolicitudes *lista_solicitudes, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Registrarse),
    listaUsuarios(listaUsuarios),
    listadoblepublicacion(listadoblepublicacion),
    lista_solicitudes(lista_solicitudes),
    login(nullptr)

{
    ui->setupUi(this);
}

Registrarse::~Registrarse()
{
    delete ui;
}


void Registrarse::on_Registrar_boton_clicked() {
    // Convertir los campos de registro de QString a std::string
    std::string nombre = ui->nombre_registro_txt_area->text().toStdString();
    std::string apellido = ui->apellido_registro_txt_area->text().toStdString();
    std::string fecha_de_nacimiento = ui->fecha_nacimiento_registro->text().toStdString();
    std::string correo = ui->correo_registro_txt_area->text().toStdString();
    std::string contrasena = ui->contrasena_registro_txt_area->text().toStdString();
    std::string confirmacion_contrasena = ui->confirmacion_contrasena_txt_area->text().toStdString();

    // Verificar si algún campo está vacío
    if (nombre.empty() || apellido.empty() || fecha_de_nacimiento.empty() || correo.empty() || contrasena.empty() || confirmacion_contrasena.empty()) {
        QMessageBox::warning(this, "Error", "Todos los campos deben ser completados.");
        return;
    }

    // Verificar si las contraseñas coinciden
    if (contrasena != confirmacion_contrasena) {
        QMessageBox::warning(this, "Error", "Las contraseñas no coinciden.");
        return;
    }

    // Verificar si el usuario ya existe
    if (listaUsuarios->usuarioDuplicado(correo)) {
        QMessageBox::warning(this, "Error", "El usuario ya está registrado con ese correo.");
        return;
    }

    // Crear un nuevo usuario
    Usuario nuevoUsuario(nombre, apellido, fecha_de_nacimiento, correo, contrasena);

    // Agregar el nuevo usuario a la lista de usuarios
    listaUsuarios->agregarUsuario(nuevoUsuario);

    // Mostrar mensaje de éxito
    QMessageBox::information(this, "Registro Exitoso", "El usuario ha sido registrado con éxito.");

    if (!login) {
        login = new Login(listaUsuarios, listadoblepublicacion, lista_solicitudes, this);
    }
    login->show();
    this->hide();

    // Limpiar los campos de texto
    ui->nombre_registro_txt_area->clear();
    ui->apellido_registro_txt_area->clear();
    ui->fecha_nacimiento_registro->clear();
    ui->correo_registro_txt_area->clear();
    ui->contrasena_registro_txt_area->clear();
    ui->confirmacion_contrasena_txt_area->clear();
}


void Registrarse::on_cancelar_boton_clicked()
{
    if (parentWidget()) {
        parentWidget()->show();
    }
    this->close();
}

