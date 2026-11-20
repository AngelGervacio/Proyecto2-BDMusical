/**
 * @file ValidadorSemantico.cpp
 * @brief Implementación de la clase ValidadorSemantico.
 */

#include "proyecto2/compilador/ValidadorSemantico.hpp"

void ValidadorSemantico::validar(const Vertice& raiz){
  validarVertice(raiz);
}

void ValidadorSemantico::validarVertice(const Vertice& vertice){
  Token token = vertice.getToken();
  if(token.getTipo() == TipoToken::AND || token.getTipo() == TipoToken::OR){
    if(!vertice.hayIzquierdo() || !vertice.hayDerecho())
      throw std::invalid_argument("La busqueda realizada no es valida.");
    
    validarVertice(vertice.getIzquierdo());
    validarVertice(vertice.getDerecho());
    return;
  }
  validarCondicion(vertice);
}

void ValidadorSemantico::validarCondicion(const Vertice& vertice){
  if(!vertice.hayIzquierdo() || !vertice.hayDerecho())
    throw std::invalid_argument("La busqueda realizada no es valida.");

  const Token& campo = vertice.getIzquierdo().getToken();
  const Token& operador = vertice.getToken();
  const Token& valor = vertice.getDerecho().getToken();

  if(!esCampoTexto(campo) && !esCampoNumerico(campo))
    throw std::invalid_argument("La busqueda realizada no es valida: Campo desconocido");
  
  if(!operadorValido(campo, operador))
    throw std::invalid_argument("La busqueda realizada no es valida: Operador no compatible.");
  
  if(esCampoTexto(campo) && valor.getTipo() != TipoToken::CADENA)
    throw std::invalid_argument("La busqueda realizada no es valida: Valor no valido.");

  if(esCampoNumerico(campo) && valor.getTipo() != TipoToken::NUMERO)
    throw std::invalid_argument("La busqueda realizada no es valida: Valor no valido.");
}

bool ValidadorSemantico::esCampoTexto(const Token& campo) const{
  TipoToken tipo = campo.getTipo();

  return tipo == TipoToken::ARTISTA || tipo == TipoToken::ALBUM || tipo == TipoToken::RUTA || tipo == TipoToken::TITULO || tipo == TipoToken::GENERO;
}

bool ValidadorSemantico::esCampoNumerico(const Token& campo) const{
  TipoToken tipo = campo.getTipo();
    
  return tipo == TipoToken::PISTA || tipo == TipoToken::ANIO;
}

bool ValidadorSemantico::operadorValido(const Token& campo, const Token& operador) const{
  TipoToken tipo = operador.getTipo();
  
  if(esCampoTexto(campo))
    return tipo == TipoToken::IGUAL || tipo == TipoToken::CONTIENE || tipo == TipoToken::DIFERENTE;

  if(esCampoNumerico(campo))
    return tipo == TipoToken::IGUAL || tipo == TipoToken::DIFERENTE || tipo == TipoToken::MENOR || tipo == TipoToken::MAYOR || tipo == TipoToken::MENORIGUAL || tipo == TipoToken::MAYORIGUAL;

  throw std::invalid_argument("La busqueda realizada no es valida");
}
