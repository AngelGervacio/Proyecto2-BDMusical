/**
 * @file Controlador.hpp
 * @brief Header de la clase Controlador.
 */

#ifndef CONTROLADOR_HPP
#define CONTROLADOR_HPP

#include <QObject>
#include <QString>
#include <vector>
#include <QVector>
#include <QMetaObject>
#include <QFuture>
#include <QUrl>
#include <string>
#include <filesystem>
#include <QVariantList>
#include <QVariantMap>
#include <QtConcurrent/QtConcurrentRun>
#include "proyecto2/bd/Metadatos.hpp"
#include "proyecto2/bd/Artista.hpp"
#include "proyecto2/bd/Album.hpp"
#include "proyecto2/bd/Banda.hpp"
#include "proyecto2/bd/Persona.hpp"
#include "proyecto2/minero/Minero.hpp"
#include "proyecto2/bd/dao/CancionesDAO.hpp"
#include "proyecto2/bd/dao/ArtistasDAO.hpp"
#include "proyecto2/bd/dao/AlbumsDAO.hpp"
#include "proyecto2/bd/dao/BandasDAO.hpp"
#include "proyecto2/bd/dao/PersonasDAO.hpp"
#include "proyecto2/gui/CancionesModelo.hpp"
#include "proyecto2/compilador/Compilador.hpp"

/**
 * @class Controlador
 * @brief Clase que comunica la base de datos con la interfaz.
 */
class Controlador : public QObject{
  Q_OBJECT

  /**
   * @brief Indica si se esta minando un directorio.
   */
  Q_PROPERTY(bool minando READ minando NOTIFY cambioMinando)
private:
  CancionesDAO& canciones; ///< Administra las canciones de la base de datos.
  ArtistasDAO& artistas; ///< Administra los artistas de la base de datos.
  AlbumsDAO& albums; ///< Administra los albums de la base de datos.
  BandasDAO& bandas; ///< Administra las bandas de la base de datos.
  PersonasDAO& personas; ///< Administra las personas de la base de datos.
  CancionesModelo& modelo; ///< El modelo de la tabla.
  bool estaMinando = false; ///< Indica si se estan minando directorios.
  QFuture<void> futuroMinado; ///< Administra el estado de el minado.

  /**
   * @brief Regresa la ruta al directorio de cache de el usuario.
   * @return La ruta al directorio de cache de el usuario.
   */
  std::filesystem::path obtenerDirectorioCache();

  /**
   * @brief Agrega una imagen al directorio de cache de el usuario.
   * @param imagen La ruta de la imagen.
   * @param id El id de la canción, artista, album o persona.
   * @param sufijo El sufijo con el que se guardara la imagen (Cancion, Artista, Album, Persona)
   */
  void agregarImagenEnCache(const QUrl& imagen, int id, std::string sufijo);

  /**
   * @brief Regresa la ruta de una imagen.
   * @param id El id de la canción, artista, album o banda.
   * @param sufijo El sufijo de la imagen (Cancion, Artista, Album, Persona)
   * @return La ruta de la imagen.
   */
  QUrl obtenerImagenDeCache(int id, std::string sufijo);
  
public:

  /**
   * @brief Constructor de la clase.
   * @param canciones Administra las canciones de la base de datos.
   * @param artistas Administra los artistas de la base de datos.
   * @param albums Administra los albums de la base de datos.
   * @param bandas Administra las bandas de la base de datos.
   * @param personas Administra las personas de la base de datos.
   * @param modelo El modelo de la tabla.
   * @param parent Objeto padre.
   */
  Controlador(CancionesDAO& canciones, ArtistasDAO& artistas, AlbumsDAO& albums, BandasDAO& bandas,
	      PersonasDAO& personas, CancionesModelo& modelo, QObject* parent = nullptr);

  /**
   * @brief Carga las canciones en el modelo.
   */
  Q_INVOKABLE void cargarCanciones();

  /**
   * @brief Regresa el estado del minado.
   */
  bool minando();

  /**
   * @brief Regresa la ruta de una canción.
   * @param id El id de la canción.
   * @return La ruta de la canción.
   */
  Q_INVOKABLE QString obtenerRuta(int id) const;

  /**
   * @brief Mina los archivos MP3 de un directorio.
   * @param directorio El directorio a minar.
   */
  Q_INVOKABLE void minarDirectorio(QString directorio);

  /**
   * @brief Agrega una persona a la base de datos.
   * @param imagen La imagen de la persona.
   * @param artistico El nombre artistico de la persona.
   * @param real El nombre real de la persona.
   * @param nacimiento La fecha de nacimiento de la persona.
   * @param fallecimiento La fecha de fallecimiento de la persona.
   */
  Q_INVOKABLE void agregarPersona(const QUrl& imagen, const QString& artistico, const QString& real,
				  const QString& nacimiento, const QString& fallecimiento);

  /**
   * @brief Regresa la información de el album de una canción. 
   * @param id El id de la canción.
   * @return La información de el album de la canción.
   */
  Q_INVOKABLE QVariantMap obtenerDatosAlbum(int id);

  /**
   * @brief Regresa el id de el album de una canción.
   * @param id El id de la canción.
   * @return El id de el album de la canción.
   */
  Q_INVOKABLE int obtenerIdAlbum(int id);

  /**
   * @brief Agrega un album a la base de datos.
   * @param ruta La ruta de el album.
   * @param nombre El nombre de el album.
   * @param anio El año de el album.
   * @param idCancion El id de la canción que esta en el album.
   * @param imagen La imagen de el album.
   */
  Q_INVOKABLE void agregarAlbum(const QString& ruta, const QString nombre, int anio, int idCancion, const QUrl& imagen);

  /**
   * @brief Actualiza la información de un album.
   * @param ruta La ruta de el album.
   * @param nombre El nombre de el album.
   * @param anio El año de el album.
   * @param idAlbum El id de el album a actualizar.
   * @param imagen La imagen del album.
   */
  Q_INVOKABLE void actualizarAlbum(const QString& ruta, const QString nombre, int anio, int idAlbum, const QUrl& imagen);

  /**
   * @brief Elimina un album de la base de datos.
   * @param idAlbum El id de el album a eliminar.
   */
  Q_INVOKABLE void eliminarAlbum(int idAlbum);

  /**
   * @brief Regresa la información de una canción.
   * @param id El id de la canción.
   * @return La información de la canción.
   */
  Q_INVOKABLE QVariantMap obtenerDatosCancion(int id);

  /**
   * @brief Elimina una canción de la base de datos.
   * @param id El id de la canción a eliminar.
   */
  Q_INVOKABLE void eliminarCancion(int id);

  /**
   * @brief Actualiza la información de una canción.
   * @param id El id de la canción a actualizar.
   * @param ruta La ruta de la canción.
   * @param titulo El titulo de la canción.
   * @param artista El artista de la canción.
   * @param idAlbum El album de la canción.
   * @param pista El número de pista de la canción.
   * @param anio El año de la canción.
   * @param genero El genero de la canción.
   * @param imagen La imagen de la canción.
   */
  Q_INVOKABLE void actualizarCancion(int id, const QString& ruta, const QString& titulo, const QString& artista,
				     int idAlbum, int pista, int anio, const QString& genero, const QUrl& imagen);

  /**
   * @brief Regresa una lista con los albums en la base de datos.
   * @return Una lista con los albums en la base de datos.
   */
  Q_INVOKABLE QVariantList obtenerAlbums();

  /**
   * @brief Regresa la información de el artista de una canción.
   * @param id El id de la canción.
   * @return La información de el artista de la canción.
   */
  Q_INVOKABLE QVariantMap obtenerDatosArtista(int id);

  /**
   * @brief Actualiza la información de un artista.
   * @param idArtista El id de el artista.
   * @param tipo El tipo de artista.
   * @param nombre El nombre de el artista.
   * @param imagen La imagen de el artista.
   */
  Q_INVOKABLE void actualizarArtista(int idArtista, int tipo, const QString& nombre, const QUrl& imagen);

  /**
   * @brief Agrega un artista a la base de datos.
   * @param tipo El tipo de artista.
   * @param nombre El nombre de el artista.
   * @param imagen La imagen de el artista.
   * @param idCancion El id de la canción.
   */
  Q_INVOKABLE void agregarArtista(int tipo, const QString& nombre, const QUrl& imagen, int idCancion);

  /**
   * @brief Elimina un artista de la base de datos.
   * @param idArtista El id de el artista a eliminar.
   */
  Q_INVOKABLE void eliminarArtista(int idArtista);

  /**
   * @brief Regresa la información de una banda.
   * @param idArtista El id de la banda.
   * @param nombre El nombre de la banda.
   * @return La información de la banda.
   */
  Q_INVOKABLE QVariantMap obtenerDatosBanda(int idArtista, const QString& nombre);

  /**
   * @brief Actualiza la información de una banda.
   * @param idBanda El id de la banda.
   * @param nombre El nombre de la banda.
   * @param formacion La fecha de formación de la banda.
   * @param separacion La fecha de separación de la banda.
   * @param enBanda La lista de los miembros de la banda.
   * @param aEliminar La lista de personas a eliminar de la base de datos.
   */
  Q_INVOKABLE void actualizarBanda(int idBanda, const QString& nombre, const QString& formacion,
				   const QString& separacion, const QVariantList& enBanda, const QVariantList& aEliminar);

  /**
   * @brief Agrega una banda a la base de datos.
   * @param nombre El nombre de la banda.
   * @param formacion La fecha de formación de la banda.
   * @param separacion La fecha de separación de la banda.
   * @param enBanda La lista de los miembros de la banda.
   * @param aEliminar La lista de personas a eliminar de la base de datos.
   */
  Q_INVOKABLE void agregarBanda(const QString& nombre, const QString& formacion,
				const QString& separacion, const QVariantList& enBanda, const QVariantList& aEliminar);

  /**
   * @brief Elimina una banda de la base de datos.
   * @param id El id de la banda a eliminar.
   */
  Q_INVOKABLE void eliminarBanda(int id);

  /**
   * @brief Regresa una lista de las personas en una banda.
   * @param idBanda El id de la banda.
   * @return Una lista de las personas en la banda.
   */
  Q_INVOKABLE QVariantList personasEnBanda(int idBanda);

  /**
   * @brief Regresa la lista de las personas en la base de datos.
   * @return La lista de las personas en la base de datos.
   */
  Q_INVOKABLE QVariantList obtenerPersonas();
  
  /**
   * @brief Regresa una lista de las personas que no estan en la banda.
   * @param idBanda El id de la banda.
   * @return Una lista de las personas que no estan en la banda.
   */
  Q_INVOKABLE QVariantList personasDisponibles(int idBanda);

  /**
   * @brief Actualiza la información de una persona.
   * @param id El id de la persona a actualizar.
   * @param artistico El nombre artistico de la persona.
   * @param real El nombre real de la persona.
   * @param nacimiento La fecha de nacimiento de la persona.
   * @param fallecimiento La fecha de fallecimiento de la persona.
   * @param imagen La imagen de la persona.
   */
  Q_INVOKABLE void actualizarPersona(int id, const QString& artistico, const QString& real, const QString& nacimiento,
				     const QString& fallecimiento, const QUrl& imagen);

  /**
   * @brief Realiza una busqueda y regresa la información de las canciones obtenidas.
   * 
   *
   */
  Q_INVOKABLE void realizarBusqueda(const QString& busqueda);

signals:
  /**
   * @brief Notifica que se esta minando un directorio.
   */
  void cambioMinando();

  /**
   * @brief Notifica que una persona ya se encuentra en la base de datos.
   */
  void personaYaInsertada();

  /**
   * @brief Notifica que ocurrio un error.
   * @param error El mensaje de error.
   */
  void ocurrioError(QString error);

  /**
   * @brief Notifica que se actualizo la información de un album.
   */
  void albumActualizado();

  /**
   * @brief Notifica que se actualizo la información de una canción.
   */
  void cancionActualizada();

  /**
   * @brief Notifica que se actualizo la información de un artista.
   */
  void artistaActualizado();

  /**
   * @brief Notifica que se actualizo la información de una persona.
   */
  void personaActualizada();

};

#endif
