#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "proyecto2/bd/AdministraBD.hpp"
#include "proyecto2/bd/BaseDatosDAO.hpp"
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
    BaseDatosDAO dao(admin);
    CancionesModelo modelo;
    Controlador controlador(dao, modelo);
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
