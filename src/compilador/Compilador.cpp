/**
 * @file Compilador.cpp
 * @brief Implementación de la clase Compilador.
 */

#include "proyecto2/compilador/Compilador.hpp"

Resultado Compilador::compilar(const std::string& busqueda){
  Lexer lexer(busqueda);
  lexer.analizarBusqueda();
  std::list<Token> tokens = lexer.getTokens();
  
  Parser parser(tokens);
  std::unique_ptr<Vertice> raiz = parser.parsear();
  
  ValidadorSemantico::validar(*raiz);
    
  Generador generador;
  return generador.generar(*raiz);
}
