/**
 * @file AdministraBD.cpp
 * @brief Implementación de la clase AdministraBD.
 */

#include "proyecto2/bd/AdministraBD.hpp"

AdministraBD::AdministraBD(){
  bd = nullptr;
}

AdministraBD::~AdministraBD(){
  if(estaAbierta())
    cerrar();
}

void AdministraBD::crear(){
  std::ifstream esquema(RUTA_ESQUEMA);

  if(!esquema.is_open()){
    throw std::runtime_error("No se pudo abrir el archivo del esquema SQL.");
  }

  std::string texto;
  std::string linea;
  while(std::getline(esquema, linea)){
    texto += linea + "\n";
  }

  if(esquema.bad())
    throw std::runtime_error("No se pudo leer el archivo del esquema SQL.");

  const char* sql = texto.c_str();

  char* mensajeError = nullptr;

  int r = sqlite3_exec(getBaseDeDatos(), sql, nullptr, nullptr, &mensajeError);

  if(r != SQLITE_OK){
    std::string mensaje = mensajeError ? mensajeError : sqlite3_errmsg(bd);
    sqlite3_free(mensajeError);
    throw std::runtime_error("No se pudo crear el esquema: " + mensaje);
  }
}

std::string AdministraBD::obtenerRutaBD() const{
  const char* xdg = std::getenv("XDG_DATA_HOME");
  std::filesystem::path directorio;

  if(xdg == nullptr || *xdg == '\0' || !std::filesystem::path(xdg).is_absolute()){
    xdg = std::getenv("HOME");
    if(xdg == nullptr || *xdg == '\0'){
      throw std::runtime_error("No se pudo determinar el directorio del usuario.");
    }
    directorio = std::filesystem::path(xdg) / ".local" / "share";
  }else{
    directorio = xdg;
  }

  directorio /= "proyecto2";

  std::filesystem::create_directories(directorio);

  return (directorio / "rolas.db").string();
}

void AdministraBD::abrir(){
  std::string archivo;
  
  try{
    archivo = obtenerRutaBD();
  }catch(std::filesystem::filesystem_error&){
    throw std::runtime_error("No se pudo acceder al archivo.");
  }catch(std::bad_alloc&){
    throw std::runtime_error("Error de asignación de memoria.");
  }

  std::filesystem::path pathArchivo = archivo;

  bool existe = std::filesystem::exists(pathArchivo);

  int r = sqlite3_open(archivo.c_str(), &bd);

  if(r != SQLITE_OK){
    std::string mensaje = bd ? sqlite3_errmsg(bd) : "Error desconocido";
    cerrar();
    throw std::runtime_error("No se pudo abrir la base de datos correctamente: " + mensaje);
  }

  if(!existe){
    try{
      crear();
    }catch(...){
      cerrar();
      std::error_code error;
      std::filesystem::remove(pathArchivo, error);
      throw;
    }
  }
}

bool AdministraBD::estaAbierta() const{
  return bd != nullptr;
}

sqlite3* AdministraBD::getBaseDeDatos() const{
  return bd;
}

void AdministraBD::cerrar(){
  if(!estaAbierta())
    return;

  int r = sqlite3_close(bd);

  if(r != SQLITE_OK){
    std::string mensaje = sqlite3_errmsg(bd);
    throw std::runtime_error("No se pudo cerrar la base de datos correctamente: " + mensaje);
  }

  bd = nullptr;
}
