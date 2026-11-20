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
    QString operador = busqueda.mid(posicion, 2);
    TipoToken tipo;
    
    if(operador == "!=")
      tipo = TipoToken::DIFERENTE;
    else if(operador == "~=")
      tipo = TipoToken::CONTIENE;
    else if(operador == ">=")
      tipo = TipoToken::MAYORIGUAL;
    else if(operador == "<=")
      tipo = TipoToken::MENORIGUAL;
    else if(operador == "&&")
      tipo = TipoToken::AND;
    else if(operador == "||")
      tipo = TipoToken::OR;
    else
      tipo = TipoToken::NOVALIDO;

    if(tipo != TipoToken::NOVALIDO){
      tokens.push_back(Token(tipo, operador.toStdString()));
      posicion += 2;
      return true;
    }
  }

  return false;
}

bool Lexer::esOperadorSimple(){
  if(posicion < busqueda.length()){
    QChar operador = busqueda[posicion];
    TipoToken tipo;

    if(operador == '=')
      tipo = TipoToken::IGUAL;
    else if(operador == '>')
      tipo = TipoToken::MAYOR;
    else if(operador == '<')
      tipo = TipoToken::MENOR;
    else
      tipo = TipoToken::NOVALIDO;

    if(tipo != TipoToken::NOVALIDO){
      tokens.push_back(Token(tipo, std::string(1, static_cast<char>(operador.unicode()))));
      posicion++;
      return true;
    }
  }
 
  return false;
}

Token Lexer::leerCadena(){
  posicion++;
  int inicio = posicion;

  while(posicion < busqueda.length() && busqueda[posicion] != QChar('"'))
    posicion++;

  if(posicion >= busqueda.length())
    throw std::invalid_argument("La busqueda ingresada no es valida, falta una comilla de cierre.");

  QString texto = busqueda.mid(inicio, posicion - inicio);
  posicion++;

  return Token(TipoToken::CADENA, texto.toStdString());
}

Token Lexer::leerNumero(){
  int inicio = posicion;

  while(posicion < busqueda.length() && busqueda[posicion].isDigit())
    posicion++;

  QString texto = busqueda.mid(inicio, posicion - inicio);

  return Token(TipoToken::NUMERO, texto.toStdString());
}

Token Lexer::leerCampo(){
  int inicio = posicion;

  while(posicion < busqueda.length() && busqueda[posicion].isLetter())
    posicion++;

  QString texto = busqueda.mid(inicio, posicion - inicio);
  std::string valor = texto.toStdString();

  TipoToken tipo;

  if(valor == "ARTISTA")
    tipo = TipoToken::ARTISTA;
  else if(valor == "ALBUM")
    tipo = TipoToken::ALBUM;
  else if(valor == "RUTA")
    tipo = TipoToken::RUTA;
  else if(valor == "TITULO")
    tipo = TipoToken::TITULO;
  else if(valor == "PISTA")
    tipo = TipoToken::PISTA;
  else if(valor == "AÑO")
    tipo = TipoToken::ANIO;
  else if(valor == "GENERO")
    tipo = TipoToken::GENERO;
  else
    throw std::invalid_argument("La busqueda ingresada no es valida, hay un campo no valido.");

  return Token(tipo, valor);
}
