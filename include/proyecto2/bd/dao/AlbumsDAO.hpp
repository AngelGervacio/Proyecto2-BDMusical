/**
 * @file AlbumsDAO.hpp
 * @brief Header de la clase AlbumsDAO.
 */

#ifndef ALBUMSDAO_HPP
#define ALBUMSDAO_HPP

#include <sqlite3.h>
#include <filesystem>
#include <string>
#include <vector>
#include "proyecto2/bd/Metadatos.hpp"
#include "proyecto2/bd/Album.hpp"
#include "proyecto2/bd/Artista.hpp"
#include "proyecto2/bd/Banda.hpp"
#include "proyecto2/bd/Persona.hpp"
#include "proyecto2/bd/AdministraBD.hpp"

/**
 * @class AlbumsDAO
 * @brief Clase que implementa el patron de diseño Data Access Object
 * para interactuar con los álbums de la base de datos.
 */
class AlbumsDAO{
private:
  AdministraBD& admin; ///< Administra la base de datos.
  
public:
  /**
   * @brief Constructor de la clase.
   * @param admin El administrador de la base de datos.
   */
  AlbumsDAO(AdministraBD& admin);

  /**
   * @brief Regresa la id del album buscado, -1 si no lo encuentra.
   * @param ruta La ruta del album.
   * @return La id del album.
   */
  int obtenerAlbum(const std::string& ruta) const;

  /**
   * @brief Inserta un album en la base de datos y regresa la id asignada.
   * @param album El album a agregar.
   * @return La id asignada al album.
   */
  int insertarAlbum(const Album& album) const;

  /**
   * @brief Actualiza la información de un album.
   * @param album El album con la información actualizada.
   */
  void actualizarAlbum(const Album& album);

  /**
   * @brief Regresa los datos de un album.
   * @param id El id de el album.
   * @return Los datos de el album.
   */
  Album obtenerDatosAlbum(int id);

  /**
   * @brief Regresa el id de un album mediante una canción.
   * @param id El id de la canción.
   * @return El id de el album.
   */
  int obtenerIdAlbum(int id);

  /**
   * @brief Regresa un vector de albums.
   * @return Un vector de albums.
   */ 
  std::vector<Album> obtenerAlbums();

  /**
   * @brief Elimina un album de la base de datos.
   * @param id El id de el album a eliminar.
   */
  void eliminarAlbum(int id);
};

#endif
