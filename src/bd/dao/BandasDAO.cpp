/**
 * @file BandasDAO.cpp
 * @brief Implementanción de la clase BandasDAO.
 */

#include "proyecto2/bd/dao/BandasDAO.hpp"

BandasDAO::BandasDAO(AdministraBD& admin) : admin(admin){}

int BandasDAO::obtenerBanda(const std::string& banda) const{
  if(!admin.estaAbierta())
    throw std::runtime_error("La base de datos no esta abierta.");

  if(banda.empty())
    throw std::invalid_argument("No se pudo obtener el id de la banda, tiene un nombre no valido");

  sqlite3* bd = admin.getBaseDeDatos();
  const char* sql = "SELECT id_group FROM groups WHERE name = ? LIMIT 1";
  sqlite3_stmt* stmt = nullptr;

  if(sqlite3_prepare_v2(bd, sql, -1, &stmt, nullptr) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo obtener el id de la banda: ") + sqlite3_errmsg(bd));
  }
  
  if((sqlite3_bind_text(stmt, 1, banda.c_str(), -1, SQLITE_STATIC)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo obtener el id de la banda: ") + sqlite3_errmsg(bd));
  }

  int r = sqlite3_step(stmt);
  int id = -1;

  if(r == SQLITE_ROW)
    id = sqlite3_column_int(stmt, 0);
  else if(r != SQLITE_DONE){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo obtener el id de la banda: ") + sqlite3_errmsg(bd));
  }

  sqlite3_finalize(stmt);
  return id;  
}

int BandasDAO::insertarBanda(const Banda& banda){
  if(!admin.estaAbierta())
    throw std::runtime_error("La base de datos no esta abierta.");

  std::string nombre = banda.getNombre();

  if(nombre.empty())
    throw std::invalid_argument("No se pudo insertar a la banda, tiene un nombre no valido");

  int id = obtenerBanda(nombre);
  if(id != -1)
    return id;

  std::string formacion = banda.getFormacion();
  std::string separacion = banda.getSeparacion();

  sqlite3* bd = admin.getBaseDeDatos();

  const char* sql = "INSERT INTO groups (name, start_date, end_date) VALUES (?, ?, ?)";
  sqlite3_stmt* stmt = nullptr;

  if(sqlite3_prepare_v2(bd, sql, -1, &stmt, nullptr) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo insertar a la banda: ") + sqlite3_errmsg(bd));
  }
  
  if((sqlite3_bind_text(stmt, 1, nombre.c_str(), -1, SQLITE_STATIC)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo insertar a la banda: ") + sqlite3_errmsg(bd));
  }

  if((sqlite3_bind_text(stmt, 2, formacion.c_str(), -1, SQLITE_STATIC)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo insertar a la banda: ") + sqlite3_errmsg(bd));
  }

  if((sqlite3_bind_text(stmt, 3, separacion.c_str(), -1, SQLITE_STATIC)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo insertar a la banda: ") + sqlite3_errmsg(bd));
  }

  int r = sqlite3_step(stmt);

  if(r != SQLITE_DONE){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo insertar a la banda: ") + sqlite3_errmsg(bd));
  }

  id = static_cast<int>(sqlite3_last_insert_rowid(bd));

  sqlite3_finalize(stmt);
  return id;
}

void BandasDAO::actualizarBanda(const Banda& banda){
  if(!admin.estaAbierta())
    throw std::runtime_error("La base de datos no esta abierta.");

  std::string nombre = banda.getNombre();
  
  if(nombre.empty())
    throw std::invalid_argument("No se pudo actualizar a la banda, tiene un nombre no valido.");

  int id = banda.getId();

  if(id < 1)
    throw std::invalid_argument("No se pudo actualizar a la banda, la id no es valida.");

  std::string formacion = banda.getFormacion();
  std::string separacion = banda.getSeparacion();
  
  sqlite3* bd = admin.getBaseDeDatos();
  const char* sql = "UPDATE groups SET name = ?, start_date = ?, end_date = ? WHERE id_group = ?";
  sqlite3_stmt* stmt = nullptr;

  if(sqlite3_prepare_v2(bd, sql, -1, &stmt, nullptr) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo actualizar a la banda: ") + sqlite3_errmsg(bd));
  }

  if((sqlite3_bind_text(stmt, 1, nombre.c_str(), -1, SQLITE_STATIC)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo actualizar a la banda: ") + sqlite3_errmsg(bd));
  }

  if((sqlite3_bind_text(stmt, 2, formacion.c_str(), -1, SQLITE_STATIC)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo actualizar a la banda: ") + sqlite3_errmsg(bd));
  }

  if((sqlite3_bind_text(stmt, 3, separacion.c_str(), -1, SQLITE_STATIC)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo actualizar a la banda: ") + sqlite3_errmsg(bd));
  }

  if((sqlite3_bind_int(stmt, 4, id)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo actualizar a la banda: ") + sqlite3_errmsg(bd));
  }

  int resultado = sqlite3_step(stmt);

  if(sqlite3_changes(bd) == 0){
    sqlite3_finalize(stmt);
    throw std::runtime_error("No se actualizo ninguna banda.");
  }

  if(resultado != SQLITE_DONE){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("Error al actualizar a la banda: ") + sqlite3_errmsg(bd));
  }

  sqlite3_finalize(stmt);
}

Banda BandasDAO::obtenerDatosBanda(const std::string& nombre){
  if(!admin.estaAbierta())
    throw std::runtime_error("La base de datos no esta abierta.");
  
  if(nombre.empty())
    throw std::invalid_argument("El nombre no es valido.");
  
  sqlite3* bd = admin.getBaseDeDatos();
  const char* sql = "SELECT id_group, name, start_date, end_date FROM groups WHERE name = ? LIMIT 1";
  sqlite3_stmt* stmt = nullptr;

  if(sqlite3_prepare_v2(bd, sql, -1, &stmt, nullptr) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo obtener los datos de la banda: ") + sqlite3_errmsg(bd));
  }

  if((sqlite3_bind_text(stmt, 1, nombre.c_str(), -1, SQLITE_STATIC)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo obtener los datos de la banda: ") + sqlite3_errmsg(bd));
  }

  int resultado = sqlite3_step(stmt);

  int id;
  std::string banda;
  std::string formacion;
  std::string separacion;
  
  if(resultado == SQLITE_ROW){
    id = sqlite3_column_int(stmt, 0);
    
    const unsigned char* texto = sqlite3_column_text(stmt, 1);
    int longitud = sqlite3_column_bytes(stmt, 1);

    if(texto == nullptr)
      banda = "";
    else
      banda.assign(reinterpret_cast<const char*>(texto), longitud);

    texto = sqlite3_column_text(stmt, 2);
    longitud = sqlite3_column_bytes(stmt, 2);

    if(texto == nullptr)
      formacion = "";
    else
      formacion.assign(reinterpret_cast<const char*>(texto), longitud);

    texto = sqlite3_column_text(stmt, 3);
    longitud = sqlite3_column_bytes(stmt, 3);

    if(texto == nullptr)
      separacion = "";
    else
      separacion.assign(reinterpret_cast<const char*>(texto), longitud);
    
  }else if(resultado != SQLITE_DONE){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo obtener los datos de la banda: ") + sqlite3_errmsg(bd));
  }

  sqlite3_finalize(stmt);
  return Banda(id, banda, formacion, separacion);
}

void BandasDAO::eliminarBanda(int id){
  if(!admin.estaAbierta())
    throw std::runtime_error("La base de datos no esta abierta.");

  if(id < 1)
    throw std::invalid_argument("No se pudo eliminar a la banda, la id no es valida.");
  
  sqlite3* bd = admin.getBaseDeDatos();
  const char* sql = "DELETE FROM in_group WHERE id_group = ?";
  sqlite3_stmt* stmt = nullptr;
  
  if(sqlite3_prepare_v2(bd, sql, -1, &stmt, nullptr) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo eliminar a la banda: ") + sqlite3_errmsg(bd));
  }

  if((sqlite3_bind_int(stmt, 1, id)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo eliminar a la banda: ") + sqlite3_errmsg(bd));
  }

  int resultado = sqlite3_step(stmt);

  if(resultado != SQLITE_DONE){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("Error al eliminar a la banda: ") + sqlite3_errmsg(bd));
  }

  sqlite3_finalize(stmt);
  stmt = nullptr;
  sql = "DELETE FROM groups WHERE id_group = ?";
  
  if(sqlite3_prepare_v2(bd, sql, -1, &stmt, nullptr) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo eliminar a la banda: ") + sqlite3_errmsg(bd));
  }

  if((sqlite3_bind_int(stmt, 1, id)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo eliminar a la banda: ") + sqlite3_errmsg(bd));
  }

  resultado = sqlite3_step(stmt);

  if(sqlite3_changes(bd) == 0){
    sqlite3_finalize(stmt);
    throw std::runtime_error("No se elimino ninguna banda.");
  }

  if(resultado != SQLITE_DONE){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("Error al eliminar a la banda: ") + sqlite3_errmsg(bd));
  }

  sqlite3_finalize(stmt);  
}

std::vector<Persona> BandasDAO::personasEnBanda(int idBanda){
  if(!admin.estaAbierta())
    throw std::runtime_error("La base de datos no esta abierta.");

  if(idBanda < 1)
    throw std::runtime_error("No hay bandas con esa id.");
  
  sqlite3* bd = admin.getBaseDeDatos();
  const char* sql = "SELECT persons.id_person, persons.stage_name, persons.real_name, persons.birth_date, persons.death_date "
    "FROM persons INNER JOIN in_group ON in_group.id_person = persons.id_person WHERE in_group.id_group = ?";
  sqlite3_stmt* stmt = nullptr;

  if(sqlite3_prepare_v2(bd, sql, -1, &stmt, nullptr) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo obtener a las personas en la banda: ") + sqlite3_errmsg(bd));
  }

  if((sqlite3_bind_int(stmt, 1, idBanda)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo obtener a las personas en la banda: ") + sqlite3_errmsg(bd));
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
    throw std::runtime_error(std::string("Error al obtener a las personas en la banda: ") + sqlite3_errmsg(bd));
  }

  sqlite3_finalize(stmt);
  return personas;
}

std::vector<Persona> BandasDAO::personasDisponibles(int idBanda){
  if(!admin.estaAbierta())
    throw std::runtime_error("La base de datos no esta abierta.");
  
  if(idBanda < 1)
    throw std::runtime_error("No hay bandas con esa id.");
  
  sqlite3* bd = admin.getBaseDeDatos();
  const char* sql = "SELECT persons.id_person, persons.stage_name, persons.real_name, persons.birth_date, persons.death_date "
    "FROM persons LEFT JOIN in_group ON in_group.id_person = persons.id_person AND in_group.id_group = ? WHERE in_group.id_person IS NULL";
  sqlite3_stmt* stmt = nullptr;
  
  if(sqlite3_prepare_v2(bd, sql, -1, &stmt, nullptr) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo obtener a las personas disponibles: ") + sqlite3_errmsg(bd));
  }

  if((sqlite3_bind_int(stmt, 1, idBanda)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo obtener a las personas disponibles: ") + sqlite3_errmsg(bd));
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
    throw std::runtime_error(std::string("Error al obtener a las personas disponibles: ") + sqlite3_errmsg(bd));
  }

  sqlite3_finalize(stmt);
  return personas;
}

void BandasDAO::actualizarMiembros(int idBanda, std::vector<int> ids){
  if(!admin.estaAbierta())
    throw std::runtime_error("La base de datos no esta abierta.");
  
  if(idBanda < 1)
    throw std::runtime_error("No hay bandas con esa id.");
  
  sqlite3* bd = admin.getBaseDeDatos();
  const char* sql = "DELETE FROM in_group WHERE id_group = ?";
  sqlite3_stmt* stmt = nullptr;
  
  if(sqlite3_prepare_v2(bd, sql, -1, &stmt, nullptr) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo actualizar los integrantes de la banda: ") + sqlite3_errmsg(bd));
  }

  if((sqlite3_bind_int(stmt, 1, idBanda)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo actualizar los integrantes de la banda: ") + sqlite3_errmsg(bd));
  }

  int resultado = sqlite3_step(stmt);

  if(resultado != SQLITE_DONE){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("Error al actualizar los integrantes de la banda: ") + sqlite3_errmsg(bd));
  }

  sqlite3_finalize(stmt);
  stmt = nullptr;
  sql = "INSERT INTO in_group (id_person, id_group) VALUES (?, ?)";

  if(sqlite3_prepare_v2(bd, sql, -1, &stmt, nullptr) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo actualizar los integrantes de la banda: ") + sqlite3_errmsg(bd));
  }
  
  size_t i = 0;
  while (i < ids.size()) {
    
    if((sqlite3_bind_int(stmt, 1, ids[i])) != SQLITE_OK){
      sqlite3_finalize(stmt);
      throw std::runtime_error(std::string("No se pudo actualizar los integrantes de la banda: ") + sqlite3_errmsg(bd));
    }
    
    if((sqlite3_bind_int(stmt, 2, idBanda)) != SQLITE_OK){
      sqlite3_finalize(stmt);
      throw std::runtime_error(std::string("No se pudo actualizar los integrantes de la banda: ") + sqlite3_errmsg(bd));
    }

    if(sqlite3_step(stmt) != SQLITE_DONE) {
      sqlite3_finalize(stmt);
      throw std::runtime_error(std::string("No se pudo actualizar los integrantes de la banda: ") + sqlite3_errmsg(bd));
    }

    sqlite3_reset(stmt);
    sqlite3_clear_bindings(stmt);
    
    i++;
  }

  sqlite3_finalize(stmt);
}
