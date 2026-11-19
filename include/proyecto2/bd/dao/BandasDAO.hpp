/**
 * @file BandasDAO.hpp
 * @brief Header de la clase BandasDAO.
 */

#ifndef BANDASDAO_HPP
#define BANDASDAO_HPP

#include <sqlite3.h>
#include <filesystem>
#include <string>
#include <vector>
#include "proyecto2/bd/Banda.hpp"
#include "proyecto2/bd/Persona.hpp"
#include "proyecto2/bd/AdministraBD.hpp"

/**
 * @class BandasDAO
 * @brief Clase que implementa el patron de diseño Data Access Object
 * para interactuar con las bandas de la base de datos.
 */
class BandasDAO{
private:
  AdministraBD& admin; ///< Administra base de datos.
  
public:
  /**
   * @brief Constructor de la clase.
   * @param admin El administrador de la base de datos.
   */
  BandasDAO(AdministraBD& admin);

  /**
   * @brief Regresa el id de una banda.
   * @param banda El nombre de la banda.
   * @return El id de la banda.
   */
  int obtenerBanda(const std::string& banda) const;

  /**
   * @brief Inserta una banda en la base de datos.
   * @param banda La banda a insertar.
   * @return El id asginado a la banda en la base de datos.
   */
  int insertarBanda(const Banda& banda);

  /**
   * @brief Actualiza la información de una banda.
   * @param banda La banda con la información actualizada.
   */
  void actualizarBanda(const Banda& banda);

  /**
   * @brief Regresa la información de una banda.
   * @param nombre El nombre de la banda.
   * @return La información de una banda.
   */
  Banda obtenerDatosBanda(const std::string& nombre);

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
};

#endif

