#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "proyecto2/bd/AdministraBD.hpp"
#include "proyecto2/bd/dao/CancionesDAO.hpp"
#include "proyecto2/bd/dao/ArtistasDAO.hpp"
#include "proyecto2/bd/dao/AlbumsDAO.hpp"
#include "proyecto2/bd/dao/BandasDAO.hpp"
#include "proyecto2/bd/dao/PersonasDAO.hpp"
#include "proyecto2/gui/CancionesModelo.hpp"
#include "proyecto2/gui/Controlador.hpp"
#include "proyecto2/gui/Reproductor.hpp"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    AdministraBD admin;
    CancionesDAO canciones(admin);
    ArtistasDAO artistas(admin);
    AlbumsDAO albums(admin);
    BandasDAO bandas(admin);
    PersonasDAO personas(admin);
    CancionesModelo modelo;
    Controlador controlador(canciones, artistas, albums, bandas, personas, modelo);
    Reproductor reproductor;

    admin.abrir();
    controlador.cargarCanciones();

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("cancionesModelo", &modelo);
    engine.rootContext()->setContextProperty("controlador", &controlador);
    engine.rootContext()->setContextProperty("reproductor", &reproductor);
    
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    return QGuiApplication::exec();
}
