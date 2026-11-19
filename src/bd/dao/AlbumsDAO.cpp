/**
 * @file AlbumsDAO.cpp
 * @brief Implementación de la clase AlbumsDAO.
 */

#include "proyecto2/bd/dao/AlbumsDAO.hpp"

AlbumsDAO::AlbumsDAO(AdministraBD& admin) : admin(admin){}

int AlbumsDAO::obtenerAlbum(const std::string& ruta) const{
  if(!admin.estaAbierta())
    throw std::runtime_error("La base de datos no esta abierta.");

  if(ruta.empty())
    throw std::invalid_argument("No se pudo obtener la id del album, tiene una ruta no valida");

  sqlite3* bd = admin.getBaseDeDatos();
  const char* sql = "SELECT id_album FROM albums WHERE path = ? LIMIT 1";
  sqlite3_stmt* stmt = nullptr;

  if(sqlite3_prepare_v2(bd, sql, -1, &stmt, nullptr) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo obtener la id del album: ") + sqlite3_errmsg(bd));
  }
  
  if((sqlite3_bind_text(stmt, 1, ruta.c_str(), -1, SQLITE_STATIC)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo obtener la id del album: ") + sqlite3_errmsg(bd));
  }

  int r = sqlite3_step(stmt);
  int id = -1;

  if(r == SQLITE_ROW)
    id = sqlite3_column_int(stmt, 0);
  else if(r != SQLITE_DONE){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo obtener la id del album: ") + sqlite3_errmsg(bd));
  }

  sqlite3_finalize(stmt);
  return id;
}

int AlbumsDAO::insertarAlbum(const Album& album) const{
  if(!admin.estaAbierta())
    throw std::runtime_error("La base de datos no esta abierta.");

  std::string ruta = album.getRuta();

  if(ruta.empty())
    throw std::invalid_argument("No se pudo insertar el album, tiene una ruta no valida");

  std::string nombre = album.getNombre();

  if(nombre.empty())
    throw std::invalid_argument("No se pudo insertar el album, tiene un nombre no valido");

  int anio = album.getAnio();

  if(anio < 0)
    throw std::invalid_argument("No se pudo insertar el album, tiene un año invalido");

  int id = obtenerAlbum(ruta);
  if(id != -1)
    return id;

  sqlite3* bd = admin.getBaseDeDatos();

  const char* sql = "INSERT INTO albums (path, name, year) VALUES (?, ?, ?)";
  sqlite3_stmt* stmt = nullptr;

  if(sqlite3_prepare_v2(bd, sql, -1, &stmt, nullptr) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo insertar el album: ") + sqlite3_errmsg(bd));
  }
  
  if((sqlite3_bind_text(stmt, 1, ruta.c_str(), -1, SQLITE_STATIC)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo insertar el album: ") + sqlite3_errmsg(bd));
  }

  if((sqlite3_bind_text(stmt, 2, nombre.c_str(), -1, SQLITE_STATIC)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo insertar el album: ") + sqlite3_errmsg(bd));
  }

  if((sqlite3_bind_int(stmt, 3, anio)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo insertar el album: ") + sqlite3_errmsg(bd));
  }

  int r = sqlite3_step(stmt);

  if(r != SQLITE_DONE){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo insertar el album: ") + sqlite3_errmsg(bd));
  }

  id = static_cast<int>(sqlite3_last_insert_rowid(bd));

  sqlite3_finalize(stmt);
  return id;
}

void AlbumsDAO::actualizarAlbum(const Album& album){
  if(!admin.estaAbierta())
    throw std::runtime_error("La base de datos no esta abierta.");

  std::string ruta = album.getRuta();

  if(ruta.empty())
    throw std::invalid_argument("No se pudo actualizar el album, tiene una ruta no valida.");

  std::string nombre = album.getNombre();
  
  if(nombre.empty())
    throw std::invalid_argument("No se pudo actualizar el album, tiene un nombre no valido.");

  int anio = album.getAnio();

  if(anio < 0)
    throw std::invalid_argument("No se pudo actualizar el album, tiene un año no valido.");

  int id = album.getId();

  if(id < 1)
    throw std::invalid_argument("No se pudo actualizar el album, la id del album no es valida.");
  
  sqlite3* bd = admin.getBaseDeDatos();
  const char* sql = "UPDATE albums SET path = ?, name = ?, year = ? WHERE id_album = ?";
  sqlite3_stmt* stmt = nullptr;

  if(sqlite3_prepare_v2(bd, sql, -1, &stmt, nullptr) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo actualizar el album de la canción: ") + sqlite3_errmsg(bd));
  }

  if((sqlite3_bind_text(stmt, 1, ruta.c_str(), -1, SQLITE_STATIC)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo actualizar el album de la canción: ") + sqlite3_errmsg(bd));
  }

  if((sqlite3_bind_text(stmt, 2, nombre.c_str(), -1, SQLITE_STATIC)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo actualizar el album de la canción: ") + sqlite3_errmsg(bd));
  }

  if((sqlite3_bind_int(stmt, 3, anio)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo actualizar el album de la canción: ") + sqlite3_errmsg(bd));
  }

  if((sqlite3_bind_int(stmt, 4, id)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo actualizar el album de la canción: ") + sqlite3_errmsg(bd));
  }

  int resultado = sqlite3_step(stmt);

  if(sqlite3_changes(bd) == 0){
    sqlite3_finalize(stmt);
    throw std::runtime_error("No se actualizo ningun album.");
  }

  if(resultado != SQLITE_DONE){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("Error al actualizar el album de la canción: ") + sqlite3_errmsg(bd));
  }

  sqlite3_finalize(stmt);
}

Album AlbumsDAO::obtenerDatosAlbum(int id){
  if(!admin.estaAbierta())
    throw std::runtime_error("La base de datos no esta abierta.");

  if(id < 0)
    throw std::invalid_argument("El id no es valido.");

  sqlite3* bd = admin.getBaseDeDatos();
  const char* sql = "SELECT id_album, path, name, year FROM albums WHERE id_album = ?";
  sqlite3_stmt* stmt = nullptr;

  if(sqlite3_prepare_v2(bd, sql, -1, &stmt, nullptr) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudieron obtener los datos del album: ") + sqlite3_errmsg(bd));
  }

  if((sqlite3_bind_int(stmt, 1, id)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudieron obtener los datos del album: ") + sqlite3_errmsg(bd));
  }

  int resultado = sqlite3_step(stmt);

  int idAlbum = 0;
  std::string ruta;
  std::string nombre = "Unknown";
  int anio = 0;
  
  if(resultado == SQLITE_ROW){
    idAlbum = sqlite3_column_int(stmt, 0);
    
    const unsigned char* texto = sqlite3_column_text(stmt, 1);
    int longitud = sqlite3_column_bytes(stmt, 1);

    if(texto == nullptr)
      ruta = "";
    else
      ruta.assign(reinterpret_cast<const char*>(texto), longitud);

    texto = sqlite3_column_text(stmt, 2);
    longitud = sqlite3_column_bytes(stmt, 2);

    if(texto == nullptr)
      nombre = "Unknown";
    else
      nombre.assign(reinterpret_cast<const char*>(texto), longitud);
    
    anio = sqlite3_column_int(stmt, 3);
    
  }else if(resultado != SQLITE_DONE){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("Error al obtener los datos del album: ") + sqlite3_errmsg(bd));
  }

  sqlite3_finalize(stmt);
  return Album(idAlbum, ruta, nombre, anio);
}

int AlbumsDAO::obtenerIdAlbum(int id){
  if(!admin.estaAbierta())
    throw std::runtime_error("La base de datos no esta abierta.");
  
  if(id < 1)
    throw std::invalid_argument("El id no es valido1.");

  sqlite3* bd = admin.getBaseDeDatos();
  const char* sql = "SELECT albums.id_album FROM albums INNER JOIN rolas ON albums.id_album = rolas.id_album WHERE rolas.id_rola = ? LIMIT 1";
  sqlite3_stmt* stmt = nullptr;

  if(sqlite3_prepare_v2(bd, sql, -1, &stmt, nullptr) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo obtener la id del album: ") + sqlite3_errmsg(bd));
  }

  if((sqlite3_bind_int(stmt, 1, id)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo obtener la id del album: ") + sqlite3_errmsg(bd));
  }

  int resultado = sqlite3_step(stmt);

  int idAlbum = -1;
  
  if(resultado == SQLITE_ROW){
    idAlbum = sqlite3_column_int(stmt, 0);
  }else if(resultado != SQLITE_DONE){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("Error al obtener la id del album: ") + sqlite3_errmsg(bd));
  }

  sqlite3_finalize(stmt);
  return idAlbum;
}
 
std::vector<Album> AlbumsDAO::obtenerAlbums(){
  if(!admin.estaAbierta())
    throw std::runtime_error("La base de datos no esta abierta.");

  sqlite3* bd = admin.getBaseDeDatos();
  const char* sql = "SELECT id_album, name FROM albums";
  sqlite3_stmt* stmt = nullptr;

  if(sqlite3_prepare_v2(bd, sql, -1, &stmt, nullptr) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo obtener los albums: ") + sqlite3_errmsg(bd));
  }

  std::vector<Album> albums;

  int resultado;

  while((resultado = sqlite3_step(stmt)) == SQLITE_ROW){
    int id = sqlite3_column_int(stmt, 0);
    std::string nombre;
    
    const unsigned char* texto = sqlite3_column_text(stmt, 1);
    int longitud = sqlite3_column_bytes(stmt, 1);

    if(texto == nullptr)
      nombre = "Unknown";
    else if(id == 0)
      nombre = "Sin Album";
    else
      nombre.assign(reinterpret_cast<const char*>(texto), longitud);

    albums.push_back(Album(id, "", nombre, 0));
  }

  if(resultado != SQLITE_DONE){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("Error al recorrer los albums: ") + sqlite3_errmsg(bd));
  }

  sqlite3_finalize(stmt);
  return albums;
}

void AlbumsDAO::eliminarAlbum(int id){
  if(!admin.estaAbierta())
    throw std::runtime_error("La base de datos no esta abierta.");

  if(id < 1)
    throw std::invalid_argument("No se pudo eliminar el album, la id del album no es valida.");
  
  sqlite3* bd = admin.getBaseDeDatos();
  const char* sql = "DELETE FROM albums WHERE id_album = ?";
  sqlite3_stmt* stmt = nullptr;

  if(sqlite3_prepare_v2(bd, sql, -1, &stmt, nullptr) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo eliminar el album: ") + sqlite3_errmsg(bd));
  }

  if((sqlite3_bind_int(stmt, 1, id)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo eliminar el album: ") + sqlite3_errmsg(bd));
  }

  int resultado = sqlite3_step(stmt);

  if(sqlite3_changes(bd) == 0){
    sqlite3_finalize(stmt);
    throw std::runtime_error("No se elimino ningun album.");
  }

  if(resultado != SQLITE_DONE){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("Error al eliminar el album: ") + sqlite3_errmsg(bd));
  }

  sqlite3_finalize(stmt);
}
