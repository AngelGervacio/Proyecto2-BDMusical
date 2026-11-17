/**
 * @file Reproductor.cpp
 * @brief Implementación de la clase Reproductor.
 */

#include "proyecto2/gui/Reproductor.hpp"

Reproductor::Reproductor(QObject* parent) : QObject(parent){
  reproductor.setAudioOutput(&salida);
  
  connect(&reproductor, &QMediaPlayer::playbackStateChanged, this, [this](QMediaPlayer::PlaybackState estado){
    if(estado == QMediaPlayer::StoppedState && hayRutaSolicitada){
      procesarRutaSolicitada();
    }
  });
  
  connect(&reproductor, &QMediaPlayer::positionChanged, this, [this](qint64) {
    emit cambioPosicion();
  });
  
  connect(&reproductor, &QMediaPlayer::durationChanged, this, [this](qint64) {
    emit cambioDuracion();
  });
  
  connect(&reproductor, &QMediaPlayer::mediaStatusChanged, this, [this](QMediaPlayer::MediaStatus estatus) {
    if (estatus == QMediaPlayer::EndOfMedia) {
      emit finalizoCancion();
    }
  });
}

qint64 Reproductor::posicion() const{
  return reproductor.position();
}

qint64 Reproductor::duracion() const{
  return reproductor.duration();
}

float Reproductor::volumen() const{
  return salida.volume() * 100.0f;
}

void Reproductor::procesarRutaSolicitada(){
  if(!hayRutaSolicitada)
    return;

  if(reproductor.playbackState() != QMediaPlayer::StoppedState){
    return;
  }

  hayRutaSolicitada = false;

  reproductor.setSource(rutaSolicitada);
  reproductor.play();
}

void Reproductor::reproducir(const QString& ruta){
  QUrl rutaNueva = QUrl::fromLocalFile(ruta);
  
  if(reproductor.source() == rutaNueva){
    reproductor.play();
    return;
  }
  
  rutaSolicitada = rutaNueva;
  hayRutaSolicitada = true;
  
  if(reproductor.playbackState() != QMediaPlayer::StoppedState){
    reproductor.pause();
    reproductor.stop();
    return;
  }
  
  procesarRutaSolicitada();
}

void Reproductor::pausar(){
  reproductor.pause();
}

void Reproductor::detener(){
  reproductor.stop();
}

void Reproductor::cambiaVolumen(float volumen){
  volumen = std::clamp(volumen, 0.0f, 100.0f);
  salida.setVolume(volumen/100.0f);
  emit cambioVolumen();
}

void Reproductor::cambiaPosicion(qint64 posicion){
  reproductor.setPosition(posicion);
}
