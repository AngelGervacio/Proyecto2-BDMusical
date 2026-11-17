import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects

Rectangle{
    property var tema
    property var cancion

    color: tema.colorFondo

    Image{
        id: portadaFondo
        cache: false
        source: cancion.imagen.toString() !== "" ? cancion.imagen : "qrc:/imagenes/cover.png"
        anchors.fill: parent
        fillMode: Image.PreserveAspectCrop
        layer.enabled: true
        layer.effect: MultiEffect{
            blurEnabled: true
            blur: 1.0
            blurMax: 64
        }
    }

    RowLayout{
        anchors.fill: parent
        spacing: 40

        Item {
            Layout.fillWidth: true
        }

        Item{
            Layout.preferredHeight: 250
            Layout.preferredWidth: 250
            Layout.alignment: Qt.AlignCenter

            Image{
                id: portada
                cache: false
                source: cancion.imagen.toString() !== "" ? cancion.imagen : "qrc:/imagenes/cover.png"
                fillMode: Image.PreserveAspectFit
                anchors.fill: parent
            }

            Image{
                cache: false
                source: cancion.imagenArtista.toString() !== "" ? cancion.imagenArtista : "qrc:/imagenes/artist.png"
                fillMode: Image.PreserveAspectFit
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                anchors.leftMargin: -50
                anchors.bottomMargin: -50
                height: 100
                width: 100
            }
        }

        Rectangle{
            radius: 12
            implicitHeight: 250
            implicitWidth: 300
            color: tema.colorFondo

            ColumnLayout{
                anchors.fill: parent
                anchors.margins: 15
                Layout.alignment: Qt.AlignCenter

                Label{
                    text: cancion.titulo
                    color: tema.colorTexto
                    font.pixelSize: 18
                    Layout.preferredWidth: 270
                    elide: Text.ElideRight
                    font.bold: true
                }

                Label{
                    text: cancion.artista
                    color: tema.colorTexto
                    font.pixelSize: 16
                    Layout.preferredWidth: 270
                    elide: Text.ElideRight
                    font.bold: true
                }

                Label{
                    text: cancion.album
                    color: tema.colorTexto
                    font.pixelSize: 14
                    Layout.preferredWidth: 270
                    elide: Text.ElideRight
                }

                Label{
                    text: cancion.pista
                    color: tema.colorTexto
                    font.pixelSize: 14
                    Layout.preferredWidth: 270
                    elide: Text.ElideRight
                }

                Label{
                    text: cancion.anio
                    color: tema.colorTexto
                    font.pixelSize: 14
                    Layout.preferredWidth: 270
                    elide: Text.ElideRight
                }

                Label{
                    text: cancion.genero
                    color: tema.colorTexto
                    font.pixelSize: 14
                    Layout.preferredWidth: 270
                    elide: Text.ElideRight
                }
            }
        }

        Item {
            Layout.fillWidth: true
        }
    }
}
