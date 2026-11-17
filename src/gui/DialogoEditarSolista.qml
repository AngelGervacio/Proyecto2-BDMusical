import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtCore

Rectangle{
    id: editarSolistaDialogo
    property var tema
    width: 400
    height: 450
    color: tema.colorBarra
    radius: 10

    property alias faltaSeleccionarCancion: faltaSeleccionarCancion
    property var datos: ({
        id: 0,
        tipo: 0,
        nombre: "",
        imagen: ""
    })

    property int idCancion: -1
    property url imagenElegida: datos.imagen
    property bool hayCambios: false
    property int idArtista: -1

    FileDialog{
        id: dialogoImagen
        title: "Elegir imagen"
        nameFilters: ["Imagenes (*.png *.jpg *.jpeg *.webp)"]
        currentFolder: StandardPaths.standardLocations(StandardPaths.HomeLocation)[0]

        onAccepted: {
            imagenElegida = selectedFile
            hayCambios = true
        }
    }

    ColumnLayout{
        anchors.fill: parent
        anchors.margins: 20
        spacing: 10

        Image{
            cache: false
            Layout.preferredHeight: 200
            Layout.preferredWidth: 200
            Layout.margins: 10
            Layout.alignment: Image.AlignHCenter

            source: imagenElegida.toString() === "" ? "qrc:/imagenes/cover.png" : imagenElegida
            fillMode: Image.PreserveAspectFit
        }

        Button{
            Layout.alignment: Qt.AlignHCenter
            Layout.preferredHeight: 25
            Layout.preferredWidth: 150

            Label{
                text: "Elegir Imagen"
                font.bold: true
                color: tema.colorTexto
                anchors.centerIn: parent
            }

            background: Rectangle{
                color: mouseAreaElegirImagen.containsMouse ? tema.colorSobre : tema.colorBoton
                radius: 10
            }

            MouseArea{
                id: mouseAreaElegirImagen
                anchors.fill: parent
                hoverEnabled: true

                onClicked: {
                    dialogoImagen.open()
                }
            }
        }

        Label{
            text: "Nombre"
            color: tema.colorTexto
            font.bold: true
        }

        Label{
            text: "* Obligatorio"
            color: tema.colorSobre
            font.pixelSize: 10
            font.bold: true
        }

        TextField{
            id: campoNombre
            text: datos.nombre
            color: tema.colorTexto
            Layout.fillWidth: true
            leftPadding: 15
            rightPadding: 15

            background: Rectangle{
                color: tema.colorSobre
                radius: 5
            }

            onTextChanged: {
                if(datos.nombre !== campoNombre.text)
                    hayCambios = true
                else
                    hayCambios = false
            }
        }

        RowLayout{
            Layout.fillWidth: true
            spacing: 10

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
                        editarSolistaDialogo.visible = false
                    }
                }
            }

            Button{
                Layout.alignment: Qt.AlignHCenter
                Layout.preferredHeight: 25
                Layout.fillWidth: true

                Label{
                    text: "Eliminar"
                    font.bold: true
                    color: tema.colorTexto
                    anchors.centerIn: parent
                }

                background: Rectangle{
                    color: mouseAreaEliminar.containsMouse ? tema.colorSobre : tema.colorBoton
                    radius: 10
                }

                MouseArea{
                    id: mouseAreaEliminar
                    anchors.fill: parent
                    hoverEnabled: true

                    onClicked: {
                        controlador.eliminarArtista(datos.id)
                        editarSolistaDialogo.visible = false
                    }
                }
            }

            Button{
                Layout.alignment: Qt.AlignHCenter
                Layout.preferredHeight: 25
                Layout.fillWidth: true

                Label{
                    text: "Actualizar"
                    font.bold: true
                    color: tema.colorTexto
                    anchors.centerIn: parent
                }

                background: Rectangle{
                    color: mouseAreaActualizar.containsMouse ? tema.colorSobre : tema.colorBoton
                    radius: 10
                }

                MouseArea{
                    id: mouseAreaActualizar
                    anchors.fill: parent
                    hoverEnabled: true

                    onClicked: {
                        let nuevaUrl = imagenElegida
                        if(datos.imagen.toString() === imagenElegida.toString())
                            nuevaUrl = Qt.url("")

                        if(hayCambios)
                            if(datos.id === 0)
                                controlador.agregarArtista(0, campoNombre.text, nuevaUrl, idCancion)
                            else
                                controlador.actualizarArtista(datos.id, 0, campoNombre.text, nuevaUrl)

                        editarSolistaDialogo.visible = false
                    }
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