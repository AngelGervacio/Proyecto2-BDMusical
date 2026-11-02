/**
 * @file TestBaseDatosDAO.cpp
 * @brief Pruebas unitarias para la clase BaseDatosDAO.
 */

#include <gtest/gtest.h>
#include <string>
#include <stdexcept>
#include "proyecto2/bd/BaseDatosDAO.hpp"
#include "proyecto2/bd/Metadatos.hpp"
#include "../util/Util.hpp"

/**
 * @brief Entorno de pruebas para TestBaseDatosDAO.
 * Contiene una instancia de AdministraBD con una base de datos
 * para pruebas y una instancia de BaseDatosDAO para acceder a la
 * base de datos, al iniciar cada prueba abre la base de datos y
 * al terminar la cierra y elimina.
 */
class BaseDatosDAOTestFixture : public ::testing::Test{
protected:
  std::filesystem::path testBD = std::filesystem::path(TEST_DIR) / "testDAO.db"; ///< Ruta de la base de datos.
  AdministraBD admin{testBD}; ///< Administrador de la base de datos.
  BaseDatosDAO dao{admin}; ///< DAO de la base de datos.

  /**
   * @brief Inicializa el entorno de pruebas para cada prueba.
   * Abre la base de datos.
   */
  void SetUp() override{
    admin.abrir();
  }

  /**
   * @brief Destructor del entorno de pruebas.
   * Cierra y elimina la base de datos creada durante las pruebas.
   */
  void TearDown() override{
    if(admin.estaAbierta())
      admin.cerrar();
    
    if(std::filesystem::exists(testBD))
      std::filesystem::remove(testBD);
  }
};

/**
 * @brief Prueba del metodo insertarArtista() de la clase BaseDatosDAO.
 * Comprueba que se pueda insertar un artista en la base de datos.
 */
TEST_F(BaseDatosDAOTestFixture, InsertarArtista){
  std::string artista;

  EXPECT_THROW(dao.insertarArtista(artista), std::invalid_argument);

  for(int i = 0; i < 20; i++){
    int id = -1;
    artista = nombreAleatorio() + std::to_string(i);
    EXPECT_NO_THROW(id = dao.insertarArtista(artista));
    EXPECT_EQ(id, i+1);
  }

  artista = "Duplicado";

  int idDuplicado = dao.insertarArtista(artista);
  EXPECT_EQ(dao.insertarArtista(artista), idDuplicado);

  admin.cerrar();
  EXPECT_THROW(dao.insertarArtista("artista"), std::runtime_error);
}

/**
 * @brief Prueba del metodo obtenerArtista() de la clase BaseDatosDAO.
 * Comprueba que se pueda obtener la id de un artista en la base de datos.
 */
TEST_F(BaseDatosDAOTestFixture, ObtenerArtista){
  std::string artista;
  
  EXPECT_THROW(dao.obtenerArtista(artista), std::invalid_argument);

  for(int i = 0; i < 20; i++){
    artista = nombreAleatorio() + std::to_string(i);
    int id = dao.insertarArtista(artista);
    EXPECT_EQ(id, dao.obtenerArtista(artista));
  }
  
  artista = nombreAleatorio();
  EXPECT_EQ(dao.obtenerArtista(artista), -1);

  admin.cerrar();

  EXPECT_THROW(dao.obtenerArtista("artista"), std::runtime_error);
}

/**
 * @brief Prueba del metodo insertarAlbum() de la clase BaseDatosDAO.
 * Comprueba que se pueda insertar un album en la base de datos.
 */
TEST_F(BaseDatosDAOTestFixture, InsertarAlbum){
  std::string album;
  std::string ruta;
  int anio;
    
  EXPECT_THROW(dao.insertarAlbum(ruta, "album", 0), std::invalid_argument);
  EXPECT_THROW(dao.insertarAlbum("ruta", album, 0), std::invalid_argument);

  for(int i = 0; i < 20; i++){
    int id = -1;
    album = nombreAleatorio() + std::to_string(i);
    ruta = "test/" + album;
    anio = numeroAleatorio() * 40;
    EXPECT_NO_THROW(id = dao.insertarAlbum(ruta, album, anio));
    EXPECT_EQ(id, i+1);
  }
  
  album = "Duplicado";  
  ruta = "test/" + album;
  
  int idDuplicado = dao.insertarAlbum(ruta, album, 0);
  album = "Duplicado2";
  EXPECT_EQ(dao.insertarAlbum(ruta, album, 0), idDuplicado);
  
  admin.cerrar();
  EXPECT_THROW(dao.insertarAlbum("ruta", "album", 0), std::runtime_error);

}

/**
 * @brief Prueba del metodo obtenerAlbum() de la clase BaseDatosDAO.
 * Comprueba que se pueda obtener la id de un album en la base de datos.
 */
TEST_F(BaseDatosDAOTestFixture, ObtenerAlbum){
  std::string ruta;
    
  EXPECT_THROW(dao.obtenerAlbum(ruta), std::invalid_argument);

  for(int i = 0; i < 20; i++){
    std::string album = nombreAleatorio() + std::to_string(i);
    ruta = "test/" + album;
    int id = dao.insertarAlbum(ruta, album, 0);
    EXPECT_EQ(dao.obtenerAlbum(ruta), id);
  }
    
  ruta = "test/" + nombreAleatorio();
  EXPECT_EQ(dao.obtenerAlbum(ruta), -1);

  admin.cerrar();
  EXPECT_THROW(dao.obtenerAlbum("ruta"), std::runtime_error);
}

/**
 * @brief Prueba del metodo insertarCancion() de la clase BaseDatosDAO.
 * Comprueba que se pueda insertar una canción en la base de datos.
 */
TEST_F(BaseDatosDAOTestFixture, InsertarCancion){
  Metadatos metadatos = Metadatos::Builder().build();

  EXPECT_THROW(dao.insertarCancion(metadatos), std::invalid_argument);

  std::filesystem::path ruta = std::filesystem::path(TEST_DIR) / "demo.mp3";
  metadatos = Metadatos::Builder()
    .setRuta(ruta)
    .setTitulo("T")
    .setArtista("A")
    .setAlbum("L")
    .setPista(3)
    .setAnio(1950)
    .setGenero("Jazz")
    .build();
  
  EXPECT_NO_THROW(dao.insertarCancion(metadatos));
  
  admin.cerrar();
  EXPECT_THROW(dao.insertarCancion(Metadatos::Builder().build()), std::runtime_error);
}

/**
 * @brief Prueba del metodo existeRuta() de la clase BaseDatosDAO.
 * Comprueba que se pueda saber si existe una canción con una ruta
 * especifica en la base de datos.
 */
TEST_F(BaseDatosDAOTestFixture, ExisteRuta){
  std::filesystem::path ruta = std::filesystem::path(TEST_DIR) / "demo.mp3";

  EXPECT_FALSE(dao.existeRuta(std::filesystem::path()));
  EXPECT_FALSE(dao.existeRuta(std::filesystem::path(TEST_DIR) / "noExiste.mp3"));
  EXPECT_FALSE(dao.existeRuta(ruta));

  Metadatos metadatos = Metadatos::Builder()
    .setRuta(ruta)
    .setTitulo("T")
    .setArtista("A")
    .setAlbum("L")
    .setPista(3)
    .setAnio(1950)
    .setGenero("Jazz")
    .build();

  dao.insertarCancion(metadatos);

  EXPECT_TRUE(dao.existeRuta(ruta));
}
