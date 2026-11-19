/**
 * @file Minero.hpp
 * @brief Header de la clase Minero.
 */

#ifndef MINERO_HPP
#define MINERO_HPP

#include <iostream>
#include <filesystem>
#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/mpegfile.h>
#include <taglib/id3v2tag.h>
#include <taglib/attachedpictureframe.h>
#include <fstream>
#include <string>
#include "proyecto2/bd/Metadatos.hpp"
#include "proyecto2/bd/dao/CancionesDAO.hpp"
#include "proyecto2/bd/dao/ArtistasDAO.hpp"
#include "proyecto2/bd/dao/AlbumsDAO.hpp"

/**
 * @class Minero
 * @brief Clase que recorre un directorio recursivamente y
 * llena una base de datos con los metadatos de archivos mp3.
 */
class Minero{
private:
  CancionesDAO& canciones; ///< Administra las canciones de la base de datos.
  ArtistasDAO& artistas; ///< Administra los artistas de la base de datos.
  AlbumsDAO& albums; ///< Administra los albums de la base de datos.
  
  /**
   * @brief Recorre recursivamente un directorio.
   * @param directorio La ruta del directorio a recorrer.
   */
  void recorreDirectorio(const std::filesystem::path& directorio) const;

  /**
   * @brief Registra una canción en la base de datos,
   * si ya esta en la base de datos no la registra.
   * @param cancion La ruta de la canción a registrar.
   */
  void registrarCancion(const std::filesystem::path& cancion) const;
  
  /**
   * @brief Extrae la imagen de una canción.
   * @param cancion La ruta de la canción de la cual se extraera la imagen.
   * @param id El id de la canción o de el artista.
   * @param tipo La imagen que se extraera.
   * @param directorio El directorio donde se guardara la imagen.
   */
  void extraerImagen(const std::filesystem::path& cancion, int id, TagLib::ID3v2::AttachedPictureFrame::Type tipo,
		    const std::filesystem::path& directorio) const;

  /**
   * @brief Regresa la ruta de el directorio de cache de el usuario.
   * @return La ruta de el directorio de cache de el usuario.
   */
  std::filesystem::path obtenerDirectorioCache() const;

public:
  /**
   * @brief Constructor de la clase.
   * @param dao DAO de la base de datos.
   */
  Minero(CancionesDAO& canciones, ArtistasDAO& artistas, AlbumsDAO& albums);
  
  /**
   * @brief Mina los archivos MP3 de un directorio y
   * los agrega a la base de datos.
   * @param directorio La ruta del directorio a minar.
   */
  void minar(const std::filesystem::path& directorio) const;
};

#endif
