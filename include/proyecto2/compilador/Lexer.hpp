/**
 * @file Lexer.hpp
 * @brief Header de la clase Lexer.
 */

#ifndef LEXER_HPP
#define LEXER_HPP

#include <string>
#include <list>
#include <ctype.h>
#include <stdexcept>
#include <QString>
#include <QChar>
#include "proyecto2/compilador/Token.hpp"

/**
 * @class Lexer
 * @brief Analiza una cadena recibida y genera una lista de tokens.
 */
class Lexer{
private:
  QString busqueda; ///< Busqueda a analizar.
  int posicion; ///< Posicion en la que se encuentra el lexer.
  std::list<Token> tokens; ///< Lista de tokens.

public:
  /**
   * @brief Constructor de la clase.
   * @param busqueda La cadena que se analizara.
   */
  Lexer(std::string busqueda);

  std::list<Token> getTokens() const;

  /**
   * @brief Analiza la cadena y genera la lista de tokens.
   */
  void analizarBusqueda();

  /**
   * @brief Analiza si el caracter en la posición actual y el siguiente son un operador doble.
   * @return true si es un operador doble, false en otro caso.
   */
  bool esOperadorDoble();

  /**
   * @brief Analiza si el caracter en la posición actual es un operador simple.
   * @return true si es un operador simple, false en otro caso.
   */
  bool esOperadorSimple();

  /**
   * @brief Lee una cadena a partir de la posición actual.
   * @return Un token de tipo cadena con el valor leído.
   */
  Token leerCadena();

  /**
   * @brief Lee un número a partir de la posición actual.
   * @return Un token de tipo número con el valor leído.
   */
  Token leerNumero();

  /**
   * @brief Lee un campo a partir de la posición actual.
   * @return Un token de tipo canpo con el valor leído.
   */
  Token leerCampo();
};

#endif
