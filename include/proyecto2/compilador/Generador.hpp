/**
 * @file Generador.hpp
 * @brief Header de la clase Generador.hpp
 */

#ifndef GENERADOR_HPP
#define GENERADOR_HPP

#include <string>
#include <vector>
#include "proyecto2/compilador/Token.hpp"
#include "proyecto2/compilador/Vertice.hpp"

/**
 * @brief Estructura que contiene una consulta y sus parametros.
 */
struct Resultado{
  std::string consulta; ///< Consulta generada.
  std::vector<std::string> parametros; ///< Parametros de la consulta.
};

/**
 * @class Generador
 * @brief Clase que genera la consulta SQL y obtiene sus parametros.
 */
class Generador{
private:
  /**
   * @brief Genera la consulta SQL a partir de un vertice.
   * @param vertice El vertice de donde se generara.
   * @param resultado El resultado actual.
   */
  void generarVertice(const Vertice& vertice, Resultado& resultado);

  /**
   * @brief Genera la consulta SQL a partir de una condición.
   * @param vertice El vertice raiz de la condición.
   * @param resultado El resultado actual.
   */
  void generarCondicion(const Vertice& vertice, Resultado& resultado);

  /**
   * @brief Regresa el string de un campo.
   * @param campo El campo que se busca.
   * @return El string de un campo.
   */
  std::string obtenerCampo(const Token& campo) const;

  /**
   * @brief Regresa el string de un operador.
   * @param operador El operador que se busca.
   * @return El string de un operador.
   */
  std::string obtenerOperador(const Token& operador) const;
public:
  /**
   * @brief Genera una consulta SQL con parametros.
   * @param raiz El vertice raiz de la consulta.
   * @return La consulta SQL con parametros.
   */
  Resultado generar(const Vertice& raiz);
};
