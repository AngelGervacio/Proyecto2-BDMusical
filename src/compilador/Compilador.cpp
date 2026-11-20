/**
 * @file Compilador.cpp
 * @brief Implementación de la clase Compilador.
 */

#include "proyecto2/compilador/Compilador.hpp"

Resultado Compilador::compilar(const std::string& busqueda){
  Generador generador;
  if(!busqueda.starts_with("@@"))
    return generador.generarBusqueda(busqueda);

  std::string expresion = busqueda.substr(2);
  
  Lexer lexer(expresion);
  lexer.analizarBusqueda();
  std::list<Token> tokens = lexer.getTokens();
  
  Parser parser(tokens);
  std::unique_ptr<Vertice> raiz = parser.parsear();
  
  ValidadorSemantico::validar(*raiz);
  
  return generador.generar(*raiz);
}
