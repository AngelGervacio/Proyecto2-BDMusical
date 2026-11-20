/**
 * @file Parser.hpp
 * @brief Header de la clase Parser.
 */

#ifndef PARSER_HPP
#define PARSER_HPP

#include <list>
#include <memory>
#include "proyecto2/compilador/Token.hpp"
#include "proyecto2/compilador/Vertice.hpp"

class Parser{
private:
  const std::list<Token>& tokens; ///< Lista de tokens.
  std::list<Token>::const_iterator actual; ///< Iterador de la lista de tokens.

  /**
   * @brief Analiza las expresiones de tipo OR.
   * @return Vertice raiz de la expresión.
   */
  std::unique_ptr<Vertice> parsearOR();

  /**
   * @brief Analiza las expresiones de tipo AND.
   * @return Vertice raiz de la expresión.
   */
  std::unique_ptr<Vertice> parsearAND();

  /**
   * @brief Analiza las expresiones con condiciones.
   * @return Vertice raiz de la expresión.
   */
  std::unique_ptr<Vertice> parsearCondicion();

  /**
   * @brief Regresa el token actual y avanza al siguiente.
   * @return El token actual.
   */
  Token avanzarToken();
public:
  /**
   * @brief Constructor de la clase.
   * @param tokens La lista de tokens a parsear.
   */
  Parser(const std::list<Token>& tokens);

  /**
   * @brief Parsea la lista de tokens.
   * @return Vertice raiz.
   */
  std::unique_ptr<Vertice> parsear();
};

#endif
