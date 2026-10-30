/**
 * @file BaseDatosDAO.cpp
 * @brief Implementación de la clase BaseDatosDAO.
 */

#include "proyecto2/bd/BaseDatosDAO.hpp"

BaseDatosDAO::BaseDatosDAO(AdministraBD& admin) : admin(admin){}

bool BaseDatosDAO::existeRuta(const std::filesystem::path& cancion) const{
  if(!admin.estaAbierta())
    throw std::runtime_error("La base de datos no esta abierta.");

  if(!std::filesystem::exists(cancion))
    return false;

  std::string ruta = cancion.string();

  if(ruta.empty())
    return false;

  sqlite3* bd = admin.getBaseDeDatos();
  const char* sql = "SELECT 1 FROM rolas WHERE path = ? LIMIT 1";
  sqlite3_stmt* stmt = nullptr;
  bool existe = false;

  if(sqlite3_prepare_v2(bd, sql, -1, &stmt, nullptr) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo realizar la busqueda por ruta: ") + sqlite3_errmsg(bd));
  }

  if((sqlite3_bind_text(stmt, 1, ruta.c_str(), -1, SQLITE_STATIC)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo realizar la busqueda por ruta: ") + sqlite3_errmsg(bd));
  }

  int r = sqlite3_step(stmt);

  if(r == SQLITE_ROW)
    existe = true;
  else if(r != SQLITE_DONE){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo realizar la busqueda por ruta: ") + sqlite3_errmsg(bd));
  }

  sqlite3_finalize(stmt);
  return existe;
}

int BaseDatosDAO::obtenerArtista(const std::string& artista) const{
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

int BaseDatosDAO::insertarArtista(const std::string& artista) const{
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

int BaseDatosDAO::obtenerAlbum(const std::string& ruta) const{
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

int BaseDatosDAO::insertarAlbum(const std::string& ruta, const std::string& nombre, int año) const{
  if(!admin.estaAbierta())
    throw std::runtime_error("La base de datos no esta abierta.");

  if(ruta.empty())
    throw std::invalid_argument("No se pudo insertar el album, tiene una ruta no valida");

  if(nombre.empty())
    throw std::invalid_argument("No se pudo insertar el album, tiene un nombre no valido");

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

  if((sqlite3_bind_int(stmt, 3, año)) != SQLITE_OK){
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

void BaseDatosDAO::insertarCancion(const Metadatos& datos) const{
  if(!admin.estaAbierta())
    throw std::runtime_error("La base de datos no esta abierta.");

  if(!std::filesystem::exists(datos.getRuta()) || datos.getRuta().string().empty())
    throw std::invalid_argument("No se pudo insertar la canción, tiene una ruta no valida");

  std::filesystem::path ruta = datos.getRuta();

  if(existeRuta(ruta))
    return;

  std::string rutaCancion = ruta.string();

  int idArtista;
  if(datos.getArtista().empty())
    idArtista = insertarArtista("Unknown");
  else
    idArtista = insertarArtista(datos.getArtista());

  std::filesystem::path rutaAlbum = ruta.parent_path();

  int idAlbum = 0;
  if(!datos.getAlbum().empty())
    idAlbum = insertarAlbum(rutaAlbum.string(), datos.getAlbum(), datos.getAño());

  std::string titulo = datos.getTitulo();
  if(titulo.empty())
    titulo = datos.getRuta().stem().string();

  std::string genero = datos.getGenero();
  if(genero.empty())
    genero = "Unknown";

  sqlite3* bd = admin.getBaseDeDatos();

  const char* sql = "INSERT INTO rolas (id_performer, id_album, path, title, track, year, genre) VALUES (?, ?, ?, ?, ?, ?, ?)";
  sqlite3_stmt* stmt = nullptr;

  if(sqlite3_prepare_v2(bd, sql, -1, &stmt, nullptr) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo insertar la canción: ") + sqlite3_errmsg(bd));
  }

  if((sqlite3_bind_int(stmt, 1, idArtista)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo insertar la canción: ") + sqlite3_errmsg(bd));
  }

  if((sqlite3_bind_int(stmt, 2, idAlbum)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo insertar la canción: ") + sqlite3_errmsg(bd));
  }

  if((sqlite3_bind_text(stmt, 3, rutaCancion.c_str(), -1, SQLITE_STATIC)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo insertar la canción: ") + sqlite3_errmsg(bd));
  }

  if((sqlite3_bind_text(stmt, 4, titulo.c_str(), -1, SQLITE_STATIC)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo insertar la canción: ") + sqlite3_errmsg(bd));
  }

  if((sqlite3_bind_int(stmt, 5, datos.getPista())) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo insertar la canción: ") + sqlite3_errmsg(bd));
  }

  if((sqlite3_bind_int(stmt, 6, datos.getAño())) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo insertar la canción: ") + sqlite3_errmsg(bd));
  }

  if((sqlite3_bind_text(stmt, 7, genero.c_str(), -1, SQLITE_STATIC)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo insertar la canción: ") + sqlite3_errmsg(bd));
  }

  int r = sqlite3_step(stmt);

  if(r != SQLITE_DONE){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo insertar la canción: ") + sqlite3_errmsg(bd));
  }

  sqlite3_finalize(stmt);
}
