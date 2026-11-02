#include "proyecto2/minero/Minero.hpp"
#include <filesystem>
#include <memory>
#include <iostream>

std::string rutaDirectorio = "";
std::string rutaBD = "";

void ayuda(){
  std::string ayuda = "Modo de empleo: ./build/src/Minero [OPCIONES...]\n\n"
    "Minero de MP3.\n\n"
    "\u001B[1mOPCIONES:\u001B[0m\n\n" 
    "  -d \u001B[4mdirectorio\u001B[0m\n"
    "         Indica el directorio a minar recursivamente.\n\n"
    "  -b \u001B[4mruta\u001B[0m\n"
    "         Indica la ruta del archivo de la base de datos. (si no se especifica la ruta, se guarda la base\n"
    "         de datos dentro de la carpeta predeterminada ~/.local/share/proyecto2/ o en la configurada por el\n"
    "         usuario)\n\n"
    "  -h \n"
    "         Muestra este mensaje de ayuda.\n\n"
    "\u001B[1mEJEMPLOS:\u001B[0m\n\n"
    "       Las opciones se pueden ingresar en cualquier orden, siempre y cuando se ingresen de forma correcta.\n\n"
    "       ./build/src/Minero -d \u001B[4m~/Música\u001B[0m\n"
    "              Se minara el directorio Música.\n\n"
    "       ./build/src/Minero -d \u001B[4m~/Música\u001B[0m -b \u001B[4m~/Documentos/datos.db\u001B[0m\n"
    "              Se minara el directorio Música y se llenara la base de datos llamada datos.db que se encuentra\n"
    "              en el directorio documentos.\n\n"
    "       ./build/src/Minero -h\n"
    "              Muestra este mensaje de ayuda.\n\n";

  std::cout << ayuda;
}

int configurar(int argc, char* argv[]){
  if(argc < 2){
    std::cout << "Minero: Faltan argumentos.\n";
    return 1;
  }

  for(int i = 1; i < argc; i++){
    std::string argumento = argv[i];

    if(argumento == "-d"){
      if(!rutaDirectorio.empty()){
	std::cout << "Minero: La ruta del directorio a minar ya fue ingresado.\n";
	return 1;
      }

      if(i+1 >= argc){
	std::cout << "Minero: Falta ingresar el directorio a minar.\n";
	return 1;
      }

      rutaDirectorio = argv[++i];
      
    }else if(argumento == "-b"){
      if(!rutaBD.empty()){
	std::cout << "Minero: La ruta de la base de datos ya fue ingresada.\n";
	return 1;
      }

      if(i+1 >= argc){
	std::cout << "Minero: Falta ingresar la ruta de la base de datos.\n";
	return 1;
      }

      rutaBD = argv[++i];
      
    }else if(argumento == "-h"){
      ayuda();
      return 2;
      
    }else{
      std::cout << "Minero: La bandera " << argumento << " no es válida.\n";
      return 1;
    }
  }

  if(rutaDirectorio.empty()){
    std::cout << "Minero: Falta ingresar el directorio a minar.\n";
    return 1;
  }

  return 0;
}

int main(int argc, char* argv[]){
  int resultado = configurar(argc, argv);

  if(resultado == 1){
    std::cout << "Utiliza la bandera -h para ver un mensaje de ayuda.\n";
    return 1;
  }else if(resultado == 2)
    return 0;

  try{
    std::unique_ptr<AdministraBD> admin;

    if(!rutaBD.empty())
      admin = std::make_unique<AdministraBD>(rutaBD);
    else
      admin = std::make_unique<AdministraBD>();

    admin->abrir();

    BaseDatosDAO dao(*admin);
    Minero minero(dao);

    minero.minar(std::filesystem::path(rutaDirectorio));
    
  }catch(const std::exception& e){
    std::cerr << "Minero: " << e.what() << "\n";
    return 1;
  }

  return 0;
}
