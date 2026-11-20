/**
 * @file Generador.cpp
 * @brief Implementación de la clase Generador.
 */

#include "proyecto2/compilador/Generador.hpp"

Resultado Generador::generar(const Vertice& raiz){
  Resultado resultado;
  resultado.consulta = "SELECT rolas.id_rola, rolas.title, performers.name, albums.name, rolas.track, rolas.year, rolas.genre "
    "FROM rolas LEFT JOIN performers ON rolas.id_performer = performers.id_performer "
    "LEFT JOIN albums ON rolas.id_album = albums.id_album WHERE ";
  
  generarVertice(raiz, resultado);
  
  resultado.consulta += " ORDER BY rolas.title ASC";
  
  return resultado;
}

Resultado Generador::generarBusqueda(const std::string& busqueda){
  Resultado resultado;
  resultado.consulta = "SELECT rolas.id_rola, rolas.title, performers.name, albums.name, rolas.track, rolas.year, rolas.genre "
    "FROM rolas LEFT JOIN performers ON rolas.id_performer = performers.id_performer "
    "LEFT JOIN albums ON rolas.id_album = albums.id_album WHERE rolas.title LIKE ? OR performers.name LIKE ? OR "
    "albums.name LIKE ? OR rolas.path LIKE ? OR rolas.genre LIKE ? ORDER BY rolas.title ASC";
  
  for(int i = 0; i < 5; i++){
    resultado.parametros.push_back("%" + busqueda + "%");
  }
  
  return resultado;
}

void Generador::generarVertice(const Vertice& vertice, Resultado& resultado){
  Token token = vertice.getToken();
  if(token.getTipo() == TipoToken::AND || token.getTipo() == TipoToken::OR){
    if(!vertice.hayIzquierdo() || !vertice.hayDerecho())
      throw std::invalid_argument("La busqueda realizada no es valida.");
    
    generarVertice(vertice.getIzquierdo(), resultado);

    if(token.getTipo() == TipoToken::AND)
      resultado.consulta += " AND ";
    else
      resultado.consulta += " OR ";
    
    generarVertice(vertice.getDerecho(), resultado);
    return;
  }
  generarCondicion(vertice, resultado);
}

void Generador::generarCondicion(const Vertice& vertice, Resultado& resultado){
  if(!vertice.hayIzquierdo() || !vertice.hayDerecho())
    throw std::invalid_argument("La busqueda realizada no es valida.");
  
  const Token& campo = vertice.getIzquierdo().getToken();
  const Token& operador = vertice.getToken();
  const Token& valor = vertice.getDerecho().getToken();

  std::string campoConsulta = obtenerCampo(campo);
  std::string operadorConsulta = obtenerOperador(operador);

  resultado.consulta += campoConsulta + " " + operadorConsulta + " ?";

  if(operadorConsulta == "LIKE")
    resultado.parametros.push_back("%" + valor.getValor() + "%");
  else
    resultado.parametros.push_back(valor.getValor());
}

std::string Generador::obtenerCampo(const Token& campo) const{
  TipoToken tipo = campo.getTipo();

  switch(tipo){
  case TipoToken::ARTISTA: return "performers.name";
  case TipoToken::ALBUM: return "albums.name";
  case TipoToken::RUTA: return "rolas.path";
  case TipoToken::TITULO: return "rolas.title";
  case TipoToken::PISTA: return "rolas.track";
  case TipoToken::ANIO: return "rolas.year";
  case TipoToken::GENERO: return "rolas.genre";
  default: throw std::invalid_argument("La busqueda realizada no es valida: Campo no valido.");
  }
}

std::string Generador::obtenerOperador(const Token& operador) const{
  TipoToken tipo = operador.getTipo();
  
  switch(tipo){
  case TipoToken::IGUAL: return "=";
  case TipoToken::CONTIENE: return "LIKE";
  case TipoToken::DIFERENTE: return "!=";
  case TipoToken::MENOR: return "<";
  case TipoToken::MAYOR: return ">";
  case TipoToken::MENORIGUAL: return "<=";
  case TipoToken::MAYORIGUAL: return ">=";
  default: throw std::invalid_argument("La busqueda realizada no es valida: Operador no compatible.");
  }
}
