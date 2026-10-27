/**
 * @file AdministraBD.hpp
 * @brief Header de la clase AdministraBD.
 */

#ifndef ADMINISTRABD_HPP
#define ADMINISTRABD_HPP

#include <sqlite3.h>
#include <filesystem>
#include <fstream>
#include <string>
#include <cstdlib>
#include <stdexcept>
#include <system_error>
#include <iostream>

/**
 * @class AdministraBD
 * @brief Clase que administra la base de datos con sqlite.
 */
class AdministraBD{

private:
  sqlite3* bd; /*!< Base de datos >*/
  const std::string RUTA_ESQUEMA = std::string(DATOS_DIR) + "/rolas.sql"; /*!< Ruta del esquema de la base de datos >*/

  /**
   * @brief Crea la base de datos.
   */
  void crear();

  /**
   * @brief Regresa la ruta de la base de datos.
   * @return la ruta de la base de datos.
   */
  std::string obtenerRutaBD() const;

public:
  /**
   * @brief Constructor de la clase.
   */
  AdministraBD();

  /**
   * @brief Destructor de la clase.
   */
  ~AdministraBD();

  /**
   * @brief Abre la base de datos.
   */
  void abrir();

  /**
   * @brief Regresa true si la base de datos esta abierta,
   * false en otro caso.
   * @return si esta abierta la base de datos.
   */
  bool estaAbierta() const;

  /**
   * @brief Cierra la base de datos.
   */
  void cerrar();

  /**
   * @brief Regresa la base de datos.
   * @return la base de datos.
   */
  sqlite3* getBaseDeDatos() const;
};

#endif
