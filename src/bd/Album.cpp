/**
 * @file Album.cpp
 * @brief Implementación de la clase Album.
 */

#include "proyecto2/bd/Album.hpp"

Album::Album(int id, const std::string& ruta, const std::string& nombre, int anio){
  this->id = id;
  this->ruta = ruta;
  this->nombre = nombre;
  this->anio = anio;
}

int Album::getId() const{
  return id;
}

std::string Album::getRuta() const{
  return ruta;
}

std::string Album::getNombre() const{
  return nombre;
}

int Album::getAnio() const{
  return anio;
}
