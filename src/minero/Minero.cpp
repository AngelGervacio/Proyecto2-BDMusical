/**
 * @file Minero.cpp
 * @brief Implementación de la clase Minero.
 */

#include "proyecto2/minero/Minero.hpp"

Minero::Minero(BaseDatosDAO& dao) : dao(dao){}

void Minero::recorreDirectorio(const std::filesystem::path& directorio) const{
  if(directorio.empty())
    throw std::invalid_argument("No es un directorio valido");
  
  if(!std::filesystem::exists(directorio))
    throw std::invalid_argument("El directorio no existe");

  if(!std::filesystem::is_directory(directorio))
    throw std::invalid_argument("La ruta no es un directorio");
  
  for(auto& archivo : std::filesystem::recursive_directory_iterator(directorio)){
    if(archivo.path().extension() == ".mp3"){
      std::cout << "Minando " << archivo.path().filename() << "\n";
      registrarCancion(archivo.path());
    }
  }
}

void Minero::registrarCancion(const std::filesystem::path& cancion) const{
  if(dao.existeRuta(cancion))
    return;

  TagLib::FileRef archivo(cancion.c_str());

  if(archivo.isNull() || archivo.tag() == nullptr)
    return;

  std::string titulo = archivo.tag()->title().to8Bit(true);
  std::string artista = archivo.tag()->artist().to8Bit(true);
  std::string album = archivo.tag()->album().to8Bit(true);
  int pista = static_cast<int>(archivo.tag()->track());
  int anio = static_cast<int>(archivo.tag()->year());
  std::string genero = archivo.tag()->genre().to8Bit(true);

  int id = dao.insertarCancion(Metadatos::Builder()
			       .setRuta(cancion)
			       .setTitulo(titulo)
			       .setArtista(artista)
			       .setAlbum(album)
			       .setPista(pista)
			       .setAnio(anio)
			       .setGenero(genero)
			       .build());

  int idArtista = (artista.empty()) ? -1 : dao.obtenerArtista(artista);

  if(id != -1)
    extraerImagen(cancion, id, TagLib::ID3v2::AttachedPictureFrame::FrontCover, obtenerDirectorioCache());

  if(idArtista > 0)
    extraerImagen(cancion, idArtista, TagLib::ID3v2::AttachedPictureFrame::Artist, obtenerDirectorioCache());
}

void Minero::extraerImagen(const std::filesystem::path& cancion, int id, TagLib::ID3v2::AttachedPictureFrame::Type tipo,
			  const std::filesystem::path& directorio) const{
  TagLib::MPEG::File archivo(cancion.c_str());

  if(!archivo.isValid())
    return;

  TagLib::ID3v2::Tag* tag = archivo.ID3v2Tag();

  if(tag == nullptr)
    return;

  std::string stringTipo;

  if(tipo == TagLib::ID3v2::AttachedPictureFrame::FrontCover)
    stringTipo = "Portada";
  else if(tipo == TagLib::ID3v2::AttachedPictureFrame::Artist)
    stringTipo = "Artista";
  else
    return;

  TagLib::ID3v2::FrameList frames = tag->frameListMap()["APIC"];

  for(TagLib::ID3v2::Frame* frame : frames){
    auto* imagen = dynamic_cast<TagLib::ID3v2::AttachedPictureFrame*>(frame);

    if(imagen == nullptr || imagen->type() != tipo)
      continue;

    TagLib::ByteVector datos = imagen->picture();

    if(datos.isEmpty())
      return;

    std::string mime = imagen->mimeType().to8Bit(true);
    std::string extension;

    if(mime == "image/jpeg")
      extension = ".jpg";
    else if(mime == "image/png")
      extension = ".png";
    else if(mime == "image/webp")
      extension = ".webp";
    else
      return;

    std::filesystem::path ruta = directorio / (std::to_string(id) + stringTipo + extension);
    std::ofstream archivoImagen(ruta, std::ios::binary);

    if(!archivoImagen)
      return;

    archivoImagen.write(datos.data(), datos.size());

    if(!archivoImagen)
      return;
  }
}

std::filesystem::path Minero::obtenerDirectorioCache() const{
  const char* xdg = std::getenv("XDG_CACHE_HOME");
  std::filesystem::path directorio;

  if(xdg == nullptr || *xdg == '\0' || !std::filesystem::path(xdg).is_absolute()){
    xdg = std::getenv("HOME");
    if(xdg == nullptr || *xdg == '\0'){
      throw std::runtime_error("No se pudo determinar el directorio del usuario.");
    }
    directorio = std::filesystem::path(xdg) / ".cache";
  }else{
    directorio = xdg;
  }

  directorio /= "proyecto2";

  try{
    std::filesystem::create_directories(directorio);
  }catch(std::filesystem::filesystem_error&){
    throw std::runtime_error("No se pudo acceder al directorio de cache.");
  }

  return directorio;
}

void Minero::minar(const std::filesystem::path& directorio) const{
  recorreDirectorio(directorio);
}
