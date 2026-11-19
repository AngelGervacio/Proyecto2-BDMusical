/**
 * @file PersonasDAO.hpp
 * @brief Header de la clase PersonasDAO.
 */

#ifndef PERSONASDAO_HPP
#define PERSONASDAO_HPP

#include <sqlite3.h>
#include <filesystem>
#include <string>
#include <vector>
#include "proyecto2/bd/Persona.hpp"
#include "proyecto2/bd/AdministraBD.hpp"

/**
 * @class PersonasDAO
 * @brief Clase que implementa el patron de diseño Data Access Object
 * para interactuar con las personas de la base de datos.
 */
class PersonasDAO{
private:
  AdministraBD& admin; ///< Administra base de datos.
  
public:
  /**
   * @brief Constructor de la clase.
   * @param admin El administrador de la base de datos.
   */
  PersonasDAO(AdministraBD& admin);

  /**
   * @brief Regresa el id de una persona mediante su nombre artistico.
   * @param artistico El nombre artistico de la persona.
   * @return El id de la persona.
   */
  int obtenerPersona(const std::string& artistico) const;

  /**
   * @brief Inserta una persona a la base de datos.
   * @param persona La persona a insertar en la base de datos.
   * @return El id asignado a la persona insertada.
   */
  int insertarPersona(const Persona& persona);

  /**
   * @brief Actualiza la información de una persona.
   * @param persona La persona con la información actualizada.
   */
  void actualizarPersona(const Persona& persona);
  
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
