/**
 * @file TestMinero.cpp
 * @brief Pruebas unitarias para la clase Minero.
 */

#include <gtest/gtest.h>
#include <stdexcept>
#include <filesystem>
#include "proyecto2/minero/Minero.hpp"
#include "proyecto2/bd/AdministraBD.hpp"
#include "proyecto2/bd/BaseDatosDAO.hpp"

/**
 * @brief Prueba el metodo minar() de la clase Minero.
 * Comprueba que se pueda minar correctamente los directorios y
 * llenar la base de datos.
 */
TEST(MineroTest, minar){
  std::filesystem::path testBD = std::filesystem::path(TEST_DIR) / "testMinero.db";
  AdministraBD admin{testBD};
  BaseDatosDAO dao{admin};
  Minero minero{dao};
  
  admin.abrir();
  
  EXPECT_THROW(minero.minar(std::filesystem::path()), std::invalid_argument);

  EXPECT_THROW(minero.minar(std::filesystem::path("NoExiste")), std::invalid_argument);

  EXPECT_THROW(minero.minar(std::filesystem::path(TEST_DIR) / "demo.mp3"), std::invalid_argument);

  EXPECT_FALSE(dao.existeRuta(std::filesystem::path(TEST_DIR) / "demo.mp3"));
  
  EXPECT_NO_THROW(minero.minar(std::filesystem::path(TEST_DIR)));

  EXPECT_TRUE(dao.existeRuta(std::filesystem::path(TEST_DIR) / "demo.mp3"));

  if(admin.estaAbierta())
    admin.cerrar();
  
  if(std::filesystem::exists(testBD))
    std::filesystem::remove(testBD);
}
