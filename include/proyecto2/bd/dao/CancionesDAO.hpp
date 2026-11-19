/**
  * @file CancionesDAO.hpp
  * @brief Header de la clase CancionesDAO.
  */

#ifndef CANCIONESDAO_HPP
#define CANCIONESDAO_HPP

#include <sqlite3.h>
#include <filesystem>
#include <string>
#include <vector>
#include "proyecto2/bd/Metadatos.hpp"
#include "proyecto2/bd/AdministraBD.hpp"


/**
 * @class CancionesDAO
 * @brief Clase que implementa el patron de diseño Data Access Object
 * para interactuar con las canciones de la base de datos.
 */
class CancionesDAO{
private:
  AdministraBD& admin; ///< Administra la base de datos.
  
public:
  /**
   * @brief Constructor de la clase.
   * @param admin El administrador de la base de datos.
   */
  CancionesDAO(AdministraBD& admin);
  
  /**
   * @brief Regresa true si ya hay una canción con la ruta,
   * false en otro caso.
   * @param cancion La ruta de la cancion.
   * @return si ya hay una canción con la ruta.
   */
  bool existeCancion(const std::filesystem::path& cancion) const;

  /**
   * @brief Inserta una canción a la base de datos.
   * @param datos Los datos de la canción.
   * @param idArtista El id de el artista de la canción.
   * @param idAlbum El id de el album de la canción.
   * @return La id asignada a la canción en la base de datos.
   */
  int insertarCancion(const Metadatos& datos, int idArtista, int idAlbum) const;

  /**
   * @brief Actualiza la información de una canción.
   * @param datos Los datos de la canción.
   * @param idArtista El id de el nuevo artista de la canción.
   * @param idAlbum El id de el nuevo album de la canción.
   */
  void actualizarCancion(const Metadatos& datos, int idArtista, int idAlbum);

  /**
   * @brief Regresa un vector con los metadatos de las canciones.
   * @return Un vector con los metadatos de las canciones.
   */
  std::vector<Metadatos> obtenerCanciones();

  /**
   * @brief Regresa la ruta de una canción.
   * @param id El id de la canción.
   * @return La ruta de la canción.
   */
  std::string obtenerRuta(int id) const;

  /**
   * @brief Actualiza el album de una canción.
   * @param idCancion El id de la canción a la cual se le actualizara su album.
   * @param idAlbum El nuevo album de la canción.
   */
  void actualizarAlbumEnCancion(int idCancion, int idAlbum);

  /**
   * @brief Actualiza el artista de una canción.
   * @param idCancion El id de la canción a actualizar.
   * @param idArtista El id de el nuevo artista de la canción.
   */
  void actualizarArtistaEnCancion(int idCancion, int idArtista);

  /**
   * @brief Regresa la información de una canción.
   * @param id El id de la canción.
   * @return La información de la canción.
   */
  Metadatos obtenerDatosCancion(int id);

  /**
   * @brief Elimina una canción de la base de datos.
   * @param id El id de la canción a eliminar.
   */
  void eliminarCancion(int id);

  /**
   * @brief Regresa las ids de las canciones de un artista.
   * @param idArtista El id del artista.
   * @return Las ids de las canciones de el artista.
   */
  std::vector<int> cancionesDeArtista(int idArtista);

  /**
   * @brief Regresa las ids de las canciones de un album.
   * @param idAlbum El id del album.
   * @return Las ids de las canciones de el album.
   */
  std::vector<int> cancionesDeAlbum(int idAlbum);
};

#endif
