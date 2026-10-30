#include "proyecto2/minero/Minero.hpp"
#include <filesystem>

int main(int argc, char* argv[]){
  AdministraBD admin;
  try{
    admin.abrir();
  }catch(...){
  }

  BaseDatosDAO dao(admin);
  Minero minero(dao);

  if(argc != 2)
    return 0;

  
  minero.minar(std::filesystem::path(argv[1]));
}
