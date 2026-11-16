/**
 * @file BaseDatosDAO.hpp
 * @brief Header de la clase BaseDatosDao.
 */

#ifndef BASEDATOSDAO_HPP
#define BASEDATOSDAO_HPP

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
 * @class BaseDatosDAO
 * @brief Clase que implementa el patro de diseño Data Access Object
 * para interactuar con la base de datos.
 */
class BaseDatosDAO{
private:
  AdministraBD& admin; ///< Administra base de datos.
  
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
   * @return La id asignada a la canción en la base de datos.
   */
  int insertarCancion(const Metadatos& datos) const;

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
   * @brief Regresa el id de una persona mediante su nombre artistico.
   * @param artistico El nombre artistico de la persona.
   * @return El id de la persona.
   */
  int obtenerPersona(const std::string& artistico) const;

  /**
   * @brief Inserta una persona a la base de datos.
   * @param artistico El nombre artistico de la persona.
   * @param real El nombre real de la persona.
   * @param nacimiento La fecha de nacimiento de la persona.
   * @param fallecimiento La fecha de fallecimiento de la persona.
   * @return El id asignado a la persona insertada.
   */
  int insertarPersona(const std::string& artistico, const std::string& real, const std::string& nacimiento,
		      const std::string& fallecimiento);

  /**
   * @brief Actualiza la información de una persona.
   * @param id El id de la persona a actualizar.
   * @param artistico El nuevo nombre artistico.
   * @param real El nuevo nombre real.
   * @param nacimiento La fecha de nacimiento actualizada.
   * @param fallecimiento La fecha de fallecimiento actualizada.
   */
  void actualizarPersona(int id, const std::string& artistico, const std::string& real, const std::string& nacimiento,
			 const std::string& fallecimiento);

  /**
   * @brief Regresa los datos de un album.
   * @param id El id de el album.
   * @return Los datos de el album.
   */
  Album obtenerDatosAlbum(int id);

  /**
   * @brief Actualiza el album de una canción.
   * @param idCancion El id de la canción a la cual se le actualizara su album.
   * @param idAlbum El nuevo album de la canción.
   */
  void actualizarAlbumEnCancion(int idCancion, int idAlbum);

  /**
   * @brief Actualiza la información de un album.
   * @param ruta La nueva ruta del album.
   * @param nombre El nuevo nombre del album.
   * @param año El nuevo año del album.
   * @param id El id del album a actualizar.
   */
  void actualizarAlbum(const std::string& ruta, const std::string& nombre, int año, int id);

  /**
   * @brief Elimina un album de la base de datos.
   * @param id El id de el album a eliminar.
   */
  void eliminarAlbum(int id);

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
   * @brief Actualiza la información de una canción.
   * @param id El id de la canción a actualizar.
   * @param ruta La nueva ruta de la canción.
   * @param titulo El nuevo titulo de la canción.
   * @param artista El nuevo artista de la canción.
   * @param idAlbum El nuevo album de la canción.
   * @param pista El nuevo número de pista de la canción.
   * @param anio El nuevo año de la canción.
   * @param genero El nuevo genero de la canción.
   */
  void actualizarCancion(int id, std::string ruta, std::string titulo, std::string artista,
			 int idAlbum, int pista, int anio, std::string genero);

  /**
   * @brief Regresa un vector de albums.
   * @return Un vector de albums.
   */ 
  std::vector<Album> obtenerAlbums();

  /**
   * @brief Regresa el id de un album mediante una canción.
   * @param id El id de la canción.
   * @return El id de el album.
   */
  int obtenerIdAlbum(int id);

  /**
   * @brief Regresa la información de el artista de una canción.
   * @param id El id de la canción.
   * @return La información de el artista de la canción.
   */
  Artista obtenerDatosArtista(int id);

  /**
   * @brief Actualiza la información de un artista.
   * @param idArtista El id de el artista a actualizar.
   * @param tipo El tipo de artista.
   * @param nombre El nombre de el artista.
   */
  void actualizarArtista(int idArtista, int tipo, const std::string& nombre);

  /**
   * @brief Actualiza el artista de una canción.
   * @param idCancion El id de la canción a actualizar.
   * @param idArtista El id de el nuevo artista de la canción.
   */
  void actualizarArtistaEnCancion(int idCancion, int idArtista);

  /**
   * @brief Elimina a un artista de la base de datos.
   * @param id El id de el artista a eliminar.
   */
  void eliminarArtista(int id);

  /**
   * @brief Regresa la información de una banda.
   * @param nombre El nombre de la banda.
   * @return La información de una banda.
   */
  Banda obtenerDatosBanda(const std::string& nombre);

  /**
   * @brief Actualiza la información de una banda.
   * @param id El id de la banda a actualizar.
   * @param nombre El nuevo nombre de la banda.
   * @param formacion La fecha de formación actualizada.
   * @param separacion La fecha de separación actualizada.
   *
   */
  void actualizarBanda(int id, const std::string& nombre, const std::string& formacion, const std::string& separacion);

  /**
   * @brief Regresa el id de una banda.
   * @param banda El nombre de la banda.
   * @return El id de la banda.
   */
  int obtenerBanda(const std::string& banda) const;

  /**
   * @brief Inserta una banda en la base de datos. 
   * @param nombre El nombre de la banda.
   * @param formacion La fecha de formación.
   * @param separacion La fecha de separación.
   */
  int insertarBanda(const std::string& nombre, const std::string& formacion, const std::string& separacion);

  /**
   * @brief Elimina una banda de la base de datos.
   * @param id El id de la banda a eliminar.
   */
  void eliminarBanda(int id);

  /**
   * @brief Regresa un vector con las personas que forman parte de una banda.
   * @param idBanda El id de la banda.
   * @return Un vector con las personas que forman parte de la banda.
   */
  std::vector<Persona> personasEnBanda(int idBanda);

  /**
   * @brief Regresa un vector con las personas que no forman parte de una banda.
   * @param idBanda El id de la banda.
   * @return Un vector con las personas que no forman parte de la banda.
   */
  std::vector<Persona> personasDisponibles(int idBanda);

  /**
   * @brief Actualiza los miembros de una banda.
   * @param idBanda El id de la banda.
   * @param ids Los ids de las personas que forman parte de la banda.
   */
  void actualizarMiembros(int idBanda, std::vector<int> ids);

  /**
   * @brief Elimina una persona de la base de datos.
   * @param id El id de la persona a eliminar.
   */
  void eliminarPersona(int id);

  /**
   * @brief Regresa un vector con todas las personas.
   * @return Un vector con todas la personas.
   */
  std::vector<Persona> obtenerPersonas();

};

#endif

