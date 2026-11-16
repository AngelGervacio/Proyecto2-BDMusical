/**
 * @file Persona.cpp
 * @brief Implementación de la clase Persona.
 */

#include "proyecto2/bd/Persona.hpp"

Persona::Persona(int id, const std::string& artistico, const std::string& real,
		 const std::string& nacimiento, const std::string& fallecimiento){
  this->id = id;
  this->artistico = artistico;
  this->real = real;
  this->nacimiento = nacimiento;
  this->fallecimiento = fallecimiento;
}

int Persona::getId() const{
  return id;
}

std::string Persona::getArtistico() const{
  return artistico;
}

std::string Persona::getReal() const{
  return real;
}

std::string Persona::getNacimiento() const{
  return nacimiento;
}

std::string Persona::getFallecimiento() const{
  return fallecimiento;
}
