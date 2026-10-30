/**
 * @file BaseDatosDAO.hpp
 * @brief Header de la clase BaseDatosDao.
 */

#ifndef BASEDATOSDAO_HPP
#define BASEDATOSDAO_HPP

#include <sqlite3.h>
#include <filesystem>
#include <string>
#include "proyecto2/bd/Metadatos.hpp"
#include "proyecto2/bd/AdministraBD.hpp"

/**
 * @class BaseDatosDAO
 * @brief Clase que implementa el patro de diseño Data Access Object
 * para interactuar con la base de datos.
 */
class BaseDatosDAO{
private:
  AdministraBD& admin; /*!< Administra base de datos > */
  
public:
  /**
   * @brief Constructor de la clase.
   * @param admin El administrador de la base de datos.
   */
  BaseDatosDAO(AdministraBD& admin);
  
  /**
   * @brief Regresa true si ya hay una canción con la ruta,
   * false en otro caso.
   * @param cancion La ruta de la cancion.
   * @return si ya hay una canción con la ruta.
   */
  bool existeRuta(const std::filesystem::path& cancion) const;

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
   * @brief Regresa la id del album buscado, -1 si no lo encuentra.
   * @param ruta La ruta del album.
   * @return La id del album.
   */
  int obtenerAlbum(const std::string& ruta) const;

  /**
   * @brief Inserta un album en la base de datos y regresa la id asignada.
   * @param ruta La ruta del album.
   * @param nombre El nombre del album.
   * @param año El año del album.
   * @return La id asignada al album.
   */
  int insertarAlbum(const std::string& ruta, const std::string& nombre, int año) const;

  /**
   * @brief Inserta una canción a la base de datos.
   * @param datos Los datos de la canción.
   */
  void insertarCancion(const Metadatos& datos) const;
};

#endif
