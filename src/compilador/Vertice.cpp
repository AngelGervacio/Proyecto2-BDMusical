/**
 * @file Vertice.cpp
 * @brief Implementación de la clase Vertice.
 */

#include "proyecto2/compilador/Vertice.hpp"

Vertice::Vertice(Token token, std::unique_ptr<Vertice> izquierdo, std::unique_ptr<Vertice> derecho) :
  token(std::move(token)),
  izquierdo(std::move(izquierdo)),
  derecho(std::move(derecho)){}

Token Vertice::getToken() const{
  return token;
}

bool Vertice::hayIzquierdo() const{
  return izquierdo != nullptr;
}

bool Vertice::hayDerecho() const{
  return derecho != nullptr;
}

const Vertice& Vertice::getIzquierdo() const{
  if(izquierdo == nullptr)
    throw std::logic_error("El vertice no tiene hijo izquierdo.");

  return *izquierdo;
}

const Vertice& Vertice::getDerecho() const{
  if(derecho == nullptr)
    throw std::logic_error("El vertice no tiene hijo derecho.");

  return *derecho;  
}

void Vertice::setIzquierdo(std::unique_ptr<Vertice> izquierdo){
  this->izquierdo = std::move(izquierdo);
}

void Vertice::setDerecho(std::unique_ptr<Vertice> derecho){
  this->derecho = std::move(derecho);
}
