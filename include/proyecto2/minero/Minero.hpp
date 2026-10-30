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
#include "proyecto2/bd/Metadatos.hpp"
#include "proyecto2/bd/BaseDatosDAO.hpp"

/**
 * @class Minero
 * @brief Clase que recorre un directorio recursivamente.
 */
class Minero{
private:
  BaseDatosDAO& dao; /*!< Administra Base de Datos > */
  
  /**
   * @brief Recorre recursivamente un directorio.
   * @param directorio La ruta del directorio a recorrer.
   */
  void recorreDirectorio(const std::filesystem::path& directorio) const;

  /**
   * @brief Registra una canción en la base de datos,
   * si ya esta en la base de datos no la registra.
   * @param canción La ruta de la canción a registrar.
   */
  void registrarCancion(const std::filesystem::path& cancion) const;

public:
  /**
   * @brief Constructor de la clase.
   * @param dao DAO de la base de datos.
   */
  Minero(BaseDatosDAO& dao);
  
  /**
   * @brief Mina los archivos MP3 de un directorio y
   * los agrega a la base de datos.
   * @param directorio La ruta del directorio a minar.
   */
  void minar(const std::filesystem::path& directorio) const;
};

#endif
