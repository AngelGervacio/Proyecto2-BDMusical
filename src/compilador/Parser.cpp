/**
 * @file Parser.cpp
 * @brief Implementación de la clase Parser.
 */

#include "proyecto2/compilador/Parser.hpp"

Parser::Parser(const std::list<Token>& tokens){
  this->tokens = tokens;
  actual = this->tokens.begin();
}

std::unique_ptr<Vertice> Parser::parsear(){
  std::unique_ptr<Vertice> raiz = parsearOR();

  if(actual != tokens.end())
    throw std::invalid_argument("No se pudo realizar la busqueda: busqueda invalida.");
  
  return raiz;
}

std::unique_ptr<Vertice> Parser::parsearOR(){
  std::unique_ptr<Vertice> izquierdo = parsearAND();
  
  while(actual != tokens.end() && actual->getTipo() == TipoToken::OR){
    Token operador = avanzarToken();
    std::unique_ptr<Vertice> derecho = parsearAND();
    izquierdo = std::make_unique<Vertice>(operador, std::move(izquierdo), std::move(derecho));
  }

  return izquierdo;
}

std::unique_ptr<Vertice> Parser::parsearAND(){
  std::unique_ptr<Vertice> izquierdo = parsearCondicion();
    
  while(actual != tokens.end() && actual->getTipo() == TipoToken::AND){
    Token operador = avanzarToken();
    std::unique_ptr<Vertice> derecho = parsearCondicion();
    izquierdo = std::make_unique<Vertice>(operador, std::move(izquierdo), std::move(derecho));
  }

  return izquierdo;
}

std::unique_ptr<Vertice> Parser::parsearCondicion(){
  Token campo = avanzarToken();
  Token operador = avanzarToken();
  Token valor = avanzarToken();

  std::unique_ptr<Vertice> izquierdo = std::make_unique<Vertice>(campo);
  std::unique_ptr<Vertice> derecho = std::make_unique<Vertice>(valor);

  return std::make_unique<Vertice>(operador, std::move(izquierdo), std::move(derecho));
}

Token Parser::avanzarToken(){
  if(actual == tokens.end())
    throw std::invalid_argument("No se pudo realizar la busqueda: busqueda invalida.");

  Token token = *actual;
  ++actual;
  return token;
}
