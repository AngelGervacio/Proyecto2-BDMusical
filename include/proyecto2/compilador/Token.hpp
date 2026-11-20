/**
 * @file Token.hpp
 * @brief Header de la clase Token.hpp
 */

#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>
#include "proyecto2/compilador/TipoToken.hpp"

/**
 * @class Token
 * @brief Representa un token en el análisis léxico.
 */
class Token{
  TipoToken tipo; ///< El tipo de token.
  std::string valor; ///< El valor del token.
  
public:
  /**
   * @brief Constructor de la clase.
   * @param tipo El tipo de token.
   * @param valor El valor del token.
   */
  Token(TipoToken tipo, std::string valor);

  /**
   * @brief Regresa el tipo de token.
   * @return El tipo de token.
   */
  TipoToken getTipo() const;

  /**
   * @brief Regresa el valor del token.
   * @return El valor del token.
   */
  std::string getValor() const;
};

#endif
