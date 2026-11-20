/**
 * @file Compilador.hpp
 * @brief Header de la clase Compilador.
 */

#ifndef COMPILADOR_HPP
#define COMPILADOR_HPP

#include <string>
#include <list>
#include <memory>
#include "proyecto2/compilador/Lexer.hpp"
#include "proyecto2/compilador/Parser.hpp"
#include "proyecto2/compilador/ValidadorSemantico.hpp"
#include "proyecto2/compilador/Generador.hpp"
#include "proyecto2/compilador/Token.hpp"
#include "proyecto2/compilador/Vertice.hpp"

/**
 * @class Compilador
 * @brief Clase que compila una busqueda a una consulta SQL.
 */
class Compilador {
public:
  /**
   * @brief Compila una busqueda a una consulta SQL.
   * @param busqueda La busqueda realizada.
   * @return La consulta SQL y sus parametros.
   */
  Resultado compilar(const std::string& busqueda);
};

#endif
