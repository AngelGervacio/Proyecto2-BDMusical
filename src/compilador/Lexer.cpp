/**
 * @file Lexer.cpp
 * @brief Implementación de la clase Lexer.
 */

#include "proyecto2/compilador/Lexer.hpp"

Lexer::Lexer(std::string busqueda){
  this->busqueda = QString::fromUtf8(busqueda.data(), busqueda.size());
  posicion = 0;
}

std::list<Token> Lexer::getTokens() const{
  return tokens;
}

void Lexer::analizarBusqueda(){
  while(posicion < busqueda.length()){
    QChar caracter = busqueda[posicion];

    if(caracter.isSpace()){
      posicion++;
      continue;
    }

    if(esOperadorDoble())
      continue;

    if(esOperadorSimple())
      continue;

    if(caracter == QChar('"')){
      Token cadena = leerCadena();
      tokens.push_back(cadena);
      continue;
    }

    if(caracter.isDigit()){
      Token numero = leerNumero();
      tokens.push_back(numero);
      continue;
    }

    if(caracter.isLetter()){
      Token campo = leerCampo();
      tokens.push_back(campo);
      continue;
    }

    throw std::invalid_argument("La busqueda ingresada no es valida.");
  }
}

bool Lexer::esOperadorDoble(){
  if(posicion + 1 < busqueda.length()){
    std::string operador = busqueda.toStdString().substr(posicion, 2);
    TipoToken tipo;
    switch(operador){
    case "!=":
      tipo = TipoToken::DIFERENTE;
      break;
    case "~=":
      tipo = TipoToken::CONTIENE;
      break;
    case ">=":
      tipo = TipoToken::MAYORIGUAL;
      break;
    case "<=":
      tipo = TipoToken::MENORIGUAL;
      break;
    case "&&":
      tipo = TipoToken::AND;
      break;
    case "||":
      tipo = TipoToken::OR;
      break;
    default:
      tipo = TipoToken::NOVALIDO;
      break;
    }

    if(tipo != TipoToken::NOVALIDO){
      tokens.push_back(Token(tipo, operador));
      posicion += 2;
      return true;
    }
  }

  return false;
}

bool Lexer::esOperadorSimple(){
  if(posicion < busqueda.length()){
    char operador = busqueda.toStdString()[posicion];
    TipoToken tipo;
    switch(operador){
    case '=':
      tipo = TipoToken::IGUAL;
      break;
    case '>':
      tipo = TipoToken::MAYOR;
      break;
    case '<':
      tipo = TipoToken::MENOR;
      break;
    default:
      tipo = TipoToken::NOVALIDO;
      break;
    }

    if(tipo != TipoToken::NOVALIDO){
      tokens.push_back(Token(tipo, std::string(1, operador)));
      posicion++;
      return true;
    }
  }

  return false;
}

Token Lexer::leerCadena(){
  posicion++;
  int inicio = posicion;
  std::string texto = busqueda.toStdString();
    
  while(posicion < texto.length() && texto[posicion] != '"')
    posicion++;

  std::string valor;
  if(posicion >= texto.length())
    throw std::invalid_argument("La busqueda ingresada no es valida, falta una comilla de cierre.");

  valor = texto.substr(inicio, posicion - inicio);
  posicion++;

  return Token(TipoToken::CADENA, valor);
}

Token leerNumero(){
  int inicio = posicion;
  std::string texto = busqueda.toStdString();

  while(posicion < texto.length() && busqueda[posicion].isDigit())
    posicion++;

  std::string valor = texto.substr(inicio, posicion - inicio);

  return Token(TipoToken::NUMERO, valor);
}

Token Lexer::leerCampo(){
  int inicio = posicion;

  while(posicion < busqueda.length() && busqueda[posicion].isLetter())
    posicion++;

  std::string valor = busqueda.toStdString().substr(inicio, posicion - inicio);

  TipoToken tipo;

  switch(valor){
  case "ARTISTA":
    tipo = TipoToken::ARTISTA;
    break;
  case "ALBUM":
    tipo = TipoToken::ALBUM;
    break;
  case "RUTA":
    tipo = TipoToken::RUTA;
    break;
  case "TITULO":
    tipo = TipoToken::TITULO;
    break;
  case "PISTA":
    tipo = TipoToken::PISTA;
    break;
  case "AÑO":
    tipo = TipoToken::ANIO;
    break;
  case "GENERO":
    tipo = TipoToken::GENERO;
    break;
  default:
    tipo = TipoToken::NOVALIDO
  }

  if(tipo == TipoToken::NOVALIDO)
    throw std::invalid_argument("La busqueda ingresada no es valida, hay un campo no valido.");

  return Token(tipo, valor);
}
