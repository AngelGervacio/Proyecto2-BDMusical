/**
 * @file TestAdministraBD.cpp
 * @brief Pruebas unitarias para la clase AdministraBD.
 */

#include <gtest/gtest.h>
#include <sqlite3.h>
#include "proyecto2/bd/AdministraBD.hpp"

/**
 * @brief Entorno de pruebas para TestAdministraBD.
 * Contiene una instancia de AdministraBD con una base de datos
 * para pruebas, al terminar cada prueba cierra y elimina la
 * la base de datos creada durante las pruebas.
 */
class AdministraBDTestFixture : public ::testing::Test{
protected:
  std::filesystem::path testBD = std::filesystem::path(TEST_DIR) / "testBD.db"; ///< Ruta de la base de datos.
  AdministraBD admin{testBD}; ///< Administrador de la base de datos.

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
 * @brief Prueba del constructor de la clase AdministraBD.
 */
TEST_F(AdministraBDTestFixture, ConstructorAdministraBD){
  EXPECT_FALSE(admin.estaAbierta());
  EXPECT_EQ(admin.getBaseDeDatos(), nullptr);
}

/**
 * @brief Prueba del metodo abrir() de la clase AdministraBD.
 * Comprueba que se pueda abrir correctamente la base de datos.
 */
TEST_F(AdministraBDTestFixture, AbrirBaseDeDatos){
  EXPECT_FALSE(admin.estaAbierta());

  admin.abrir();

  EXPECT_TRUE(admin.estaAbierta());
  EXPECT_NE(admin.getBaseDeDatos(), nullptr);

  EXPECT_NO_THROW(admin.abrir());
}

/**
 * @brief Prueba del metodo cerrar() de la clase AdministraBD.
 * Comprueba que se pueda cerrar correctamente la base de datos.
 */
TEST_F(AdministraBDTestFixture, CerrarBaseDeDatos){
  EXPECT_FALSE(admin.estaAbierta());
  EXPECT_NO_THROW(admin.cerrar());

  admin.abrir();

  EXPECT_TRUE(admin.estaAbierta());
  EXPECT_NO_THROW(admin.cerrar());
  EXPECT_FALSE(admin.estaAbierta());
}

