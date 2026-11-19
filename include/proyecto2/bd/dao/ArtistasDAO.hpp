/**
 * @file ArtistasDAO.hpp
 * @brief Header de la clase ArtistasDAO.
 */

#ifndef ARTISTASDAO_HPP
#define ARTISTASDAO_HPP

#include <sqlite3.h>
#include <filesystem>
#include <string>
#include <vector>
#include "proyecto2/bd/Artista.hpp"
#include "proyecto2/bd/AdministraBD.hpp"

/**
 * @class ArtistasDAO
 * @brief Clase que implementa el patron de diseño Data Access Object
 * para interactuar con los artistas de la base de datos.
 */
class ArtistasDAO{
private:
      AdministraBD& admin; ///< Administra la base de datos.
  
public:
  /**
   * @brief Constructor de la clase.
   * @param admin El administrador de la base de datos.
   */
  ArtistasDAO(AdministraBD& admin);

  /**
   * @brief Regresa la id del artista buscado, -1 si no se encuentra.
   * @param artista El artista a buscar.
   * @return La id del artista.
   */
  int obtenerArtista(const std::string& artista) const;

  /**
   * @brief Inserta un artista en la base de datos y regresa la id asignada.
   * @param artista El artista a insertar.
   * @return La id asignada al artista.
   */
  int insertarArtista(const std::string& artista) const;

  /**
   * @brief Actualiza la información de un artista.
   * @param artista El artista con la información actualizada.
   */
  void actualizarArtista(Artista artista);

  /**
   * @brief Regresa la información de el artista de una canción.
   * @param id El id de la canción.
   * @return La información de el artista de la canción.
   */
  Artista obtenerDatosArtista(int id);

  /**
   * @brief Elimina a un artista de la base de datos.
   * @param id El id de el artista a eliminar.
   */
  void eliminarArtista(int id);
};

#endif
