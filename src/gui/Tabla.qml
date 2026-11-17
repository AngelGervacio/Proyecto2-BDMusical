import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.qmlmodels

Rectangle{
    property var tema
    property var repBoton
    property var vistaCuadricula

    property int idSeleccionada: -1

    width: parent.width
    height: parent.height
    color: tema.colorFondo

    signal reproducirCancion(int id, int fila)

    ColumnLayout{
        anchors{
            fill: parent
            leftMargin: 20
            rightMargin: 20
            topMargin: 10
            bottomMargin: 20
        }

        spacing: 10

        TextField{
            id: buscador
            placeholderText: "Buscar..."
            placeholderTextColor: tema.colorTexto
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignTop
            color: tema.colorTexto
            leftPadding: 15
            rightPadding: 15

            background: Rectangle{
                color: tema.colorSobre
                radius: 5
            }
        }

        HorizontalHeaderView{
            id: horizontalHeader
            syncView: tabla
            Layout.fillWidth: true
            Layout.preferredHeight: 35
            clip: true
            visible: !vistaCuadricula

            model: ["Nombre", "Artista", "Álbum", "Genero"]

            delegate: Rectangle{
                implicitHeight: 35;
                implicitWidth: 100;
                color: tema.colorSobre

                Text{
                    text: modelData
                    color: tema.colorTexto
                    anchors.fill: parent
                    anchors.leftMargin: 10
                    anchors.rightMargin: 10
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    elide: Text.ElideRight
                }
            }
        }

        StackLayout{
            Layout.fillHeight: true
            Layout.fillWidth: true

            currentIndex: vistaCuadricula ? 1 : 0

            TableView{
                id: tabla
                Layout.fillHeight: true
                Layout.fillWidth: true
                rowSpacing: 2
                clip: true

                selectionModel: ItemSelectionModel{}
                selectionBehavior: TableView.SelectRows
                selectionMode: TableView.SingleSelection

                columnWidthProvider: function(column){
                    return tabla.width / tabla.columns
                }

                model: cancionesModelo

                delegate: Rectangle{
                    required property int idCancion
                    implicitHeight: 35;
                    required property int row
                    color: tabla.selectionModel.currentIndex.row === row ? tema.colorSobre : tema.colorBoton

                    Text{
                        text: display
                        font.pixelSize: 12
                        color: tema.colorTexto
                        anchors.fill: parent
                        anchors.leftMargin: 10
                        anchors.rightMargin: 10
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignLeft
                        elide: Text.ElideRight
                    }

                    MouseArea{
                        anchors.fill: parent

                        onClicked:{
                            tabla.selectionModel.setCurrentIndex(tabla.model.index(row, 0), ItemSelectionModel.ClearAndSelect | ItemSelectionModel.Rows)

                            idSeleccionada = row
                        }

                        onDoubleClicked: {
                            reproducirCancion(idCancion, row)
                            reproductor.reproducir(controlador.obtenerRuta(idCancion))
                            repBoton.checked = true
                        }
                    }
                }
            }

            GridView{
                id: cuadricula
                model: tabla.model

                cellWidth: 180
                cellHeight: 200
                clip: true

                leftMargin: 40

                delegate: Rectangle{
                    required property int idCancion
                    required property string titulo
                    required property string artista
                    required property int index
                    required property string portada
                    width: 140
                    height: 180
                    color: tabla.selectionModel.currentIndex.row === index ? tema.colorBoton : tema.colorBarra
                    radius: 5

                    ColumnLayout{
                        anchors.fill: parent
                        anchors.margins: 10

                        Image{
                            id: imagenPortadaGrid
                            Layout.preferredHeight: 120
                            Layout.preferredWidth: 120
                            fillMode: Image.PreserveAspectCrop

                            source: portada !== "" ? "file://" + portada : "qrc:/imagenes/cover.png"
                        }

                        Text {
                            Layout.fillWidth: true
                            text: titulo
                            color: tema.colorTexto
                            elide: Text.ElideRight
                            font.pixelSize: 14
                            font.bold: true
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }

                        Text {
                            Layout.fillWidth: true
                            text: artista
                            color: tema.colorTexto
                            elide: Text.ElideRight
                            font.pixelSize: 12
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                    }

                    MouseArea{
                        anchors.fill: parent

                        onClicked:{
                            tabla.selectionModel.setCurrentIndex(tabla.model.index(index, 0), ItemSelectionModel.ClearAndSelect | ItemSelectionModel.Rows)

                            idSeleccionada = index
                        }

                        onDoubleClicked: {
                            reproducirCancion(idCancion, index)
                            reproductor.reproducir(controlador.obtenerRuta(idCancion))
                            repBoton.checked = true
                        }
                    }
                }
            }
        }
    }
}