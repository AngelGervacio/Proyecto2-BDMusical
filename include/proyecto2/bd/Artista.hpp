/**
 * @file Artista.hpp
 * @brief Header de la clase Artista.
 */

#ifndef ARTISTA_HPP
#define ARTISTA_HPP

#include <string>

/**
 * @class Artista
 * @brief Clase que representa a un artista.
 */
class Artista{
private:
  int id; ///< Id del artista.
  int tipo; ///< Tipo de artista.
  std::string nombre; ///< Nombre del artista.

public:
  /**
   * @brief Constructor de la clase artista.
   * @param id Id del artista.
   * @param tipo El tipo de artista.
   * @param nombre El nombre del artista.
   */
  Artista(int id, int tipo, const std::string& nombre);

  /**
   * @brief Regresa el id del artista.
   * @return El id del artista.
   */
  int getId();

  /**
   * @brief Regresa el tipo de artista.
   * @return El tipo de artista.
   */
  int getTipo();

  /**
   * @brief Regresa el nombre del artista.
   * @return El nombre del artista.
   */
  std::string getNombre();
};

#endif
