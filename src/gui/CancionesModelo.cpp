/**
 * @file CancionesModelo.cpp
 * @brief Implementanción de la clase CancionesModelo.
 */

#include "proyecto2/gui/CancionesModelo.hpp"

CancionesModelo::CancionesModelo(QObject *parent) : QAbstractTableModel(parent){}

int CancionesModelo::rowCount(const QModelIndex &parent) const{
  return (parent.isValid()) ? 0 : canciones.size();
}

int CancionesModelo::columnCount(const QModelIndex &parent) const{
  return (parent.isValid()) ? 0 : 4;
}

QVariant CancionesModelo::data(const QModelIndex &index, int role) const{
  if(!index.isValid() || index.row() >= canciones.size() || index.column() >= 4)
    return {};

  const Metadatos& metadatos = canciones[index.row()];

  switch(role){
  case IdRole: return metadatos.getId();
  case TituloRole: return QString::fromUtf8(metadatos.getTitulo());
  case ArtistaRole: return QString::fromUtf8(metadatos.getArtista());
  case AlbumRole: return QString::fromUtf8(metadatos.getAlbum());
  case PistaRole: return metadatos.getPista();
  case AnioRole: return metadatos.getAnio();
  case GeneroRole: return QString::fromUtf8(metadatos.getGenero());
  case PortadaRole: return obtenerRutaImagen(metadatos.getId(), "Portada");
  case ArtistaPortadaRole: return obtenerRutaImagen(metadatos.getId(), "Artista");
  }

  if(role == Qt::DisplayRole){
    switch(index.column()){
    case 0: return QString::fromUtf8(metadatos.getTitulo());
    case 1: return QString::fromUtf8(metadatos.getArtista());
    case 2: return QString::fromUtf8(metadatos.getAlbum());
    case 3: return QString::fromUtf8(metadatos.getGenero());
    }
  }

  return {};
}

QVariant CancionesModelo::headerData(int section, Qt::Orientation orientation, int role) const{
  if(role != Qt::DisplayRole)
    return {};

  if(orientation == Qt::Horizontal){
    switch(section){
    case 0: return "Titulo";
    case 1: return "Artista";
    case 2: return "Álbum";
    case 3: return "Genero";
    }
  }

  return {};
}

QHash<int, QByteArray> CancionesModelo::roleNames() const{
  return {
    {Qt::DisplayRole, "display"},
    {IdRole, "idCancion"},
    {TituloRole, "titulo"},
    {ArtistaRole, "artista"},
    {AlbumRole, "album"},
    {PistaRole, "pista"},
    {AnioRole, "anio"},
    {GeneroRole, "genero"},
    {PortadaRole, "portada"},
    {ArtistaPortadaRole, "artistaPortada"}
  };
}

void CancionesModelo::cargarCanciones(const QVector<Metadatos>& canciones){
  beginResetModel();

  this->canciones = canciones;

  endResetModel();
}

int CancionesModelo::obtenerId(int fila) const{
  if(fila < 0 || fila >= canciones.size())
    return -1;

  return canciones[fila].getId();
}

QString CancionesModelo::obtenerRutaImagen(int id, const std::string& sufijo) const{
  const char* xdg = std::getenv("XDG_CACHE_HOME");
  std::filesystem::path directorio;

  if(xdg == nullptr || *xdg == '\0' || !std::filesystem::path(xdg).is_absolute()){
    xdg = std::getenv("HOME");
    if(xdg == nullptr || *xdg == '\0'){
      return {};
    }
    directorio = std::filesystem::path(xdg) / ".cache";
  }else{
    directorio = xdg;
  }

  directorio /= "proyecto2";

  if(!std::filesystem::exists(directorio))
    return {};

  const std::vector<std::string> extensiones = {".jpg", ".png", ".jpeg", ".webp"};

  for(const std::string& extension : extensiones){
    std::filesystem::path ruta = directorio / (std::to_string(id) + sufijo + extension);

    if(std::filesystem::exists(ruta)){
      return QString::fromStdString(ruta.string());
    }
  }

  return {};
}
