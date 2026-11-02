/**
 * @file Metadatos.hpp
 * @brief Header de la clase Metadatos.
 */

#ifndef METADATOS_HPP
#define METADATOS_HPP

#include <string>
#include <filesystem>

/**
 * @class Metadatos
 * @brief Clase con los metadatos de una canción.
 */
class Metadatos{
private:
  std::filesystem::path ruta; ///< Ruta de la canción.
  std::string titulo; ///< Titulo de la canción.
  std::string artista; ///< Artista de la canción.
  std::string album; ///< Album de la canción.
  int pista; ///< Pista de la canción.
  int anio; ///< Año de la canción.
  std::string genero; ///< Genero de la canción.

public:

  /**
   * @class Builder
   * @brief Patron de diseño Builder.
   */
  class Builder{
  private:
    std::filesystem::path ruta; ///< Ruta de la canción.
    std::string titulo; ///< Titulo de la canción.
    std::string artista; ///< Artista de la canción.
    std::string album; ///< Album de la canción.
    int pista; ///< Pista de la canción.
    int anio; ///< Año de la canción.
    std::string genero; ///< Genero de la canción.

  public:
    
    /**
     * @brief Asigna una ruta a la canción.
     * @param ruta La ruta de la canción.
     * @return Un Builder con ruta asignada.
     */
    Builder& setRuta(const std::filesystem::path& ruta);

    /**
     * @brief Asigna un titulo a la canción.
     * @param titulo El titulo de la canción.
     * @return Un Builder con titulo asignado.
     */
    Builder& setTitulo(const std::string& titulo);

    /**
     * @brief Asigna un artista a la canción.
     * @param artista El artista de la canción.
     * @return Un Builder con artista asignado.
     */
    Builder& setArtista(const std::string& artista);

    /**
     * @brief Asigna un album a la canción.
     * @param album El album de la canción.
     * @return Un Builder con album asignado.
     */
    Builder& setAlbum(const std::string& album);

    /**
     * @brief Asigna un número de pista a la canción.
     * @param pista El número de pista a la canción.
     * @return Un Builder con pista asignado.
     */
    Builder& setPista(int pista);

    /**
     * @brief Asigna un año a la canción. 
     * @param anio El año de la canción.
     * @return Un Builder con año asignado.
     */
    Builder& setAnio(int anio);

    /**
     * @brief Asigna un genero de la canción.
     * @param genero El genero de la canción.
     * @return Un Builder con genero asignado.
     */
    Builder& setGenero(const std::string& genero);

    /**
     * @brief Construye los Metadatos.
     */
    Metadatos build();
  };

  /**
   * @brief Constructor de la clase Metadatos.
   * @param ruta La ruta de la canción.
   * @param titulo El titulo de la canción.
   * @param artista El artista de la canción.
   * @param album El album de la canción.
   * @param pista El número de pista de la canción.
   * @param anio El año de la canción.
   * @param genero El genero de la canción.
   */
  Metadatos(const std::filesystem::path& ruta,
	    const std::string& titulo,
	    const std::string& artista,
	    const std::string& album,
	    int pista,
	    int anio,
	    const std::string& genero);
    
  /**
   * @brief Regresa la ruta de la canción.
   * @return La ruta de la canción.
   */
  const std::filesystem::path& getRuta() const;

  /**
   * @brief Regresa el titulo de la canción.
   * @return El titulo de la canción.
   */
  std::string getTitulo() const;

  /**
   * @brief Regresa el artista de la canción.
   * @return El artista de la canción.
   */
  std::string getArtista() const;

  /**
   * @brief Regresa el album de la canción.
   * @return El album de la canción.
   */
  std::string getAlbum() const;

  /**
   * @brief Regresa el número de pista de la canción.
   * @return El número de pista de la canción.
   */
  int getPista() const;

  /**
   * @brief Regresa el año de la canción.
   * @return El año de la canción.
   */
  int getAnio() const;

  /**
   * @brief Regresa el genero de la canción.
   * @return El genero de la canción.
   */
  std::string getGenero() const;
};

#endif
