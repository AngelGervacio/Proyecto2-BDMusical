/**
 * @file Persona.hpp
 * @brief Header de la clase Persona.
 */

#ifndef PERSONA_HPP
#define PERSONA_HPP

#include <string>

class Persona{
private:
  int id; ///< Id de la persona.
  std::string artistico; ///< Nombre artistico de la persona.
  std::string real; ///< Nombre real de la persona.
  std::string nacimiento; ///< Fecha de nacimiento de la persona.
  std::string fallecimiento; ///< Fecha de fallecimiento de la persona.

public:
  /**
   * @brief Constructor de la clase.
   * @param artistico El nombre artistico de la persona.
   * @param real El nombre real de la persona.
   * @param nacimiento La fecha de nacimiento de la persona.
   * @param fallecimiento La fecha de fallecimiento de la persona.
   */
  Persona(int id, const std::string& artistico, const std::string& real,
	  const std::string& nacimiento, const std::string& fallecimiento);

  /**
   * @brief Regresa el id de la persona.
   * @return El id de la persona.
   */
  int getId() const;

  /**
   * @brief Regresa el nombre artistico de la persona.
   * @return El nombre artistico de la persona.
   */
  std::string getArtistico() const;

  /**
   * @brief Regresa el nombre real de la persona.
   * @return El nombre real de la persona.
   */
  std::string getReal() const;

  /**
   * @brief Regresa la fecha de nacimiento de la persona.
   * @return La fecha de nacimiento de la persona.
   */
  std::string getNacimiento() const;

  /**
   * @brief Regresa la fecha de fallecimiento de la persona.
   * @return La fecha de fallecimiento de la persona.
   */
  std::string getFallecimiento() const;
};

#endif
