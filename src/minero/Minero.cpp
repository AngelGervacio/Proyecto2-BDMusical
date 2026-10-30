/**
 * @file Minero.cpp
 * @brief Implementación de la clase Minero.
 */

#include "proyecto2/minero/Minero.hpp"

Minero::Minero(BaseDatosDAO& dao) : dao(dao){}

void Minero::recorreDirectorio(const std::filesystem::path& directorio) const{
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

  dao.insertarCancion(Metadatos::Builder()
		      .setRuta(cancion)
		      .setTitulo(archivo.tag()->title().to8Bit(true))
		      .setArtista(archivo.tag()->artist().to8Bit(true))
		      .setAlbum(archivo.tag()->album().to8Bit(true))
		      .setPista(static_cast<int>(archivo.tag()->track()))
		      .setAño(static_cast<int>(archivo.tag()->year()))
		      .setGenero(archivo.tag()->genre().to8Bit(true))
		      .build());
}

void Minero::minar(const std::filesystem::path& directorio) const{
  recorreDirectorio(directorio);
}
