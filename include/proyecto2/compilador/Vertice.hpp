/**
 * @file Vertice.hpp
 * @brief Header de la clase Vertice.
 */

#ifndef VERTICE_HPP
#define VERTICE_HPP

#include <memory>
#include <utility>
#include <stdexcept>
#include "proyecto2/compilador/Token.hpp"

/**
 * @class Vertice
 * @brief Representa un vertice de un arbol.
 */
class Vertice{
private:
  Token token; ///< Token del vertice.
  std::unique_ptr<Vertice> izquierdo; ///< Vertice izquierdo.
  std::unique_ptr<Vertice> derecho; ///< Vertice derecho.
  
public:
  /**
   * @brief Constructor de la clase.
   * @param token El token del vertice.
   * @param izquierdo El vertice izquierdo.
   * @param derecho El vertice derecho.
   */
  Vertice(Token token, std::unique_ptr<Vertice> izquierdo = nullptr, std::unique_ptr<Vertice> derecho = nullptr);

  /**
   * @brief Regresa el token del vertice.
   * @return El token del vertice.
   */
  Token getToken() const;

  /**
   * @brief Regresa si hay un vertice izquierdo.
   * @return Si hay un vertice izquierdo.
   */
  bool hayIzquierdo() const;

  /**
   * @brief Regresa si hay un vertice derecho.
   * @return Si hay un vertice derecho.
   */
  bool hayDerecho() const;

  /**
   * @brief Regresa el vertice izquierdo.
   * @return El vertice izquierdo.
   */
  const Vertice& getIzquierdo() const;

  /**
   * @brief Regresa el vertice derecho.
   * @return El vertice derecho.
   */
  const Vertice& getDerecho() const;
};

#endif
