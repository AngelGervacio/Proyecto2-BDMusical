/**
 * @file Artista.cpp
 * @brief Implementación de la clase artista.
 */

#include "proyecto2/bd/Artista.hpp"

Artista::Artista(int id, int tipo, const std::string& nombre){
  this->id = id;
  this->tipo = tipo;
  this->nombre = nombre;
}

int Artista::getId(){
  return id;
}

int Artista::getTipo(){
  return tipo;
}

std::string Artista::getNombre(){
  return nombre;
}
