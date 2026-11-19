/**
 * @file ArtistasDAO.cpp
 * @brief Implementación de la clase ArtistasDAO.
 */

#include "proyecto2/bd/dao/ArtistasDAO.hpp"

ArtistasDAO::ArtistasDAO(AdministraBD& admin) : admin(admin){}

int ArtistasDAO::obtenerArtista(const std::string& artista) const{
  if(!admin.estaAbierta())
    throw std::runtime_error("La base de datos no esta abierta.");
  
  if(artista.empty())
    throw std::invalid_argument("No se pudo obtener el id del artista, tiene un nombre no valido");
  
  sqlite3* bd = admin.getBaseDeDatos();
  const char* sql = "SELECT id_performer FROM performers WHERE name = ? LIMIT 1";
  sqlite3_stmt* stmt = nullptr;

  if(sqlite3_prepare_v2(bd, sql, -1, &stmt, nullptr) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo obtener la id del artista: ") + sqlite3_errmsg(bd));
  }
  
  if((sqlite3_bind_text(stmt, 1, artista.c_str(), -1, SQLITE_STATIC)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo obtener la id del artista: ") + sqlite3_errmsg(bd));
  }

  int r = sqlite3_step(stmt);
  int id = -1;

  if(r == SQLITE_ROW)
    id = sqlite3_column_int(stmt, 0);
  else if(r != SQLITE_DONE){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo obtener la id del artista: ") + sqlite3_errmsg(bd));
  }

  sqlite3_finalize(stmt);
  return id;
}

int ArtistasDAO::insertarArtista(const std::string& artista) const{
  if(!admin.estaAbierta())
    throw std::runtime_error("La base de datos no esta abierta.");
  
  if(artista.empty())
    throw std::invalid_argument("No se pudo insertar al artista, tiene un nombre no valido");

  int id = obtenerArtista(artista);
  if(id != -1)
    return id;

  sqlite3* bd = admin.getBaseDeDatos();

  const char* sql = "INSERT INTO performers (id_type, name) VALUES (2, ?)";
  sqlite3_stmt* stmt = nullptr;

  if(sqlite3_prepare_v2(bd, sql, -1, &stmt, nullptr) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo insertar al artista: ") + sqlite3_errmsg(bd));
  }
  
  if((sqlite3_bind_text(stmt, 1, artista.c_str(), -1, SQLITE_STATIC)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo insertar al artista: ") + sqlite3_errmsg(bd));
  }

  int r = sqlite3_step(stmt);

  if(r != SQLITE_DONE){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo insertar al artista: ") + sqlite3_errmsg(bd));
  }

  id = static_cast<int>(sqlite3_last_insert_rowid(bd));

  sqlite3_finalize(stmt);
  return id;
}

void ArtistasDAO::actualizarArtista(Artista artista){
  if(!admin.estaAbierta())
    throw std::runtime_error("La base de datos no esta abierta.");

  std::string nombre = artista.getNombre();
  
  if(nombre.empty())
    throw std::invalid_argument("No se pudo actualizar al artista, tiene un nombre no valido.");

  int tipo = artista.getTipo();

  if(tipo < 0 || tipo > 2)
    throw std::invalid_argument("No se pudo actualizar al artista, tiene un tipo no valido.");

  int idArtista = artista.getId();

  if(idArtista < 0)
    throw std::invalid_argument("No se pudo actualizar al artista, el id del artista no es valido.");
  
  sqlite3* bd = admin.getBaseDeDatos();
  const char* sql = "UPDATE performers SET id_type = ?, name = ? WHERE id_performer = ?";
  sqlite3_stmt* stmt = nullptr;

  if(sqlite3_prepare_v2(bd, sql, -1, &stmt, nullptr) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo actualizar al artista: ") + sqlite3_errmsg(bd));
  }

  if((sqlite3_bind_int(stmt, 1, tipo)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo actualizar al artista: ") + sqlite3_errmsg(bd));
  }

  if((sqlite3_bind_text(stmt, 2, nombre.c_str(), -1, SQLITE_STATIC)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo actualizar al artista: ") + sqlite3_errmsg(bd));
  }

  if((sqlite3_bind_int(stmt, 3, idArtista)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo actualizar al artista: ") + sqlite3_errmsg(bd));
  }

  int resultado = sqlite3_step(stmt);

  if(sqlite3_changes(bd) == 0){
    sqlite3_finalize(stmt);
    throw std::runtime_error("No se actualizo ningun artista.");
  }

  if(resultado != SQLITE_DONE){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("Error al actualizar al artista: ") + sqlite3_errmsg(bd));
  }

  sqlite3_finalize(stmt);
}

Artista ArtistasDAO::obtenerDatosArtista(int id){
  if(!admin.estaAbierta())
    throw std::runtime_error("La base de datos no esta abierta.");
  
  if(id < 1)
    throw std::invalid_argument("El id no es valido.");

  sqlite3* bd = admin.getBaseDeDatos();
  const char* sql = "SELECT performers.id_performer, performers.id_type, performers.name FROM performers INNER JOIN rolas ON performers.id_performer = rolas.id_performer WHERE rolas.id_rola = ? LIMIT 1";
  sqlite3_stmt* stmt = nullptr;

  if(sqlite3_prepare_v2(bd, sql, -1, &stmt, nullptr) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo obtener el tipo de artista: ") + sqlite3_errmsg(bd));
  }

  if((sqlite3_bind_int(stmt, 1, id)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo obtener el tipo de artista: ") + sqlite3_errmsg(bd));
  }

  int resultado = sqlite3_step(stmt);

  int idArtista;
  int tipo;
  std::string nombre;
  
  if(resultado == SQLITE_ROW){
    idArtista = sqlite3_column_int(stmt, 0);
    tipo = sqlite3_column_int(stmt, 1);
    
    const unsigned char* texto = sqlite3_column_text(stmt, 2);
    int longitud = sqlite3_column_bytes(stmt, 2);

    if(texto == nullptr)
      nombre = "Unknown";
    else
      nombre.assign(reinterpret_cast<const char*>(texto), longitud);
  }else if(resultado != SQLITE_DONE){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo obtener el tipo de artista: ") + sqlite3_errmsg(bd));
  }

  sqlite3_finalize(stmt);
  return Artista(idArtista, tipo, nombre);
}

void ArtistasDAO::eliminarArtista(int id){
  if(!admin.estaAbierta())
    throw std::runtime_error("La base de datos no esta abierta.");

  if(id < 1)
    throw std::invalid_argument("No se pudo eliminar al artista, la id del artista no es valido.");
  
  sqlite3* bd = admin.getBaseDeDatos();
  const char* sql = "DELETE FROM performers WHERE id_performer = ?";
  sqlite3_stmt* stmt = nullptr;

  if(sqlite3_prepare_v2(bd, sql, -1, &stmt, nullptr) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo eliminar al artista: ") + sqlite3_errmsg(bd));
  }

  if((sqlite3_bind_int(stmt, 1, id)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo eliminar al artista: ") + sqlite3_errmsg(bd));
  }

  int resultado = sqlite3_step(stmt);

  if(sqlite3_changes(bd) == 0){
    sqlite3_finalize(stmt);
    throw std::runtime_error("No se elimino ningun artista.");
  }

  if(resultado != SQLITE_DONE){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("Error al eliminar al artista: ") + sqlite3_errmsg(bd));
  }

  sqlite3_finalize(stmt);  
}
