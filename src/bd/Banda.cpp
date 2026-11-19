/**
 * @file Banda.cpp
 * @brief Implementación de la clase Banda.
 */

#include "proyecto2/bd/Banda.hpp"

Banda::Banda(int id, const std::string& nombre, const std::string& formacion, const std::string& separacion){
  this->id = id;
  this->nombre = nombre;
  this->formacion = formacion;
  this->separacion = separacion;
}

int Banda::getId() const{
  return id;
}

std::string Banda::getNombre() const{
  return nombre;
}

std::string Banda::getFormacion() const{
  return formacion;
}

std::string Banda::getSeparacion() const{
  return separacion;
}
