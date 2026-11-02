/**
 * @file Util.hpp
 * @brief Contiene métodos usados en mas de una prueba unitaria.
 */

#ifndef UTIL_HPP
#define UTIL_HPP

#include <random>
#include <vector>

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

#endif
