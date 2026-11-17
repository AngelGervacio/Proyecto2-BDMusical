import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtCore

Rectangle{
    id: tipoInterpreteDialogo
    property var tema
    width: 300
    height: 150
    color: tema.colorBarra
    radius: 10

    property alias faltaSeleccionarCancion: faltaSeleccionarCancion
    property var datos: ({
        id: 0,
        tipo: 0,
        nombre: "",
        imagen: ""
    })

    signal editarSolista()
    signal editarBanda()

    ColumnLayout{
        anchors.fill: parent
        anchors.margins: 20
        spacing: 10

        Label{
            text: "Seleccione el tipo de interprete"
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignHCenter
            color: tema.colorTexto
        }

        RowLayout{
            Layout.fillHeight: true
            Layout.fillWidth: true
            spacing: 10

            Button{
                Layout.alignment: Qt.AlignHCenter
                Layout.preferredHeight: 25
                Layout.fillWidth: true

                Label{
                    text: "Persona"
                    font.bold: true
                    color: tema.colorTexto
                    anchors.centerIn: parent
                }

                background: Rectangle{
                    color: mouseAreaPersona.containsMouse ? tema.colorSobre : tema.colorBoton
                    radius: 10
                }

                MouseArea{
                    id: mouseAreaPersona
                    anchors.fill: parent
                    hoverEnabled: true

                    onClicked:{
                        tipoInterpreteDialogo.visible = false
                        editarSolista()
                    }
                }
            }

            Button{
                Layout.alignment: Qt.AlignHCenter
                Layout.preferredHeight: 25
                Layout.fillWidth: true

                Label{
                    text: "Banda"
                    font.bold: true
                    color: tema.colorTexto
                    anchors.centerIn: parent
                }

                background: Rectangle{
                    color: mouseAreaBanda.containsMouse ? tema.colorSobre : tema.colorBoton
                    radius: 10
                }

                MouseArea{
                    id: mouseAreaBanda
                    anchors.fill: parent
                    hoverEnabled: true

                    onClicked: {
                        tipoInterpreteDialogo.visible = false
                        editarBanda()
                    }
                }
            }
        }

        Button{
            Layout.alignment: Qt.AlignHCenter
            Layout.preferredHeight: 25
            Layout.fillWidth: true

            Label{
                text: "Cancelar"
                font.bold: true
                color: tema.colorTexto
                anchors.centerIn: parent
            }

            background: Rectangle{
                color: mouseAreaCancelar.containsMouse ? tema.colorSobre : tema.colorBoton
                radius: 10
            }

            MouseArea{
                id: mouseAreaCancelar
                anchors.fill: parent
                hoverEnabled: true

                onClicked: {
                    tipoInterpreteDialogo.visible = false
                }
            }
        }
    }

    Dialog{
        id: faltaSeleccionarCancion
        title: "Falta seleccionar una canción."
        anchors.centerIn: parent
        modal: true
        standardButtons: Dialog.Ok

        Label {
            text: "Primero seleccione una canción de la tabla de canciones."
        }

        onAccepted: faltaSeleccionarCancion.close()
    }
}