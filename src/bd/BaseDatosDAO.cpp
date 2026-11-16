/**
 * @file BaseDatosDAO.cpp
 * @brief Implementación de la clase BaseDatosDAO.
 */

#include "proyecto2/bd/BaseDatosDAO.hpp"

BaseDatosDAO::BaseDatosDAO(AdministraBD& admin) : admin(admin){}

bool BaseDatosDAO::existeRuta(const std::filesystem::path& cancion) const{
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

int BaseDatosDAO::insertarAlbum(const std::string& ruta, const std::string& nombre, int anio) const{
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

int BaseDatosDAO::insertarCancion(const Metadatos& datos) const{
  if(!admin.estaAbierta())
    throw std::runtime_error("La base de datos no esta abierta.");

  if(!std::filesystem::exists(datos.getRuta()) || datos.getRuta().string().empty())
    throw std::invalid_argument("No se pudo insertar la canción, tiene una ruta no valida");

  std::filesystem::path ruta = datos.getRuta();

  if(existeRuta(ruta))
    return -1;

  std::string rutaCancion = ruta.string();

  int idArtista;
  if(datos.getArtista().empty())
    idArtista = 0;
  else
    idArtista = insertarArtista(datos.getArtista());

  std::filesystem::path rutaAlbum = ruta.parent_path();

  int idAlbum = 0;
  if(!datos.getAlbum().empty())
    idAlbum = insertarAlbum(rutaAlbum.string(), datos.getAlbum(), datos.getAnio());

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

std::vector<Metadatos> BaseDatosDAO::obtenerCanciones(){
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

std::string BaseDatosDAO::obtenerRuta(int id) const{
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

int BaseDatosDAO::obtenerPersona(const std::string& artistico) const{
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

int BaseDatosDAO::insertarPersona(const std::string& artistico, const std::string& real, const std::string& nacimiento,
				  const std::string& fallecimiento){
  if(!admin.estaAbierta())
    throw std::runtime_error("La base de datos no esta abierta.");

  if(artistico.empty())
    throw std::invalid_argument("Nombre de persona no valido, no se pudo insertar");

  int idPersona = obtenerPersona(artistico);

  if(idPersona != -1)
    return -1;

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

void BaseDatosDAO::actualizarPersona(int id, const std::string& artistico, const std::string& real, const std::string& nacimiento,
		       const std::string& fallecimiento){
  if(!admin.estaAbierta())
    throw std::runtime_error("La base de datos no esta abierta.");
  
  if(id < 1)
    throw std::invalid_argument("No se pudo actualizar a la persona, la id no es valida.");

  if(artistico.empty())
    throw std::invalid_argument("No se pudo actualizar a la persona, tiene un nombre no valido.");

  int idExiste = obtenerPersona(artistico);

  if(idExiste != -1)
    throw std::invalid_argument("Ya existe una persona con ese nombre.");
  
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

Album BaseDatosDAO::obtenerDatosAlbum(int id){
  if(!admin.estaAbierta())
    throw std::runtime_error("La base de datos no esta abierta.");

  if(id < 1)
    throw std::invalid_argument("El id no es valido.");

  sqlite3* bd = admin.getBaseDeDatos();
  const char* sql = "SELECT albums.id_album, albums.path, albums.name, albums.year FROM albums INNER JOIN rolas ON rolas.id_album = albums.id_album WHERE rolas.id_rola = ? LIMIT 1";
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

void BaseDatosDAO::actualizarAlbumEnCancion(int idCancion, int idAlbum){
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

void BaseDatosDAO::actualizarAlbum(const std::string& ruta, const std::string& nombre, int anio, int id){
  if(!admin.estaAbierta())
    throw std::runtime_error("La base de datos no esta abierta.");

  if(ruta.empty())
    throw std::invalid_argument("No se pudo actualizar el album, tiene una ruta no valida.");
  
  if(nombre.empty())
    throw std::invalid_argument("No se pudo actualizar el album, tiene un nombre no valido.");

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

void BaseDatosDAO::eliminarAlbum(int id){
  if(!admin.estaAbierta())
    throw std::runtime_error("La base de datos no esta abierta.");

  if(id < 1)
    throw std::invalid_argument("No se pudo eliminar el album, la id del album no es valida.");
  
  sqlite3* bd = admin.getBaseDeDatos();
  const char* sql = "SELECT id_rola FROM rolas WHERE id_album = ?";
  sqlite3_stmt* stmt = nullptr;

  if(sqlite3_prepare_v2(bd, sql, -1, &stmt, nullptr) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo eliminar el album: ") + sqlite3_errmsg(bd));
  }

  if((sqlite3_bind_int(stmt, 1, id)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo eliminar el album: ") + sqlite3_errmsg(bd));
  }

  int resultado;

  try {
    while((resultado = sqlite3_step(stmt)) == SQLITE_ROW){
      int idCancion = sqlite3_column_int(stmt, 0);
      actualizarAlbumEnCancion(idCancion, 0);
    }
  }
  catch(...) {
    sqlite3_finalize(stmt);
    throw;
  }

  if(resultado != SQLITE_DONE){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("Error al eliminar el album: ") + sqlite3_errmsg(bd));
  }

  sqlite3_finalize(stmt);
  stmt = nullptr;

  sql = "DELETE FROM albums WHERE id_album = ?";

  if(sqlite3_prepare_v2(bd, sql, -1, &stmt, nullptr) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo eliminar el album: ") + sqlite3_errmsg(bd));
  }

  if((sqlite3_bind_int(stmt, 1, id)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo eliminar el album: ") + sqlite3_errmsg(bd));
  }

  resultado = sqlite3_step(stmt);

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

Metadatos BaseDatosDAO::obtenerDatosCancion(int id){
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

void BaseDatosDAO::eliminarCancion(int id){
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

void BaseDatosDAO::actualizarCancion(int id, std::string ruta, std::string titulo, std::string artista,
				     int idAlbum, int pista, int anio, std::string genero){

  int idArtista = insertarArtista(artista);

  if(!admin.estaAbierta())
    throw std::runtime_error("La base de datos no esta abierta.");
  
  if(ruta.empty())
    throw std::invalid_argument("No se pudo actualizar la canción, tiene una ruta no valida.");

  if(!std::filesystem::exists(std::filesystem::path(ruta)))
    throw std::invalid_argument("No se pudo actualizar la canción, la ruta no existe.");
  
  if(titulo.empty())
    throw std::invalid_argument("No se pudo actualizar la canción, tiene un nombre no valido.");

  if(id < 1)
    throw std::invalid_argument("No se pudo actualizar la canción, la id del album no es valida.");
  
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

std::vector<Album> BaseDatosDAO::obtenerAlbums(){
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

int BaseDatosDAO::obtenerIdAlbum(int id){
  if(!admin.estaAbierta())
    throw std::runtime_error("La base de datos no esta abierta.");
  
  if(id < 0)
    throw std::invalid_argument("El id no es valido.");

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

  int idAlbum = 0;
  
  if(resultado == SQLITE_ROW){
    idAlbum = sqlite3_column_int(stmt, 0);
  }else if(resultado != SQLITE_DONE){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("Error al obtener la id del album: ") + sqlite3_errmsg(bd));
  }

  sqlite3_finalize(stmt);
  return idAlbum;
}

Artista BaseDatosDAO::obtenerDatosArtista(int id){
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

void BaseDatosDAO::actualizarArtista(int idArtista, int tipo, const std::string& nombre){
  if(!admin.estaAbierta())
    throw std::runtime_error("La base de datos no esta abierta.");
  
  if(nombre.empty())
    throw std::invalid_argument("No se pudo actualizar al artista, tiene un nombre no valido.");

  if(idArtista < 0)
    throw std::invalid_argument("No se pudo actualizar la canción, la id del album no es valida.");
  
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

void BaseDatosDAO::actualizarArtistaEnCancion(int idCancion, int idArtista){
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

void BaseDatosDAO::eliminarArtista(int id){
  if(!admin.estaAbierta())
    throw std::runtime_error("La base de datos no esta abierta.");

  if(id < 1)
    throw std::invalid_argument("No se pudo eliminar al artista, la id del artista no es valido.");
  
  sqlite3* bd = admin.getBaseDeDatos();
  const char* sql = "SELECT id_rola FROM rolas WHERE id_performer = ?";
  sqlite3_stmt* stmt = nullptr;

  if(sqlite3_prepare_v2(bd, sql, -1, &stmt, nullptr) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo eliminar al artista: ") + sqlite3_errmsg(bd));
  }

  if((sqlite3_bind_int(stmt, 1, id)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo eliminar al artista: ") + sqlite3_errmsg(bd));
  }

  int resultado;

  try {
    while((resultado = sqlite3_step(stmt)) == SQLITE_ROW){
      int idCancion = sqlite3_column_int(stmt, 0);
      actualizarArtistaEnCancion(idCancion, 0);
    }
  }
  catch(...) {
    sqlite3_finalize(stmt);
    throw;
  }

  if(resultado != SQLITE_DONE){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("Error al eliminar al artista: ") + sqlite3_errmsg(bd));
  }

  sqlite3_finalize(stmt);
  stmt = nullptr;

  sql = "DELETE FROM performers WHERE id_performer = ?";

  if(sqlite3_prepare_v2(bd, sql, -1, &stmt, nullptr) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo eliminar al artista: ") + sqlite3_errmsg(bd));
  }

  if((sqlite3_bind_int(stmt, 1, id)) != SQLITE_OK){
    sqlite3_finalize(stmt);
    throw std::runtime_error(std::string("No se pudo eliminar al artista: ") + sqlite3_errmsg(bd));
  }

  resultado = sqlite3_step(stmt);

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

Banda BaseDatosDAO::obtenerDatosBanda(const std::string& nombre){
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

void BaseDatosDAO::actualizarBanda(int id, const std::string& nombre, const std::string& formacion, const std::string& separacion){
  if(!admin.estaAbierta())
    throw std::runtime_error("La base de datos no esta abierta.");
  
  if(nombre.empty())
    throw std::invalid_argument("No se pudo actualizar a la banda, tiene un nombre no valido.");

  if(id < 1)
    throw std::invalid_argument("No se pudo actualizar a la banda, la id no es valida.");
  
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

int BaseDatosDAO::obtenerBanda(const std::string& banda) const{
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

int BaseDatosDAO::insertarBanda(const std::string& nombre, const std::string& formacion, const std::string& separacion){
  if(!admin.estaAbierta())
    throw std::runtime_error("La base de datos no esta abierta.");

  if(nombre.empty())
    throw std::invalid_argument("No se pudo insertar a la banda, tiene un nombre no valido");

  int id = obtenerBanda(nombre);
  if(id != -1)
    return id;

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

void BaseDatosDAO::eliminarBanda(int id){
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

std::vector<Persona> BaseDatosDAO::personasEnBanda(int idBanda){
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

std::vector<Persona> BaseDatosDAO::personasDisponibles(int idBanda){
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

void BaseDatosDAO::actualizarMiembros(int idBanda, std::vector<int> ids){
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

void BaseDatosDAO::eliminarPersona(int id){
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

std::vector<Persona> BaseDatosDAO::obtenerPersonas(){
  if(!admin.estaAbierta())
    throw std::runtime_error("La base de datos no esta abierta.");
  
  sqlite3* bd = admin.getBaseDeDatos();
  const char* sql = "SELECT id_person, stage_name, real_name, birth_date, death_date FROM persons ";
  sqlite3_stmt* stmt = nullptr;

  if(sqlite3_prepare_v2(bd, sql, -1, &stmt, nullptr) != SQLITE_OK){
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


