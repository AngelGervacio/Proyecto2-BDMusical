/**
 * @file PersonasDAO.cpp
 * @brief Implementación de la clase PersonasDAO.
 */

#include "proyecto2/bd/dao/PersonasDAO.hpp"

PersonasDAO::PersonasDAO(AdministraBD& admin) : admin(admin){}

int PersonasDAO::obtenerPersona(const std::string& artistico) const{
  if(!admin.estaAbierta())
    throw std::runtime_error("La base de datos no esta abierta.");

  if(artistico.empty())
    throw std::invalid_argument("No se pudo obtener el id de la persona, tiene un nombre no valido");

  sqlite3* bd = admin.getBaseDeDatos();
  const char* sql = "SELECT id_person FROM persons WHERE stage_name = ? LIMIT 1";
  sqlite3_stmt* stmt = nullptr;

  if(sqlite3_prepare_v2(bd, sql, -1, &stmt, nullptr) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo obtener la id de la persona: ") + sqlite3_errmsg(bd));
  }
  
  if((sqlite3_bind_text(stmt, 1, artistico.c_str(), -1, SQLITE_STATIC)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo obtener la id de la persona: ") + sqlite3_errmsg(bd));
  }

  int r = sqlite3_step(stmt);
  int id = -1;

  if(r == SQLITE_ROW)
    id = sqlite3_column_int(stmt, 0);
  else if(r != SQLITE_DONE){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo obtener la id de la persona: ") + sqlite3_errmsg(bd));
  }

  sqlite3_finalize(stmt);
  return id;
}

int PersonasDAO::insertarPersona(const Persona& persona){
  if(!admin.estaAbierta())
    throw std::runtime_error("La base de datos no esta abierta.");

  std::string artistico = persona.getArtistico();

  if(artistico.empty())
    throw std::invalid_argument("Nombre de persona no valido, no se pudo insertar");

  int idPersona = obtenerPersona(artistico);

  if(idPersona != -1)
    return -1;

  std::string real = persona.getReal();
  std::string nacimiento = persona.getNacimiento();
  std::string fallecimiento = persona.getFallecimiento();

  sqlite3* bd = admin.getBaseDeDatos();

  const char* sql = "INSERT INTO persons (stage_name, real_name, birth_date, death_date) VALUES (?, NULLIF(?, ''), NULLIF(?, ''), NULLIF(?, ''))";
  sqlite3_stmt* stmt = nullptr;

  if(sqlite3_prepare_v2(bd, sql, -1, &stmt, nullptr) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo insertar a la persona: ") + sqlite3_errmsg(bd));
  }

  if((sqlite3_bind_text(stmt, 1, artistico.c_str(), -1, SQLITE_STATIC)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo insertar a la persona: ") + sqlite3_errmsg(bd));
  }

  if((sqlite3_bind_text(stmt, 2, real.c_str(), -1, SQLITE_STATIC)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo insertar a la persona: ") + sqlite3_errmsg(bd));
  }

  if((sqlite3_bind_text(stmt, 3, nacimiento.c_str(), -1, SQLITE_STATIC)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo insertar a la persona: ") + sqlite3_errmsg(bd));
  }

  if((sqlite3_bind_text(stmt, 4, fallecimiento.c_str(), -1, SQLITE_STATIC)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo insertar a la persona: ") + sqlite3_errmsg(bd));
  }

  int r = sqlite3_step(stmt);

  if(r != SQLITE_DONE){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo insertar a la persona: ") + sqlite3_errmsg(bd));
  }

  idPersona = static_cast<int>(sqlite3_last_insert_rowid(bd));

  sqlite3_finalize(stmt);

  return idPersona;
}

void PersonasDAO::actualizarPersona(const Persona& persona){
  if(!admin.estaAbierta())
    throw std::runtime_error("La base de datos no esta abierta.");

  int id = persona.getId();
  
  if(id < 1)
    throw std::invalid_argument("No se pudo actualizar a la persona, la id no es valida.");

  std::string artistico = persona.getArtistico();

  if(artistico.empty())
    throw std::invalid_argument("No se pudo actualizar a la persona, tiene un nombre no valido.");

  int idExiste = obtenerPersona(artistico);

  if(idExiste != -1 && persona.getId() != idExiste)
    throw std::invalid_argument("Ya existe una persona con ese nombre.");

  std::string real = persona.getReal();
  std::string nacimiento = persona.getNacimiento();
  std::string fallecimiento = persona.getFallecimiento();
  
  sqlite3* bd = admin.getBaseDeDatos();
  const char* sql = "UPDATE persons SET stage_name = ?, real_name = ?, birth_date = ?, death_date = ? WHERE id_person = ?";
  sqlite3_stmt* stmt = nullptr;

  if(sqlite3_prepare_v2(bd, sql, -1, &stmt, nullptr) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo actualizar a la persona: ") + sqlite3_errmsg(bd));
  }

  if((sqlite3_bind_text(stmt, 1, artistico.c_str(), -1, SQLITE_STATIC)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo actualizar a la persona: ") + sqlite3_errmsg(bd));
  }

  if((sqlite3_bind_text(stmt, 2, real.c_str(), -1, SQLITE_STATIC)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo actualizar a la persona: ") + sqlite3_errmsg(bd));
  }

  if((sqlite3_bind_text(stmt, 3, nacimiento.c_str(), -1, SQLITE_STATIC)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo actualizar a la persona: ") + sqlite3_errmsg(bd));
  }

  if((sqlite3_bind_text(stmt, 4, fallecimiento.c_str(), -1, SQLITE_STATIC)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo actualizar a la persona: ") + sqlite3_errmsg(bd));
  }

  if((sqlite3_bind_int(stmt, 5, id)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo actualizar a la persona: ") + sqlite3_errmsg(bd));
  }

  int resultado = sqlite3_step(stmt);

  if(sqlite3_changes(bd) == 0){
    sqlite3_finalize(stmt);
    throw std::runtime_error("No se actualizo ninguna persona.");
  }

  if(resultado != SQLITE_DONE){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("Error al actualizar a la persona: ") + sqlite3_errmsg(bd));
  }

  sqlite3_finalize(stmt);
}

void PersonasDAO::eliminarPersona(int id){
  if(!admin.estaAbierta())
    throw std::runtime_error("La base de datos no esta abierta.");

  if(id < 1)
    throw std::invalid_argument("No se pudo eliminar a la persona, la id no es valida.");
  
  sqlite3* bd = admin.getBaseDeDatos();
  const char* sql = "DELETE FROM in_group WHERE id_person = ?";
  sqlite3_stmt* stmt = nullptr;
  
  if(sqlite3_prepare_v2(bd, sql, -1, &stmt, nullptr) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo eliminar a la persona: ") + sqlite3_errmsg(bd));
  }

  if((sqlite3_bind_int(stmt, 1, id)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo eliminar a la persona: ") + sqlite3_errmsg(bd));
  }

  int resultado = sqlite3_step(stmt);

  if(resultado != SQLITE_DONE){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("Error al eliminar a la persona: ") + sqlite3_errmsg(bd));
  }

  sqlite3_finalize(stmt);
  stmt = nullptr;
  sql = "DELETE FROM persons WHERE id_person = ?";

  if(sqlite3_prepare_v2(bd, sql, -1, &stmt, nullptr) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo eliminar a la persona: ") + sqlite3_errmsg(bd));
  }

  if((sqlite3_bind_int(stmt, 1, id)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo eliminar a la persona: ") + sqlite3_errmsg(bd));
  }

  resultado = sqlite3_step(stmt);

  if(sqlite3_changes(bd) == 0){
    sqlite3_finalize(stmt);
    throw std::runtime_error("No se elimino ninguna persona.");
  }

  if(resultado != SQLITE_DONE){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("Error al eliminar a la persona: ") + sqlite3_errmsg(bd));
  }

  sqlite3_finalize(stmt);
}

std::vector<Persona> PersonasDAO::obtenerPersonas(){
  if(!admin.estaAbierta())
    throw std::runtime_error("La base de datos no esta abierta.");
  
  sqlite3* bd = admin.getBaseDeDatos();
  const char* sql = "SELECT id_person, stage_name, real_name, birth_date, death_date FROM persons ";
  sqlite3_stmt* stmt = nullptr;

  if(sqlite3_prepare_v2(bd, sql, -1, &stmt, nullptr) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo obtener a las personas en la base de datos: ") + sqlite3_errmsg(bd));
  }

  std::vector<Persona> personas;

  int resultado;

  while((resultado = sqlite3_step(stmt)) == SQLITE_ROW){
    int id = sqlite3_column_int(stmt, 0);
    std::string artistico;
    std::string real;
    std::string nacimiento;
    std::string fallecimiento;
    
    const unsigned char* texto = sqlite3_column_text(stmt, 1);
    int longitud = sqlite3_column_bytes(stmt, 1);

    if(texto == nullptr)
      artistico = "";
    else
      artistico.assign(reinterpret_cast<const char*>(texto), longitud);

    texto = sqlite3_column_text(stmt, 2);
    longitud = sqlite3_column_bytes(stmt, 2);

    if(texto == nullptr)
      real = "";
    else
      real.assign(reinterpret_cast<const char*>(texto), longitud);

    texto = sqlite3_column_text(stmt, 3);
    longitud = sqlite3_column_bytes(stmt, 3);

    if(texto == nullptr)
      nacimiento = "";
    else
      nacimiento.assign(reinterpret_cast<const char*>(texto), longitud);

    texto = sqlite3_column_text(stmt, 4);
    longitud = sqlite3_column_bytes(stmt, 4);

    if(texto == nullptr)
      fallecimiento = "";
    else
      fallecimiento.assign(reinterpret_cast<const char*>(texto), longitud);

    personas.push_back(Persona(id, artistico, real, nacimiento, fallecimiento));
  }

  if(resultado != SQLITE_DONE){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("Error al obtener a las personas en la base de datos: ") + sqlite3_errmsg(bd));
  }

  sqlite3_finalize(stmt);
  return personas;
}

