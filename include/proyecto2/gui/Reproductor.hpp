/**
 * @file Reproductor.hpp
 * @brief Header la clase Reproductor.hpp
 */

#ifndef REPRODUCTOR_HPP
#define REPRODUCTOR_HPP

#include <QObject>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QString>
#include <QUrl>
#include <QMetaObject>
#include <QCoreApplication>
#include <QTimer>
#include <algorithm>
#include <iostream>

/**
 * @class Reproductor
 * @brief Clase para reproducir archivos MP3.
 */
class Reproductor : public QObject{
  Q_OBJECT

  /**
   * @brief Indica si el reproductor cambio de posición.
   */
  Q_PROPERTY(qint64 posicion READ posicion NOTIFY cambioPosicion)

  /**
   * @brief Indica si cambio la duración.
   */
  Q_PROPERTY(qint64 duracion READ duracion NOTIFY cambioDuracion)

  /**
   * @brief Indica si cambio el volumen de la salida de el reproductor.
   */
  Q_PROPERTY(float volumen READ volumen WRITE cambiaVolumen NOTIFY cambioVolumen)

private:
  QMediaPlayer reproductor; ///< El reproductor.
  QAudioOutput salida; ///< La salida del reproductor.
  QUrl rutaSolicitada; ///< Ruta de el archivo a reproducir.
  bool hayRutaSolicitada; ///< Indica si se solicito reproducir un archivo.

  /**
   * @brief Procesa una ruta solicitada.
   */
  void procesarRutaSolicitada();

public:
  /**
   * @brief Constructor de la clase.
   * @param parent Objeto padre.
   */
  Reproductor(QObject* parent = nullptr);

  /**
   * @brief Regresa la posición de el reproductor.
   * @return La posición de el reproductor.
   */
  qint64 posicion() const;

  /**
   * @brief Regresa la duración de el archivo en reproducción.
   * @return La duración de el archivo en reproducción.
   */
  qint64 duracion() const;

  /**
   * @brief Regresa el volumen de la salida del reproductor.
   * @return El volumen de la salida del reproductor.
   */
  float volumen() const;

  /**
   * @brief Reproduce un archivo.
   * @param ruta La ruta de el archivo a reproducir.
   */
  Q_INVOKABLE void reproducir(const QString& ruta);

  /**
   * @brief Pausa la reproducción.
   */
  Q_INVOKABLE void pausar();

  /**
   * @brief Detiene la reproducción.
   */
  Q_INVOKABLE void detener();

  /**
   * @brief Cambia el volument de la salida del reproductor.
   * @param volumen El nuevo volumen.
   */
  Q_INVOKABLE void cambiaVolumen(float volumen);

  /**
   * @brief Cambia la posición del reproductor.
   * @param posicion La nueva posición del reproductor.
   */
  Q_INVOKABLE void cambiaPosicion(qint64 posicion);

signals:
  /**
   * @brief Notifica que el reproductor cambio de posición.
   */
  void cambioPosicion();

  /**
   * @brief Notifica que cambio la duración.
   */
  void cambioDuracion();

  /**
   * @brief Notifica que hubo un cambio de volumen.
   */
  void cambioVolumen();

  /**
   * @brief Notifica que finalizo la reproducción de una canción.
   */
  void finalizoCancion();

  /**
   * @brief Notifica que ocurrio un error durante la reproducción.
   * @param error El error ocurrido.
   */
  void error(QString error);
};

#endif
