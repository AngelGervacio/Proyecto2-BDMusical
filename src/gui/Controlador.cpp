/**
 * @file Controlador.cpp
 * @brief Implementación de la clase Controlador.
 */

#include "proyecto2/gui/Controlador.hpp"

Controlador::Controlador(CancionesDAO& canciones, ArtistasDAO& artistas, AlbumsDAO& albums, BandasDAO& bandas,
			 PersonasDAO& personas, CancionesModelo& modelo, QObject* parent)
  : QObject(parent), canciones(canciones), artistas(artistas), albums(albums), bandas(bandas), personas(personas),
    modelo(modelo){}

void Controlador::cargarCanciones(){
  const std::vector<Metadatos> datos = canciones.obtenerCanciones();

  QVector<Metadatos> cancionesQt;

  for(const Metadatos& metadatos : datos)
    cancionesQt.append(metadatos);

  modelo.cargarCanciones(cancionesQt);
}

bool Controlador::minando(){
  return estaMinando;
}

QString Controlador::obtenerRuta(int id) const{
  std::string ruta = canciones.obtenerRuta(id);
  return QString::fromUtf8(ruta.data(), ruta.size());
}

void Controlador::minarDirectorio(QString directorio){
  if(estaMinando)
    return;
  
  estaMinando = true;
  emit cambioMinando();
  futuroMinado = QtConcurrent::run([this, directorio](){
    AdministraBD bd;
    bd.abrir();
    CancionesDAO cancionesMinero(bd);
    ArtistasDAO artistasMinero(bd);
    AlbumsDAO albumsMinero(bd);
    Minero minero(cancionesMinero, artistasMinero, albumsMinero);

    minero.minar(std::filesystem::path(directorio.toStdString()));

    QMetaObject::invokeMethod(this, [this]() {
      cargarCanciones();
      estaMinando = false;
      emit cambioMinando();
    }, Qt::QueuedConnection);
  });
}

std::filesystem::path Controlador::obtenerDirectorioCache(){
  const char* xdg = std::getenv("XDG_CACHE_HOME");
  std::filesystem::path destino;
  
  if(xdg == nullptr || *xdg == '\0' || !std::filesystem::path(xdg).is_absolute()){
    xdg = std::getenv("HOME");
    if(xdg == nullptr || *xdg == '\0'){
      throw std::runtime_error("No se pudo determinar el directorio del usuario.");
    }
    destino = std::filesystem::path(xdg) / ".cache";
  }else{
    destino = xdg;
  }

  destino /= "proyecto2";

  try{
    std::filesystem::create_directories(destino);
  }catch(std::filesystem::filesystem_error&){
    throw std::runtime_error("No se pudo acceder al directorio de cache.");
  }

  return destino;
}

void Controlador::agregarImagenEnCache(const QUrl& imagen, int id, std::string sufijo){
  std::filesystem::path ruta = std::filesystem::path(imagen.toLocalFile().toStdString());

  if(ruta.empty())
    return;

  std::filesystem::path destino = obtenerDirectorioCache();

  std::vector<std::string> extensiones = {".png", ".jpg", ".jpeg", ".webp"};

  try{ 
    for(const auto& extension : extensiones){
      std::filesystem::path anterior = destino / (std::to_string(id) + sufijo + extension);

      if(std::filesystem::exists(anterior))
	std::filesystem::remove(anterior);
    }

    destino /= (std::to_string(id) + sufijo + ruta.extension().string());

    std::filesystem::copy_file(ruta, destino, std::filesystem::copy_options::overwrite_existing);
  }catch(std::filesystem::filesystem_error& e){
    throw std::runtime_error("No se pudo guardar la imagen.");
  }
}

QUrl Controlador::obtenerImagenDeCache(int id, std::string sufijo){

  std::filesystem::path imagen = obtenerDirectorioCache();

  imagen /= (std::to_string(id) + sufijo);

  std::vector<std::string> extensiones = {".png", ".jpg", ".jpeg", ".webp"};

  QUrl url;

  for(const auto& extension : extensiones){
    std::filesystem::path ruta = imagen;
    ruta += extension;

    if(std::filesystem::exists(ruta)){
      url = QUrl::fromLocalFile(QString::fromUtf8(ruta.string()));
      break;
    }
  }

  return url;
}

void Controlador::agregarPersona(const QUrl& imagen, const QString& artistico, const QString& real,
				const QString& nacimiento, const QString& fallecimiento){

  std::string nombreArtistico = artistico.toStdString();
  std::string nombreReal = real.toStdString();
  std::string fechaNacimiento = nacimiento.toStdString();
  std::string fechaFallecimiento = fallecimiento.toStdString();

  if(nombreArtistico.empty())
    return;

  Persona persona(-1, nombreArtistico, nombreReal, fechaNacimiento, fechaFallecimiento);

  int id = personas.insertarPersona(persona);

  if(id == -1){
    emit personaYaInsertada();
    return;
  }

  if(imagen.isEmpty())
    return;

  agregarImagenEnCache(imagen, id, "Persona");
}

QVariantMap Controlador::obtenerDatosAlbum(int id){
  if(id < 1)
    return {};

  int idAlbum = albums.obtenerIdAlbum(id);

  if(idAlbum < 1)
    idAlbum = 0;
  
  Album album = albums.obtenerDatosAlbum(idAlbum);

  QVariantMap datos;

  datos["id"] = album.getId();
  datos["ruta"] = QString::fromUtf8(album.getRuta().data(), album.getRuta().size());
  datos["nombre"] = QString::fromUtf8(album.getNombre().data(), album.getNombre().size());
  datos["anio"] = album.getAnio();
  datos["imagen"] = obtenerImagenDeCache(album.getId(), "Album");

  return datos;
}

int Controlador::obtenerIdAlbum(int id){
  if(id < 1)
    return {};
  
  return albums.obtenerIdAlbum(id);
}

void Controlador::agregarAlbum(const QString& ruta, const QString nombre, int anio, int idCancion, const QUrl& imagen){
  try{
    Album album(-1, ruta.toStdString(), nombre.toStdString(), anio);
    int idAlbum = albums.insertarAlbum(album);
    if(!imagen.isEmpty())
      agregarImagenEnCache(imagen, idAlbum, "Album");
    canciones.actualizarAlbumEnCancion(idCancion, idAlbum);
    cargarCanciones();
    void albumActualizado();
  }catch(const std::exception& e){
    emit ocurrioError(QString::fromUtf8(e.what()));
  }
}

void Controlador::actualizarAlbum(const QString& ruta, const QString nombre, int anio, int idAlbum, const QUrl& imagen){
  try{
    Album album(idAlbum, ruta.toStdString(), nombre.toStdString(), anio);
    albums.actualizarAlbum(album);
    if(!imagen.isEmpty())
      agregarImagenEnCache(imagen, idAlbum, "Album");
    cargarCanciones();
    emit albumActualizado();
  }catch(const std::exception& e){
    emit ocurrioError(QString::fromUtf8(e.what()));
  }
}

void Controlador::eliminarAlbum(int idAlbum){
  if(idAlbum < 1){
    emit ocurrioError(QString::fromUtf8("No hay album por eliminar."));
    return;
  }
  
  try{
    std::vector<int> idsCanciones = canciones.cancionesDeAlbum(idAlbum);
    for(const auto& id : idsCanciones){
      canciones.actualizarAlbumEnCancion(id, 0);
    }
    albums.eliminarAlbum(idAlbum);
    cargarCanciones();
    albumActualizado();
  }catch(const std::exception& e){
    emit ocurrioError(QString::fromUtf8(e.what()));
  }
}

QVariantMap Controlador::obtenerDatosCancion(int id){
  if(id < 1)
    return {};

  Metadatos metadatos = canciones.obtenerDatosCancion(id);

  int idArtista = -1;
  if(!metadatos.getArtista().empty())
    idArtista = artistas.obtenerArtista(metadatos.getArtista());

  QVariantMap datos;

  datos["id"] = metadatos.getId();
  datos["ruta"] = QString::fromUtf8(metadatos.getRuta().string().data(), metadatos.getRuta().string().size());
  datos["titulo"] = QString::fromUtf8(metadatos.getTitulo().data(), metadatos.getTitulo().size());
  datos["artista"] = QString::fromUtf8(metadatos.getArtista().data(), metadatos.getArtista().size());
  datos["album"] = QString::fromUtf8(metadatos.getAlbum().data(), metadatos.getAlbum().size());
  datos["pista"] = metadatos.getPista();
  datos["anio"] = metadatos.getAnio();
  datos["genero"] = QString::fromUtf8(metadatos.getGenero().data(), metadatos.getGenero().size());
  datos["imagen"] = obtenerImagenDeCache(metadatos.getId(), "Portada");
  datos["imagenArtista"] = obtenerImagenDeCache(idArtista, "Artista");

  return datos;
}

void Controlador::eliminarCancion(int id){
  if(id < 1){
    emit ocurrioError(QString::fromUtf8("No hay canciones por eliminar."));
    return;
  }
  
  try{
    canciones.eliminarCancion(id);
    cargarCanciones();
    cancionActualizada();
  }catch(const std::exception& e){
    emit ocurrioError(QString::fromUtf8(e.what()));
  }
}

void Controlador::actualizarCancion(int id, const QString& ruta, const QString& titulo, const QString& artista,
				    int idAlbum, int pista, int anio, const QString& genero, const QUrl& imagen){
  try{
    int idArtista = artistas.insertarArtista(artista.toStdString());
    Metadatos metadatos = Metadatos::Builder()
      .setId(id)
      .setRuta(std::filesystem::path(ruta.toStdString()))
      .setTitulo(titulo.toStdString())
      .setArtista(artista.toStdString())
      .setPista(pista)
      .setAnio(anio)
      .setGenero(genero.toStdString())
      .build();
    canciones.actualizarCancion(metadatos, idArtista, idAlbum);
    if(!imagen.isEmpty())
      agregarImagenEnCache(imagen, id, "Portada");
    cargarCanciones();
    emit cancionActualizada();
  }catch(const std::exception& e){
    emit ocurrioError(QString::fromUtf8(e.what()));
  }
}

QVariantList Controlador::obtenerAlbums(){
  QVariantList listaQt;

  std::vector<Album> lista = albums.obtenerAlbums();

  for(const Album& album : lista){
    listaQt.append(QVariantMap{
	{"id", album.getId()},
	{"nombre", QString::fromStdString(album.getNombre())}
      });
  }

  return listaQt;
}

QVariantMap Controlador::obtenerDatosArtista(int id){
  if(id < 1){
    return {};
  }
  
  Artista artista = artistas.obtenerDatosArtista(id);
  
  QVariantMap datos;

  datos["id"] = artista.getId();
  datos["tipo"] = artista.getTipo();
  datos["nombre"] = QString::fromUtf8(artista.getNombre().data(), artista.getNombre().size());
  datos["imagen"] = obtenerImagenDeCache(artista.getId(), "Artista");

  return datos;
}

void Controlador::actualizarArtista(int idArtista, int tipo, const QString& nombre, const QUrl& imagen){
  if(idArtista < 1){
    emit ocurrioError(QString::fromUtf8("No hay artista con esa id."));
    return;
  }

  if(nombre.isEmpty()){
    emit ocurrioError(QString::fromUtf8("El nombre no es valido."));
    return;
  }
  
  try{
    Artista artista(idArtista, tipo, nombre.toStdString());
    artistas.actualizarArtista(artista);
    if(!imagen.isEmpty())
      agregarImagenEnCache(imagen, idArtista, "Artista");
    cargarCanciones();
    emit artistaActualizado();
  }catch(const std::exception& e){
    emit ocurrioError(QString::fromUtf8(e.what()));
  }
}

void Controlador::agregarArtista(int tipo, const QString& nombre, const QUrl& imagen, int idCancion){
  if(tipo < 0 || tipo > 2){
    emit ocurrioError(QString::fromUtf8("Se ingreso un tipo no valido."));
    return;
  }

  if(idCancion < 1){
    emit ocurrioError(QString::fromUtf8("El id de la canción no es valido."));
    return;
  }

  if(nombre.isEmpty()){
    emit ocurrioError(QString::fromUtf8("El nombre no es valido."));
    return;
  }

  try{
    int idArtista = artistas.insertarArtista(nombre.toStdString());
    Artista artista(idArtista, tipo, nombre.toStdString());
    artistas.actualizarArtista(artista);
    if(!imagen.isEmpty())
      agregarImagenEnCache(imagen, idArtista, "Artista");
    canciones.actualizarArtistaEnCancion(idCancion, idArtista);
    cargarCanciones();
    emit artistaActualizado();
  }catch(const std::exception& e){
    emit ocurrioError(QString::fromUtf8(e.what()));
  }
}

void Controlador::eliminarArtista(int idArtista){
  if(idArtista < 1){
    emit ocurrioError(QString::fromUtf8("No hay artista por eliminar."));
    return;
  }

  try{
    std::vector<int> idsCanciones = canciones.cancionesDeArtista(idArtista);
    for(const auto& id : idsCanciones){
      canciones.actualizarArtistaEnCancion(id, 0);
    }
    artistas.eliminarArtista(idArtista);
    cargarCanciones();
    emit artistaActualizado();
  }catch(const std::exception& e){
    emit ocurrioError(QString::fromUtf8(e.what()));
  }
}

QVariantMap Controlador::obtenerDatosBanda(int idArtista, const QString& nombre){
  QVariantMap datos;
  if(idArtista < 1){
    datos["id"] = 0;
    datos["nombre"] = QString::fromStdString("");
    datos["formacion"] = QString::fromStdString("");
    datos["separacion"] = QString::fromStdString("");
    datos["imagen"] = QUrl("");
    return datos;
  }
  
  Banda banda = bandas.obtenerDatosBanda(nombre.toStdString());

  datos["id"] = banda.getId();
  datos["nombre"] = QString::fromUtf8(banda.getNombre().data(), banda.getNombre().size());
  datos["formacion"] = QString::fromUtf8(banda.getFormacion().data(), banda.getFormacion().size());
  datos["separacion"] = QString::fromUtf8(banda.getSeparacion().data(), banda.getSeparacion().size());
  datos["imagen"] = obtenerImagenDeCache(idArtista, "Artista");

  return datos;
}

void Controlador::actualizarBanda(int idBanda, const QString& nombre, const QString& formacion,
				  const QString& separacion, const QVariantList& enBanda, const QVariantList& aEliminar){
  if(idBanda < 1){
    emit ocurrioError(QString::fromUtf8("No hay banda con esa id."));
    return;
  }
  
  if(nombre.isEmpty()){
    emit ocurrioError(QString::fromUtf8("El nombre no es valido."));
    return;
  }

  std::vector<int> ids;

  for(const auto& elemento : enBanda){
    ids.push_back(elemento.toMap()["id"].toInt());
  }

  std::vector<int> idsEliminar;

  for(const auto& elemento : aEliminar){
    idsEliminar.push_back(elemento.toInt());
  }
  
  try{
    Banda banda(idBanda, nombre.toStdString(), formacion.toStdString(), separacion.toStdString());
    bandas.actualizarBanda(banda);
    bandas.actualizarMiembros(idBanda, ids);
    for(const auto& id : idsEliminar){
      personas.eliminarPersona(id);
    }
  }catch(const std::exception& e){
    emit ocurrioError(QString::fromUtf8(e.what()));
  }
}

void Controlador::agregarBanda(const QString& nombre, const QString& formacion,
			       const QString& separacion, const QVariantList& enBanda, const QVariantList& aEliminar){
  if(nombre.isEmpty()){
    emit ocurrioError(QString::fromUtf8("El nombre no es valido."));
    return;
  }

  std::vector<int> ids;

  for(const auto& elemento : enBanda){
    ids.push_back(elemento.toMap()["id"].toInt());
  }

  std::vector<int> idsEliminar;

  for(const auto& elemento : aEliminar){
    idsEliminar.push_back(elemento.toInt());
  }

  try{
    Banda banda(-1, nombre.toStdString(), formacion.toStdString(), separacion.toStdString());
    int id = bandas.insertarBanda(banda);
    bandas.actualizarMiembros(id, ids);
    for(const auto& id : idsEliminar){
      personas.eliminarPersona(id);
    }
  }catch(const std::exception& e){
    emit ocurrioError(QString::fromUtf8(e.what()));
  }
}

void Controlador::eliminarBanda(int id){
  if(id < 1){
    emit ocurrioError(QString::fromUtf8("No hay banda por eliminar."));
    return;
  }

  try{
    bandas.eliminarBanda(id);
  }catch(const std::exception& e){
    emit ocurrioError(QString::fromUtf8(e.what()));
  }
}

QVariantList Controlador::personasEnBanda(int idBanda){
  if(idBanda < 1){
    emit ocurrioError(QString::fromUtf8("No hay bandas con esa id."));
    return {};
  }

  std::vector<Persona> enBanda = bandas.personasEnBanda(idBanda);
  QVariantList lista;

  for(const auto& persona : enBanda){
    lista.append(QVariantMap{
	{"id", persona.getId()},
	{"artistico", QString::fromUtf8(persona.getArtistico())},
	{"real", QString::fromUtf8(persona.getReal())},
	{"nacimiento", QString::fromUtf8(persona.getNacimiento())},
	{"fallecimiento", QString::fromUtf8(persona.getFallecimiento())},
	{"imagen", obtenerImagenDeCache(persona.getId(), "Persona")}
      });
  }

  return lista;
}

QVariantList Controlador::obtenerPersonas(){
  std::vector<Persona> vector = personas.obtenerPersonas();

  QVariantList lista;

  for(const auto& persona : vector){
    lista.append(QVariantMap{
	{"id", persona.getId()},
	{"artistico", QString::fromUtf8(persona.getArtistico())},
	{"real", QString::fromUtf8(persona.getReal())},
	{"nacimiento", QString::fromUtf8(persona.getNacimiento())},
	{"fallecimiento", QString::fromUtf8(persona.getFallecimiento())},
	{"imagen", obtenerImagenDeCache(persona.getId(), "Persona")}
      });
  }  
  return lista;
}

QVariantList Controlador::personasDisponibles(int idBanda){
  if(idBanda < 1){
    emit ocurrioError(QString::fromUtf8("No hay bandas con esa id."));
    return {};
  }

  std::vector<Persona> disponibles = bandas.personasDisponibles(idBanda);
  QVariantList lista;

  for(const auto& persona : disponibles){
    lista.append(QVariantMap{
	{"id", persona.getId()},
	{"artistico", QString::fromUtf8(persona.getArtistico())},
	{"real", QString::fromUtf8(persona.getReal())},
	{"nacimiento", QString::fromUtf8(persona.getNacimiento())},
	{"fallecimiento", QString::fromUtf8(persona.getFallecimiento())},
	{"imagen", obtenerImagenDeCache(persona.getId(), "Persona")}
      });
  }

  return lista;
}

void Controlador::actualizarPersona(int id, const QString& artistico, const QString& real, const QString& nacimiento,
				   const QString& fallecimiento, const QUrl& imagen){
  if(id < 1){
    emit ocurrioError(QString::fromUtf8("No hay persona con esa id."));
    return;
  }
  
  if(artistico.isEmpty()){
    emit ocurrioError(QString::fromUtf8("El nombre no es valido."));
    return;
  }
  
  try{
    Persona persona(id, artistico.toStdString(), real.toStdString(), nacimiento.toStdString(), fallecimiento.toStdString());
    personas.actualizarPersona(persona);
    if(!imagen.isEmpty())
      agregarImagenEnCache(imagen, id, "Persona");
    emit personaActualizada();
  }catch(const std::exception& e){
    emit ocurrioError(QString::fromUtf8(e.what()));
  }
}
