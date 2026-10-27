/**
 * @file TestAdministraBD.cpp
 * @brief Pruebas unitarias para la clase AdministraBD.
 */

#include <gtest/gtest.h>
#include <sqlite3.h>
#include "proyecto2/bd/AdministraBD.hpp"

TEST(AdministraBDTest, ConstructorAdministraBD){
  AdministraBD admin;

  EXPECT_FALSE(admin.estaAbierta());
  EXPECT_EQ(admin.getBaseDeDatos(), nullptr);
}

TEST(AdministraBDTest, AbrirBaseDeDatos){
  AdministraBD admin;
  
  EXPECT_FALSE(admin.estaAbierta());

  admin.abrir();

  EXPECT_TRUE(admin.estaAbierta());
  EXPECT_NE(admin.getBaseDeDatos(), nullptr);

  EXPECT_NO_THROW(admin.abrir());
}

TEST(AdministraBDTest, CerrarBaseDeDatos){
  AdministraBD admin;

  EXPECT_FALSE(admin.estaAbierta());
  EXPECT_NO_THROW(admin.cerrar());

  admin.abrir();

  EXPECT_TRUE(admin.estaAbierta());
  EXPECT_NO_THROW(admin.cerrar());
  EXPECT_FALSE(admin.estaAbierta());
}

