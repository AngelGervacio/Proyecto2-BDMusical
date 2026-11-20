/**
 * @file ValidadorSemantico.hpp
 * @brief Header de la clase ValidadorSemantico.
 */

#ifndef VALIDADORSEMANTICO_HPP
#define VALIDADORSEMANTICO_HPP

#include "proyecto2/compilador/Token.hpp"
#include "proyecto2/compilador/Vertice.hpp"

class ValidadorSemantico{
private:
  /**
   * @brief Verifica que un vertice sea valido.
   * @param vertice El vertice a validar.
   */
  static void validarVertice(const Vertice& vertice);

  /**
   * @brief Verifica que una condición sea valida.
   * @param vertice El vertice que tiene a la condición.
   */
  static void validarCondicion(const Vertice& vertice);

  /**
   * @brief Regresa si un campo es de tipo texto.
   * @param campo El campo a verificar.
   * @return Si un campo es de tipo texto.
   */
  static bool esCampoTexto(const Token& campo) const;

  /**
   * @brief Regresa si un campo es de tipo numerico.
   * @param campo El campo a verificar.
   * @return Si un campo es de tipo numerico.
   */
  static bool esCampoNumerico(const Token& campo) const;

  /**
   * @brief Verifica que se tenga un operador valido.
   * @param campo El campo de la condición.
   * @param operador El operador de la condición.
   * @return true si es un operador valido, false en otro caso.
   */
  static bool operadorValido(const Token& campo, const Token& operador) const;
public:
  /**
   * @brief Verifica que una expresión sea valida.
   * @param raiz La raiz de la expresión a validar.
   */
  static void validar(const Vertice& raiz);
};

#endif
