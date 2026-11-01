/**
 * @file Metadatos.cpp
 * @brief Implementación de la clase Metadatos.
 */

#include "proyecto2/bd/Metadatos.hpp"

Metadatos::Builder& Metadatos::Builder::setRuta(const std::filesystem::path& ruta){
  this->ruta = ruta;
  return *this;
}

Metadatos::Builder& Metadatos::Builder::setTitulo(const std::string& titulo){
  this->titulo = titulo;
  return *this;
}

Metadatos::Builder& Metadatos::Builder::setArtista(const std::string& artista){
  this->artista = artista;
  return *this;
}

Metadatos::Builder& Metadatos::Builder::setAlbum(const std::string& album){
  this->album = album;
  return *this;
}

Metadatos::Builder& Metadatos::Builder::setPista(int pista){
  this->pista = pista;
  return *this;
}

Metadatos::Builder& Metadatos::Builder::setAño(int año){
  this->año = año;
  return *this;
}

Metadatos::Builder& Metadatos::Builder::setGenero(const std::string& genero){
  this->genero = genero;
  return *this;
}

Metadatos Metadatos::Builder::build(){
  return Metadatos(ruta, titulo, artista, album, pista, año, genero);
}

Metadatos::Metadatos(const std::filesystem::path& ruta,
		     const std::string& titulo,
		     const std::string& artista,
		     const std::string& album,
		     int pista,
		     int año,
		     const std::string& genero){
  this->ruta = ruta;
  this->titulo = titulo;
  this->artista = artista;
  this->album = album;
  this->pista = pista;
  this->año = año;
  this->genero = genero;
}

const std::filesystem::path& Metadatos::getRuta() const{
  return ruta;
}

std::string Metadatos::getTitulo() const{
  return titulo;
}

std::string Metadatos::getArtista() const{
  return artista;
}

std::string Metadatos::getAlbum() const{
  return album;
}

int Metadatos::getPista() const{
  return pista;
}

int Metadatos::getAño() const{
  return año;
}

std::string Metadatos::getGenero() const{
  return genero;
}
