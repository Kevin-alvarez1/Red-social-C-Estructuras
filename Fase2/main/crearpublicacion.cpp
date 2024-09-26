#include "crearpublicacion.h"
#include "ui_crearpublicacion.h"
#include <QMessageBox>
#include "usuarios.h"
#include <QFileDialog>
#include <QPixmap>
#include <QMessageBox>

CrearPublicacion::CrearPublicacion(std::string correoUsuario, ListaDoblePublicacion *listaDoblePublicacion, QWidget *parent)
    : QDialog(parent),
    correoActualUsuario_(correoUsuario),
    usuarioWindow(nullptr),
    listaDoblePublicacion(listaDoblePublicacion),
    idPublicacionActual_(-1)
{
    ui = new Ui::CrearPublicacion;
    ui->setupUi(this);
    ui->correo_publicacion_txt->setText(QString::fromStdString(correoActualUsuario_));

}


CrearPublicacion::~CrearPublicacion()
{
    delete ui;
}

void CrearPublicacion::on_crearPublicacion_boton_clicked()
{
    // Recuperar los datos del formulario
    QString contenido = ui->contenido_publicacion_txt->toPlainText();
    QString fecha = ui->fecha_publicacion_txt->text();
    QString hora = ui->hora_publicacion_txt->text();

    // Validar los datos
    if (contenido.isEmpty() || fecha.isEmpty() || hora.isEmpty()) {
        QMessageBox::warning(this, "Advertencia", "Por favor, complete todos los campos.");
        return;
    }

    // Crear una nueva publicación con un nuevo ID
    idPublicacionActual_ = listaDoblePublicacion->obtenerNuevoId();  // Obtener un nuevo ID único y almacenarlo
    Publicacion nuevaPublicacion(idPublicacionActual_, correoActualUsuario_, contenido.toStdString(), fecha.toStdString(), hora.toStdString());

    // Agregar la publicación a la lista de publicaciones
    listaDoblePublicacion->agregarPublicacion(nuevaPublicacion);

    // Informar al usuario
    QMessageBox::information(this, "Éxito", "Publicación creada exitosamente.");
}


void CrearPublicacion::on_seleccionarImagen_boton_clicked()
{
    // Verificar si ya se ha creado una publicación
    if (idPublicacionActual_ == -1) {
        QMessageBox::warning(this, tr("Advertencia"), tr("Primero debe crear una publicación antes de seleccionar una imagen."));
        return;
    }

    // Seleccionar la imagen
    QString fileName = QFileDialog::getOpenFileName(this, tr("Seleccionar Imagen"), "", tr("Archivos de Imagen (*.png *.jpg *.jpeg *.bmp)"));

    if (fileName.isEmpty()) {
        QMessageBox::warning(this, tr("Advertencia"), tr("No se seleccionó ninguna imagen."));
        return;
    }

    QPixmap imagen;
    if (!imagen.load(fileName)) {
        QMessageBox::warning(this, tr("Error"), tr("No se pudo cargar la imagen seleccionada."));
        return;
    }

    // Definir la ruta para guardar la imagen
    QDir directorioBase(QDir::currentPath());
    QString rutaImagenes = directorioBase.filePath("Imagenes");

    if (!QDir(rutaImagenes).exists()) {
        if (!QDir().mkpath(rutaImagenes)) {
            QMessageBox::warning(this, tr("Error"), tr("No se pudo crear el directorio de destino."));
            return;
        }
    }

    // Guardar la imagen con el ID de la publicación
    QString extension = QFileInfo(fileName).suffix();
    QString archivoDestino = QDir(rutaImagenes).filePath(QString::number(idPublicacionActual_) + "." + extension);

    // Sobrescribir la imagen si ya existe
    if (QFile::exists(archivoDestino)) {
        if (!QFile::remove(archivoDestino)) {
            QMessageBox::warning(this, tr("Error"), tr("No se pudo eliminar la imagen existente."));
            return;
        }
    }

    if (!QFile::copy(fileName, archivoDestino)) {
        QMessageBox::warning(this, tr("Error"), tr("No se pudo guardar la imagen en la carpeta destino."));
        return;
    }

    // Redimensionar y mostrar la imagen en el QLabel
    int width = ui->visualizador_imagenes->width();
    int height = ui->visualizador_imagenes->height();
    QPixmap imagenRedimensionada = imagen.scaled(width, height, Qt::KeepAspectRatio);

    // Mostrar la imagen en el QLabel
    ui->visualizador_imagenes->setPixmap(imagenRedimensionada);

    // Confirmación de guardado
    QMessageBox::information(this, tr("Éxito"), tr("La imagen se ha guardado correctamente con el ID de la publicación."));
}


void CrearPublicacion::on_cancelar_boton_clicked()
{
    if (parentWidget()) {
        parentWidget()->show();
    }
    this->close();
}


