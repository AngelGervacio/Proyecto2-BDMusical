import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.qmlmodels

Rectangle{

    property var tema

    property var vistaCuadricula

    width: parent.width
    height: parent.height
    color: tema.colorFondo

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

                model: TableModel{
                    TableModelColumn{display: "Titulo"}
                    TableModelColumn{display: "Artista"}
                    TableModelColumn{display: "Album"}
                    TableModelColumn{display: "Genero"}

                    rows:[
                        { "Titulo": "Canción 1",
                          "Artista": "Artista 1",
                          "Album": "Álbum 1",
                          "Genero": "Genero 1"},

                        { "Titulo": "Canción 2",
                          "Artista": "Artista 2",
                          "Album": "Álbum 2",
                          "Genero": "Genero 2"}
                    ]
                }

                delegate: Rectangle{
                    implicitHeight: 35;
                    required property int row
                    color: tabla.currentRow === row ? tema.colorSobre : tema.colorBoton

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
                    width: 140
                    height: 180
                    color: tema.colorBoton
                    radius: 5

                    ColumnLayout{
                        anchors.fill: parent
                        anchors.margins: 10

                        Rectangle {
                            Layout.preferredHeight: 120
                            Layout.preferredWidth: 120
                            color: tema.colorSobre
                            radius: 5

                            Text {
                            anchors.centerIn: parent
                            text: "Imagen"
                            font.pixelSize: 12
                            color: tema.colorTexto
                            }
                        }

                        Text {
                            Layout.fillWidth: true
                            text: tabla.model.getRow(index).Titulo
                            color: tema.colorTexto
                            elide: Text.ElideRight
                            font.pixelSize: 14
                            font.bold: true
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }

                        Text {
                            Layout.fillWidth: true
                            text: tabla.model.getRow(index).Artista
                            color: tema.colorTexto
                            elide: Text.ElideRight
                            font.pixelSize: 12
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                }
            }
        }
    }
}