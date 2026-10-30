/**
 * @file Metadatos.cpp
 * @brief Implementación de la clase Metadatos.
 */

#include "proyecto2/bd/Metadatos.hpp"

Metadatos::Builder& Metadatos::Builder::setRuta(const std::filesystem::path& ruta){
  this->ruta = ruta;
  return *this;
}

Metadatos::Builder& Metadatos::Builder::setTitulo(std::string titulo){
  this->titulo = titulo;
  return *this;
}

Metadatos::Builder& Metadatos::Builder::setArtista(std::string artista){
  this->artista = artista;
  return *this;
}

Metadatos::Builder& Metadatos::Builder::setAlbum(std::string album){
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

Metadatos::Builder& Metadatos::Builder::setGenero(std::string genero){
  this->genero = genero;
  return *this;
}

Metadatos Metadatos::Builder::build(){
  return Metadatos(ruta, titulo, artista, album, pista, año, genero);
}

Metadatos::Metadatos(std::filesystem::path& ruta,
		     std::string titulo,
		     std::string artista,
		     std::string album,
		     int pista,
		     int año,
		     std::string genero){
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
