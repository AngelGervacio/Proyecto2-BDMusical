/**
 * @file TestMetadatos.cpp
 * @brief Pruebas unitarias para la clase Metadatos.
 */

#include <gtest/gtest.h>
#include <filesystem>
#include <random>
#include <vector>
#include "proyecto2/bd/Metadatos.hpp"

/**
 * @brief Regresa un nombre aleatorio,
 * @return un nombre aleatorio.
 */
std::string nombreAleatorio(){
  const std::vector<std::string> nombres = {
    "Alice", "Bob", "Charlie", "Daniel"
  };
  
  std::random_device random;
  std::mt19937 gen(random());
  std::uniform_int_distribution<> distrib(0, nombres.size() - 1);

  int indice = distrib(gen);

  return nombres[indice];
}

/**
 * @brief Regresa un número aleatorio,
 * @return un número aleatorio.
 */
int numeroAleatorio(){  
  std::random_device random;
  std::mt19937 gen(random());
  std::uniform_int_distribution<> distrib(0, 50);
  return distrib(gen);
}

TEST(MetadatosTest, DatosVacios){
  Metadatos metadatos = Metadatos::Builder().build();

  EXPECT_TRUE(metadatos.getRuta().empty());
  EXPECT_TRUE(metadatos.getTitulo().empty());
  EXPECT_TRUE(metadatos.getArtista().empty());
  EXPECT_TRUE(metadatos.getAlbum().empty());
  EXPECT_EQ(metadatos.getPista(), 0);
  EXPECT_EQ(metadatos.getAnio(), 0);
  EXPECT_TRUE(metadatos.getGenero().empty());
}

TEST(MetadatosTest, AgregaObtieneRuta){
  std::filesystem::path ruta = std::filesystem::path(TEST_DIR) / "demo.mp3";

  Metadatos metadatos = Metadatos::Builder()
    .setRuta(ruta)
    .build();

  EXPECT_EQ(metadatos.getRuta(), ruta);
}

TEST(MetadatosTest, AgregaObtieneTitulo){
  std::string titulo = nombreAleatorio();
  
  Metadatos metadatos = Metadatos::Builder()
    .setTitulo(titulo)
    .build();

  EXPECT_EQ(metadatos.getTitulo(), titulo);
}

TEST(MetadatosTest, AgregaObtieneArtista){
  std::string artista = nombreAleatorio();
  
  Metadatos metadatos = Metadatos::Builder()
    .setArtista(artista)
    .build();

  EXPECT_EQ(metadatos.getArtista(), artista);
}

TEST(MetadatosTest, AgregaObtieneAlbum){
  std::string album = nombreAleatorio();
  
  Metadatos metadatos = Metadatos::Builder()
    .setAlbum(album)
    .build();

  EXPECT_EQ(metadatos.getAlbum(), album);
}

TEST(MetadatosTest, AgregaObtienePista){
  int pista = numeroAleatorio();

  Metadatos metadatos = Metadatos::Builder()
    .setPista(pista)
    .build();

  EXPECT_EQ(metadatos.getPista(), pista);
}

TEST(MetadatosTest, AgregaObtieneAnio){
  int anio = numeroAleatorio() * 40;

  Metadatos metadatos = Metadatos::Builder()
    .setAnio(anio)
    .build();

  EXPECT_EQ(metadatos.getAnio(), anio);
}

TEST(MetadatosTest, AgregaObtieneGenero){
  std::string genero = nombreAleatorio();
  
  Metadatos metadatos = Metadatos::Builder()
    .setGenero(genero)
    .build();

  EXPECT_EQ(metadatos.getGenero(), genero);  
}

TEST(MetadatosTest, DatosParciales){
  std::filesystem::path ruta = std::filesystem::path(TEST_DIR) / "demo.mp3";
  std::string titulo = nombreAleatorio();
  std::string artista = nombreAleatorio();
  int pista = numeroAleatorio();

  Metadatos metadatos = Metadatos::Builder()
    .setRuta(ruta)
    .setTitulo(titulo)
    .setArtista(artista)
    .setPista(pista)
    .build();

  EXPECT_EQ(metadatos.getRuta(), ruta);
  EXPECT_EQ(metadatos.getTitulo(), titulo);
  EXPECT_EQ(metadatos.getArtista(), artista);
  EXPECT_TRUE(metadatos.getAlbum().empty());
  EXPECT_EQ(metadatos.getPista(), pista);
  EXPECT_EQ(metadatos.getAnio(), 0);
  EXPECT_TRUE(metadatos.getGenero().empty());
}

TEST(MetadatosTest, DatosCompletos){
  std::filesystem::path ruta = std::filesystem::path(TEST_DIR) / "demo.mp3";
  std::string titulo = nombreAleatorio();
  std::string artista = nombreAleatorio();
  std::string album = nombreAleatorio();
  int pista = numeroAleatorio();
  int anio = numeroAleatorio() * 40;
  std::string genero = nombreAleatorio();

  Metadatos metadatos = Metadatos::Builder()
    .setRuta(ruta)
    .setTitulo(titulo)
    .setArtista(artista)
    .setAlbum(album)
    .setPista(pista)
    .setAnio(anio)
    .setGenero(genero)
    .build();

  EXPECT_EQ(metadatos.getRuta(), ruta);
  EXPECT_EQ(metadatos.getTitulo(), titulo);
  EXPECT_EQ(metadatos.getArtista(), artista);
  EXPECT_EQ(metadatos.getAlbum(), album);
  EXPECT_EQ(metadatos.getPista(), pista);
  EXPECT_EQ(metadatos.getAnio(), anio);
  EXPECT_EQ(metadatos.getGenero(), genero);
}
