/**
 * @file Album.hpp
 * @brief Header de la clase Album.
 */

#ifndef ALBUM_HPP
#define ALBUM_HPP

#include <string>

/**
 * @class Album
 * @brief Clase que representa a un album.
 */
class Album{
private:
  int id; ///< Id del album.
  std::string ruta; ///< Ruta del album.
  std::string nombre; ///< Nombre del album.
  int anio; ///< Año del album.

public:
  /**
   * @brief Constructor de la clase.
   * @param id El id de el album.
   * @param ruta La ruta del album.
   * @param nombre El nombre del album.
   * @param anio El año del album.
   */
  Album(int id, const std::string& ruta, const std::string& nombre, int anio);

  /**
   * @brief Regresa la id del album.
   * @return La id del album.
   */
  int getId() const;

  /**
   * @brief Regresa la ruta del album.
   * @return La ruta del album.
   */
  std::string getRuta() const;

  /**
   * @brief Regresa el nombre del album.
   * @return El nombre del album.
   */
  std::string getNombre() const;

  /**
   * @brief Regresa el año del album.
   * @return El año del album.
   */
  int getAnio() const;
};

#endif
