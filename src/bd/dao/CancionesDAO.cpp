/**
 * @file CancionesDAO.cpp
 * @brief Implementación de la clase CancionesDAO.
 */

#include "proyecto2/bd/dao/CancionesDAO.hpp"

CancionesDAO::CancionesDAO(AdministraBD& admin) : admin(admin){}

bool CancionesDAO::existeCancion(const std::filesystem::path& cancion) const{
  if(!admin.estaAbierta())
    throw std::runtime_error("La base de datos no esta abierta.");

  std::string ruta = cancion.string();

  if(ruta.empty())
    return false;

  if(!std::filesystem::exists(cancion))
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

int CancionesDAO::insertarCancion(const Metadatos& datos, int idArtista, int idAlbum) const{
  if(!admin.estaAbierta())
    throw std::runtime_error("La base de datos no esta abierta.");

  if(!std::filesystem::exists(datos.getRuta()) || datos.getRuta().string().empty())
    throw std::invalid_argument("No se pudo insertar la canción, tiene una ruta no valida");

  std::filesystem::path ruta = datos.getRuta();

  if(existeCancion(ruta))
    return -1;

  std::string rutaCancion = ruta.string();

  if(idArtista < 0)
    throw std::invalid_argument("No se pudo insertar la canción, el id de el artista no es valido.");

  if(idAlbum < 0)
    throw std::invalid_argument("No se pudo insertar la canción, el id de el album no es valido.");

  std::string titulo = datos.getTitulo();
  if(titulo.empty())
    titulo = datos.getRuta().stem().string();

  std::string genero = datos.getGenero();;

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

  if((sqlite3_bind_int(stmt, 6, datos.getAnio())) != SQLITE_OK){
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

  int idCancion = static_cast<int>(sqlite3_last_insert_rowid(bd));

  sqlite3_finalize(stmt);

  return idCancion;
}

void CancionesDAO::actualizarCancion(const Metadatos& datos, int idArtista, int idAlbum){
  if(!admin.estaAbierta())
    throw std::runtime_error("La base de datos no esta abierta.");

  std::string ruta = datos.getRuta().string();
  
  if(ruta.empty())
    throw std::invalid_argument("No se pudo actualizar la canción, tiene una ruta no valida.");

  if(!std::filesystem::exists(std::filesystem::path(ruta)))
    throw std::invalid_argument("No se pudo actualizar la canción, la ruta no existe.");

  std::string titulo = datos.getTitulo();
  
  if(titulo.empty())
    throw std::invalid_argument("No se pudo actualizar la canción, tiene un nombre no valido.");

  int id = datos.getId();

  if(id < 1)
    throw std::invalid_argument("No se pudo actualizar la canción, la id no es valida.");

  if(idArtista < 0)
    throw std::invalid_argument("No se pudo actualizar la canción, el id del artista no es valido.");

  if(idAlbum < 0)
    throw std::invalid_argument("No se pudo actualizar la canción, el id del album no es valido.");

  int pista = datos.getPista();
  int anio = datos.getAnio();
  std::string genero = datos.getGenero();
  
  sqlite3* bd = admin.getBaseDeDatos();
  const char* sql = "UPDATE rolas SET id_performer = ?, id_album = ?, path = ?, title = ?, track = ?, year = ?, genre = ? "
    "WHERE id_rola = ?";
  sqlite3_stmt* stmt = nullptr;

  if(sqlite3_prepare_v2(bd, sql, -1, &stmt, nullptr) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo actualizar la canción: ") + sqlite3_errmsg(bd));
  }

  if((sqlite3_bind_int(stmt, 1, idArtista)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo actualizar la canción: ") + sqlite3_errmsg(bd));
  }

  if((sqlite3_bind_int(stmt, 2, idAlbum)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo actualizar la canción: ") + sqlite3_errmsg(bd));
  }

  if((sqlite3_bind_text(stmt, 3, ruta.c_str(), -1, SQLITE_STATIC)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo actualizar la canción: ") + sqlite3_errmsg(bd));
  }
  
  if((sqlite3_bind_text(stmt, 4, titulo.c_str(), -1, SQLITE_STATIC)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo actualizar la canción: ") + sqlite3_errmsg(bd));
  }

  if((sqlite3_bind_int(stmt, 5, pista)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo actualizar la canción: ") + sqlite3_errmsg(bd));
  }

  if((sqlite3_bind_int(stmt, 6, anio)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo actualizar la canción: ") + sqlite3_errmsg(bd));
  }

  if((sqlite3_bind_text(stmt, 7, genero.c_str(), -1, SQLITE_STATIC)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo actualizar la canción: ") + sqlite3_errmsg(bd));
  }

  if((sqlite3_bind_int(stmt, 8, id)) != SQLITE_OK){
    sqlite3_finalize(stmt);

    throw std::runtime_error(std::string("No se pudo actualizar la canción: ") + sqlite3_errmsg(bd));
  }

  int resultado = sqlite3_step(stmt);

  if(sqlite3_changes(bd) == 0){
    sqlite3_finalize(stmt);
    throw std::runtime_error("No se actualizo ninguna canción.");
  }

  if(resultado != SQLITE_DONE){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("Error al actualizar la canción: ") + sqlite3_errmsg(bd));
  }

  sqlite3_finalize(stmt);
}

std::vector<Metadatos> CancionesDAO::obtenerCanciones(){
  if(!admin.estaAbierta())
    throw std::runtime_error("La base de datos no esta abierta.");

  sqlite3* bd = admin.getBaseDeDatos();
  const char* sql = "SELECT rolas.id_rola, rolas.title, performers.name, albums.name, rolas.track, rolas.year, rolas.genre "
    "FROM rolas LEFT JOIN performers ON rolas.id_performer = performers.id_performer "
    "LEFT JOIN albums ON rolas.id_album = albums.id_album ORDER BY rolas.title ASC";
  sqlite3_stmt* stmt = nullptr;

  if(sqlite3_prepare_v2(bd, sql, -1, &stmt, nullptr) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo obtener las canciones: ") + sqlite3_errmsg(bd));
  }

  std::vector<Metadatos> canciones;

  int resultado;

  while((resultado = sqlite3_step(stmt)) == SQLITE_ROW){
    std::string titulo;
    std::string artista;
    std::string album;
    std::string genero;

    int id = sqlite3_column_int(stmt, 0);
    
    const unsigned char* texto = sqlite3_column_text(stmt, 1);
    int longitud = sqlite3_column_bytes(stmt, 1);

    if(texto == nullptr)
      titulo = "";
    else
      titulo.assign(reinterpret_cast<const char*>(texto), longitud);

    texto = sqlite3_column_text(stmt, 2);
    longitud = sqlite3_column_bytes(stmt, 2);

    if(texto == nullptr)
      artista = "";
    else
      artista.assign(reinterpret_cast<const char*>(texto), longitud);

    texto = sqlite3_column_text(stmt, 3);
    longitud = sqlite3_column_bytes(stmt, 3);

    if(texto == nullptr)
      album = "";
    else
      album.assign(reinterpret_cast<const char*>(texto), longitud);

    int pista = sqlite3_column_int(stmt, 4);
    
    int anio = sqlite3_column_int(stmt, 5);

    texto = sqlite3_column_text(stmt, 6);
    longitud = sqlite3_column_bytes(stmt, 6);

    if(texto == nullptr)
      genero = "";
    else
      genero.assign(reinterpret_cast<const char*>(texto), longitud);
    
    Metadatos metadatos = Metadatos::Builder()
      .setId(id)
      .setTitulo(titulo)
      .setArtista(artista)
      .setAlbum(album)
      .setPista(pista)
      .setAnio(anio)
      .setGenero(genero)
      .build();

    canciones.push_back(metadatos);
  }

  if(resultado != SQLITE_DONE){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("Error al recorrer las canciones: ") + sqlite3_errmsg(bd));
  }

  sqlite3_finalize(stmt);
  return canciones;
}

std::string CancionesDAO::obtenerRuta(int id) const{
  if(!admin.estaAbierta())
    throw std::runtime_error("La base de datos no esta abierta.");

  if(id < 1)
    throw std::invalid_argument("No se pudo obtener la ruta de la canción, id no valida");

  sqlite3* bd = admin.getBaseDeDatos();
  const char* sql = "SELECT path FROM rolas WHERE id_rola = ?";
  sqlite3_stmt* stmt = nullptr;

  if(sqlite3_prepare_v2(bd, sql, -1, &stmt, nullptr) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo obtener la ruta de la canción: ") + sqlite3_errmsg(bd));
  }
  
  if((sqlite3_bind_int(stmt, 1, id)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo obtener la ruta de la canción: ") + sqlite3_errmsg(bd));
  }

  int r = sqlite3_step(stmt);
  std::string ruta;

  if(r == SQLITE_ROW){
    const unsigned char* texto = sqlite3_column_text(stmt, 0);
    int longitud = sqlite3_column_bytes(stmt, 0);

    if(texto == nullptr){
      sqlite3_finalize(stmt);
      throw std::runtime_error(std::string("No hay ninguna canción con la id ingresada"));
    }else
      ruta.assign(reinterpret_cast<const char*>(texto), longitud);
    
  }else if(r == SQLITE_DONE){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No hay ninguna canción con la id ingresada"));
  }else{
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo obtener la ruta de la canción: ") + sqlite3_errmsg(bd));
  }

  if(ruta.empty()){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("La canción no tiene una ruta"));
  }

  sqlite3_finalize(stmt);
  return ruta;
}

void CancionesDAO::actualizarAlbumEnCancion(int idCancion, int idAlbum){
  if(!admin.estaAbierta())
    throw std::runtime_error("La base de datos no esta abierta.");

  if(idCancion < 1)
    throw std::invalid_argument("El id de la canción no es valido.");

  if(idAlbum < 0)
    throw std::invalid_argument("El id del album no es valido.");

  sqlite3* bd = admin.getBaseDeDatos();
  const char* sql = "UPDATE rolas SET id_album = ? WHERE id_rola = ?";
  sqlite3_stmt* stmt = nullptr;

  if(sqlite3_prepare_v2(bd, sql, -1, &stmt, nullptr) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo actualizar el album de la canción: ") + sqlite3_errmsg(bd));
  }

  if((sqlite3_bind_int(stmt, 1, idAlbum)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo actualizar el album de la canción: ") + sqlite3_errmsg(bd));
  }

  if((sqlite3_bind_int(stmt, 2, idCancion)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo actualizar el album de la canción: ") + sqlite3_errmsg(bd));
  }

  int resultado = sqlite3_step(stmt);

  if(sqlite3_changes(bd) == 0){
    sqlite3_finalize(stmt);
    throw std::runtime_error("No se actualizo ninguna canción.");
  }

  if(resultado != SQLITE_DONE){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("Error al actualizar el album de la canción: ") + sqlite3_errmsg(bd));
  }

  sqlite3_finalize(stmt);
}

void CancionesDAO::actualizarArtistaEnCancion(int idCancion, int idArtista){
  if(!admin.estaAbierta())
    throw std::runtime_error("La base de datos no esta abierta.");

  if(idCancion < 1)
    throw std::invalid_argument("El id de la canción no es valido.");

  if(idArtista < 0)
    throw std::invalid_argument("El id del artista no es valido.");

  sqlite3* bd = admin.getBaseDeDatos();
  const char* sql = "UPDATE rolas SET id_performer = ? WHERE id_rola = ?";
  sqlite3_stmt* stmt = nullptr;

  if(sqlite3_prepare_v2(bd, sql, -1, &stmt, nullptr) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo actualizar el artista de la canción: ") + sqlite3_errmsg(bd));
  }

  if((sqlite3_bind_int(stmt, 1, idArtista)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo actualizar el artista de la canción: ") + sqlite3_errmsg(bd));
  }

  if((sqlite3_bind_int(stmt, 2, idCancion)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo actualizar el artista de la canción: ") + sqlite3_errmsg(bd));
  }

  int resultado = sqlite3_step(stmt);

  if(sqlite3_changes(bd) == 0){
    sqlite3_finalize(stmt);
    throw std::runtime_error("No se actualizo ninguna canción.");
  }

  if(resultado != SQLITE_DONE){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("Error al actualizar el artista de la canción: ") + sqlite3_errmsg(bd));
  }

  sqlite3_finalize(stmt);
}

Metadatos CancionesDAO::obtenerDatosCancion(int id){
  if(!admin.estaAbierta())
    throw std::runtime_error("La base de datos no esta abierta.");

  if(id < 1)
    throw std::invalid_argument("El id no es valido.");

  sqlite3* bd = admin.getBaseDeDatos();
  const char* sql = "SELECT rolas.id_rola, rolas.path, rolas.title, performers.name, "
    "albums.name, rolas.track, rolas.year, rolas.genre, performers.id_performer, albums.id_album "
    "FROM rolas LEFT JOIN performers ON rolas.id_performer = performers.id_performer "
    "LEFT JOIN albums ON rolas.id_album = albums.id_album WHERE rolas.id_rola = ?";
  sqlite3_stmt* stmt = nullptr;

  if(sqlite3_prepare_v2(bd, sql, -1, &stmt, nullptr) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudieron obtener los datos de la canción: ") + sqlite3_errmsg(bd));
  }

  if((sqlite3_bind_int(stmt, 1, id)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudieron obtener los datos de la canción: ") + sqlite3_errmsg(bd));
  }

  int resultado = sqlite3_step(stmt);

  int idCancion = 0;
  std::string ruta;
  std::string titulo;
  std::string artista;
  std::string album;
  int pista = 0;
  int anio = 0;
  std::string genero;
  
  if(resultado == SQLITE_ROW){
    idCancion = sqlite3_column_int(stmt, 0);
    
    const unsigned char* texto = sqlite3_column_text(stmt, 1);
    int longitud = sqlite3_column_bytes(stmt, 1);

    if(texto == nullptr)
      ruta = "";
    else
      ruta.assign(reinterpret_cast<const char*>(texto), longitud);

    texto = sqlite3_column_text(stmt, 2);
    longitud = sqlite3_column_bytes(stmt, 2);

    if(texto == nullptr)
      titulo = "";
    else
      titulo.assign(reinterpret_cast<const char*>(texto), longitud);

    texto = sqlite3_column_text(stmt, 3);
    longitud = sqlite3_column_bytes(stmt, 3);

    if(texto == nullptr)
      artista = "Unknown";
    else
      artista.assign(reinterpret_cast<const char*>(texto), longitud);

    texto = sqlite3_column_text(stmt, 4);
    longitud = sqlite3_column_bytes(stmt, 4);

    if(texto == nullptr)
      album = "Unknown";
    else
      album.assign(reinterpret_cast<const char*>(texto), longitud);
    
    pista = sqlite3_column_int(stmt, 5);
    anio = sqlite3_column_int(stmt, 6);

    texto = sqlite3_column_text(stmt, 7);
    longitud = sqlite3_column_bytes(stmt, 7);
    
    if(texto == nullptr)
      genero = "Unknown";
    else
      genero.assign(reinterpret_cast<const char*>(texto), longitud);
    
  }else if(resultado != SQLITE_DONE){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("Error al obtener los datos de la canción: ") + sqlite3_errmsg(bd));
  }

  sqlite3_finalize(stmt);
  return Metadatos::Builder()
    .setId(idCancion)
    .setRuta(std::filesystem::path(ruta))
    .setTitulo(titulo)
    .setArtista(artista)
    .setAlbum(album)
    .setPista(pista)
    .setAnio(anio)
    .setGenero(genero)
    .build();
}

void CancionesDAO::eliminarCancion(int id){
  if(!admin.estaAbierta())
    throw std::runtime_error("La base de datos no esta abierta.");
  
  if(id < 1)
    throw std::invalid_argument("No se pudo eliminar la canción, la id de la canción no es valida.");
  
  sqlite3* bd = admin.getBaseDeDatos();
  const char* sql = "DELETE FROM rolas WHERE id_rola = ?";
  sqlite3_stmt* stmt = nullptr;

  if(sqlite3_prepare_v2(bd, sql, -1, &stmt, nullptr) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo eliminar la canción: ") + sqlite3_errmsg(bd));
  }

  if((sqlite3_bind_int(stmt, 1, id)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo eliminar la canción: ") + sqlite3_errmsg(bd));
  }

  int resultado = sqlite3_step(stmt);

  if(sqlite3_changes(bd) == 0){
    sqlite3_finalize(stmt);
    throw std::runtime_error("No se elimino ninguna canción.");
  }

  if(resultado != SQLITE_DONE){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("Error al eliminar la canción: ") + sqlite3_errmsg(bd));
  }

  sqlite3_finalize(stmt);  
}

std::vector<int> CancionesDAO::cancionesDeArtista(int idArtista){
  if(!admin.estaAbierta())
    throw std::runtime_error("La base de datos no esta abierta.");

  if(idArtista < 1)
    throw std::invalid_argument("La id del artista no es valido.");
  
  sqlite3* bd = admin.getBaseDeDatos();
  const char* sql = "SELECT id_rola FROM rolas WHERE id_performer = ?";
  sqlite3_stmt* stmt = nullptr;

  if(sqlite3_prepare_v2(bd, sql, -1, &stmt, nullptr) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo obtener las canciones del artista: ") + sqlite3_errmsg(bd));
  }

  if((sqlite3_bind_int(stmt, 1, idArtista)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo obtener las canciones del artista: ") + sqlite3_errmsg(bd));
  }

  int resultado;
  std::vector<int> ids;

  try {
    while((resultado = sqlite3_step(stmt)) == SQLITE_ROW){
      int idCancion = sqlite3_column_int(stmt, 0);
      ids.push_back(idCancion);
    }
  }
  catch(...) {
    sqlite3_finalize(stmt);
    throw;
  }

  if(resultado != SQLITE_DONE){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("Error al obtener las canciones del artista: ") + sqlite3_errmsg(bd));
  }

  sqlite3_finalize(stmt);
  return ids;
}

std::vector<int> CancionesDAO::cancionesDeAlbum(int idAlbum){
  if(!admin.estaAbierta())
    throw std::runtime_error("La base de datos no esta abierta.");

  if(idAlbum < 1)
    throw std::invalid_argument("La id del album no es valido.");
  
  sqlite3* bd = admin.getBaseDeDatos();
  const char* sql = "SELECT id_rola FROM rolas WHERE id_album = ?";
  sqlite3_stmt* stmt = nullptr;

  if(sqlite3_prepare_v2(bd, sql, -1, &stmt, nullptr) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo obtener las canciones del album: ") + sqlite3_errmsg(bd));
  }

  if((sqlite3_bind_int(stmt, 1, idAlbum)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo obtener las canciones del album: ") + sqlite3_errmsg(bd));
  }

  int resultado;
  std::vector<int> ids;

  try {
    while((resultado = sqlite3_step(stmt)) == SQLITE_ROW){
      int idCancion = sqlite3_column_int(stmt, 0);
      ids.push_back(idCancion);
    }
  }
  catch(...) {
    sqlite3_finalize(stmt);
    throw;
  }

  if(resultado != SQLITE_DONE){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("Error al obtener las canciones del album: ") + sqlite3_errmsg(bd));
  }

  sqlite3_finalize(stmt);
  return ids;
}
