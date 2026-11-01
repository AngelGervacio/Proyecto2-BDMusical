/**
 * @file TestAdministraBD.cpp
 * @brief Pruebas unitarias para la clase AdministraBD.
 */

#include <gtest/gtest.h>
#include <sqlite3.h>
#include "proyecto2/bd/AdministraBD.hpp"

class AdministraBDTestFixture : public ::testing::Test{
public:
  AdministraBD admin{std::string(TEST_DIR) + "/testBD.db"};
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

TEST_F(AdministraBDTestFixture, CerrarBaseDeDatos){
  EXPECT_FALSE(admin.estaAbierta());
  EXPECT_NO_THROW(admin.cerrar());

  admin.abrir();

  EXPECT_TRUE(admin.estaAbierta());
  EXPECT_NO_THROW(admin.cerrar());
  EXPECT_FALSE(admin.estaAbierta());
}

