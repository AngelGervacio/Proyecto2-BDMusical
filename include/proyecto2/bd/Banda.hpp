/**
 * @file Banda.hpp
 * @brief Header de la clase Banda.
 */

#ifndef BANDA_HPP
#define BANDA_HPP

#include <string>

/**
 * @class Banda
 * @brief Clase que representa a una banda.
 */
class Banda{
private:
  int id; ///< Id de la banda.
  std::string nombre; ///< Nombre de la banda.
  std::string formacion; ///< Fecha de formación de la banda.
  std::string separacion; ///< Fecha de separación de la banda.

public:
  /**
   * @brief Constructor de la clase.
   * @param id El id de la banda.
   * @param nombre El nombre de la banda.
   * @param formacion La fecha de formación de la banda.
   * @param separacion La fecha de separación de la banda.
   */
  Banda(int id, const std::string& nombre, const std::string& formacion, const std::string& separacion);

  /**
   * @brief Regresa el id de la banda.
   * @return El id de la banda.
   */
  int getId() const;

  /**
   * @brief Regresa el nombre de la banda.
   * @return El nombre de la banda.
   */
  std::string getNombre() const;

  /**
   * @brief Regresa la fecha de formación de la banda.
   * @return La fecha de formación de la banda.
   */
  std::string getFormacion() const;

  /**
   * @brief Regresa la fecha de separación de la banda.
   * @return La fecha de separación de la banda.
   */
  std::string getSeparacion() const;
  
};

#endif
