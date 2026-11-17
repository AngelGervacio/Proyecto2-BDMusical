/**
 * @file CancionesModelo.hpp
 * @brief Header de la clase CancionesModelo
 */

#ifndef CANCIONESMODELO_HPP
#define CANCIONESMODELO_HPP

#include <QAbstractTableModel>
#include <QVector>
#include <QString>
#include <string>
#include <QHash>
#include <QByteArray>
#include <QVariantMap>
#include <filesystem>
#include <cstdlib>
#include <vector>
#include <QUrl>
#include "proyecto2/bd/Metadatos.hpp"

/**
 * @brief CancionesModelo
 * @param Modelo de la tabla de la interfaz.
 */
class CancionesModelo : public QAbstractTableModel{
  Q_OBJECT

private:
  QVector<Metadatos> canciones; ///< Vector con la información de las canciones.

  /**
   * @brief Regresa la ruta de la imagen.
   * @param id El id de la canción, artista, album o persona.
   * @param sufijo El sufijo de la imagen (Cancion, Artista, Album, Persona).
   * @return La ruta de la imagen.
   */
  QString obtenerRutaImagen(int id, const std::string& sufijo) const;

public:
  /**
   * @brief Enumeración con roles.
   */
  enum Roles{
    IdRole = Qt::UserRole + 1,
    TituloRole,
    ArtistaRole,
    AlbumRole,
    PistaRole,
    AnioRole,
    GeneroRole,
    PortadaRole,
    ArtistaPortadaRole
  };

  /**
   * @brief Constructor de la clase.
   * @param parent Objeto padre.
   */
  CancionesModelo(QObject *parent = nullptr);

  /**
   * @brief Regresa la cantidad de filas en la tabla.
   * @param parent El indice de un elemento padre.
   * @return La cantidad de filas en la tabla.
   */
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  
  /**
   * @brief Regresa la cantidad de columnas en la tabla.
   * @param parent El indice de un elemento padre.
   * @return La cantidad de columnas en la tabla.
   */
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;

  /**
   * @brief Regresa el dato de un indice en la tabla.
   * @param index El indice.
   * @param role El dato buscado.
   * @return El dato de un indice en la tabla.
   */
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

  /**
   * @brief Regresa el dato de un encabezado de la tabla.
   * @param section Sección del encabezado.
   * @param orientation Orientación del encabezado.
   * @param role El dato buscado.
   * @return El dato de un encabezado de la tabla
   */
  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

  /**
   * @brief Regresa el nombre asociado a cada rol.
   * @return El nombre asociado a cada rol.
   */
  QHash<int, QByteArray> roleNames() const override;

  /**
   * @brief Carga una lista de canciones en la tabla.
   * @param canciones La lista de canciones.
   */
  void cargarCanciones(const QVector<Metadatos>& canciones);

  /**
   * @brief Regresa el id de una canción mediante su fila en la tabla
   * @param fila La fila de la canción.
   * @return El id de la canción.
   */
  Q_INVOKABLE int obtenerId(int fila) const;
};

#endif
