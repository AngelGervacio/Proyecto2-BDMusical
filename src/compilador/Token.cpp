/**
 * @file Token.cpp
 * @brief Implementación de la clase Token.hpp
 */

#include "proyecto2/compilador/Token.hpp"

Token::Token(TipoToken tipo, std::string valor){
  this->tipo = tipo;
  this->valor = valor;
}

TipoToken Token::getTipo() const{
  return tipo;
}

std::string Token::getValor() const{
  return valor;
}
