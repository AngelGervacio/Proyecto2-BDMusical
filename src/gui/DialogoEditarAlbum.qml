import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtCore

Rectangle{
    id: editarAlbumDialogo
    property var tema
    width: 600
    height: 350
    color: tema.colorBarra
    radius: 10

    property alias faltaSeleccionarCancion: faltaSeleccionarCancion
    property var datos: ({
        id: 0,
        ruta: "",
        nombre: "",
        anio: 0,
        imagen: ""
    })
    property url imagenElegida: datos.imagen

    property int idCancion: -1

    FileDialog{
        id: dialogoImagen
        title: "Elegir imagen"
        nameFilters: ["Imagenes (*.png *.jpg *.jpeg *.webp)"]
        currentFolder: StandardPaths.standardLocations(StandardPaths.HomeLocation)[0]

        onAccepted: {
            imagenElegida = selectedFile
        }
    }

    RowLayout{
        anchors.fill: parent
        anchors.margins: 15
        spacing: 10

        ColumnLayout{
            Layout.fillHeight: true
            spacing: 10
            Image{
                cache: false
                Layout.preferredHeight: 200
                Layout.preferredWidth: 200
                Layout.margins: 10

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
        }

        ColumnLayout{
            Layout.fillWidth: true
            Layout.margins: 20
            spacing: 20

            ColumnLayout{
                Layout.fillWidth: true
                spacing: 10

                Label{
                    text: "Ruta"
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
                    id: campoRuta
                    text: datos.ruta
                    color: tema.colorTexto
                    Layout.fillWidth: true
                    leftPadding: 15
                    rightPadding: 15

                    background: Rectangle{
                        color: tema.colorSobre
                        radius: 5
                    }
                }
            }

            ColumnLayout{
                Layout.fillWidth: true
                spacing: 10

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
                    color: tema.colorTexto
                    text: datos.nombre
                    Layout.fillWidth: true
                    leftPadding: 15
                    rightPadding: 15

                    background: Rectangle{
                        color: tema.colorSobre
                        radius: 5
                    }
                }
            }

            ColumnLayout{
                Layout.fillWidth: true
                spacing: 10

                Label{
                    text: "Año"
                    color: tema.colorTexto
                    font.bold: true
                }

                SpinBox{
                    id: campoAnio
                    Layout.preferredHeight: 20
                    Layout.preferredWidth: 100
                    editable: true

                    from: 0
                    to: new Date().getFullYear()
                    value: datos.anio

                    textFromValue: function(value, locale){
                        return value.toString()
                    }

                    contentItem: TextInput{
                        text: parent.textFromValue(parent.value, parent.locale)
                        color: tema.colorTexto
                        horizontalAlignment: Qt.AlignHCenter
                        verticalAlignment: Qt.AlignVCenter

                        readOnly: !parent.editable
                        validator: parent.validator
                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                    }

                    background: Rectangle{
                        color: tema.colorSobre
                        radius: 5
                    }
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
                            editarAlbumDialogo.visible = false
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
                            controlador.eliminarAlbum(datos.id)
                            editarAlbumDialogo.visible = false
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
                            campoAnio.focus = false
                            let nuevaUrl = imagenElegida
                            if(datos.imagen.toString() === imagenElegida.toString())
                                nuevaUrl = Qt.url("")

                            if(datos.ruta !== campoRuta.text || datos.nombre !== campoNombre.text || datos.anio !== campoAnio.value || datos.imagen.toString() !== imagenElegida.toString()){
                                if(datos.id === 0){
                                    controlador.agregarAlbum(campoRuta.text, campoNombre.text, campoAnio.value, idCancion, nuevaUrl)
                                }else{
                                    controlador.actualizarAlbum(campoRuta.text, campoNombre.text, campoAnio.value, datos.id, nuevaUrl)
                                }
                            }
                            editarAlbumDialogo.visible = false
                        }
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
