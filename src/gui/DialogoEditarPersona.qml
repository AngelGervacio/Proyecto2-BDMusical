import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtCore

Rectangle{
    id: editarPersonaDialogo
    property var tema
    property var temaBoton
    width: 600
    height: 400
    color: tema.colorBarra
    radius: 10

    property var datos: ({
        id: 0,
        artistico: "",
        real: "",
        nacimiento: "",
        fallecimiento: "",
        imagen: Qt.url("")
        })

    property bool hayCambios: false
    property url imagenElegida: datos.imagen
    property string fechaNacimiento: datos.nacimiento
    property string fechaFallecimiento: datos.fallecimiento

    signal cerrarEdicion()

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

    Calendario{
        id: calendarioNacimiento
        tema: editarPersonaDialogo.tema

        onFechaChanged: {
            fechaNacimiento = fecha
            if(datos.nacimiento !== fecha)
                hayCambios = true
        }
    }

    Calendario{
        id: calendarioFallecimiento
        tema: editarPersonaDialogo.tema

        onFechaChanged: {
            fechaFallecimiento = fecha
            if(datos.fallecimiento !== fecha)
                hayCambios = true
        }
    }

    RowLayout{
        anchors.fill: parent
        spacing: 10
        enabled: !calendarioFallecimiento.visible && !calendarioNacimiento.visible

        ColumnLayout{
            Image{
                Layout.preferredHeight: 200
                Layout.preferredWidth: 200
                Layout.margins: 10

                source: imagenElegida == "" ? "qrc:/imagenes/artist.png" : imagenElegida
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
            Layout.topMargin: 15
            Layout.rightMargin: 30
            Layout.bottomMargin: 15
            spacing: 20

            ColumnLayout{
                Layout.fillWidth: true
                spacing: 5

                Label{
                    text: "Nombre Artistico"
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
                    id: nombreArtistico
                    text: datos.artistico
                    color: tema.colorTexto
                    placeholderText: "Nombre Artístico..."
                    placeholderTextColor: tema.colorTexto
                    Layout.fillWidth: true
                    leftPadding: 15
                    rightPadding: 15

                    background: Rectangle{
                        color: tema.colorSobre
                        radius: 5
                    }

                    onTextChanged: {
                        if(datos.artistico !== nombreArtistico.text)
                            hayCambios = true
                    }
                }
            }

            ColumnLayout{
                Layout.fillWidth: true
                spacing: 10

                Label{
                    text: "Nombre Real"
                    color: tema.colorTexto
                    font.bold: true
                }

                TextField{
                    id: nombreReal
                    text: datos.real
                    color: tema.colorTexto
                    placeholderText: "Nombre Real..."
                    placeholderTextColor: tema.colorTexto
                    Layout.fillWidth: true
                    leftPadding: 15
                    rightPadding: 15

                    background: Rectangle{
                        color: tema.colorSobre
                        radius: 5
                    }

                    onTextChanged: {
                        if(datos.real !== nombreReal.text)
                            hayCambios = true
                    }
                }
            }

            ColumnLayout{
                Layout.fillWidth: true
                spacing: 10

                Label{
                    text: "Fecha de Nacimiento"
                    color: tema.colorTexto
                    font.bold: true
                }

                RowLayout{
                    Layout.fillWidth: true

                    TextField{
                        id: nacimiento
                        text: fechaNacimiento
                        color: tema.colorTexto
                        placeholderText: "yyyy-MM-dd"
                        placeholderTextColor: tema.colorTexto
                        Layout.fillWidth: true
                        leftPadding: 15
                        rightPadding: 15
                        readOnly: true

                        background: Rectangle{
                            color: tema.colorSobre
                            radius: 5
                        }
                    }

                    Button{
                        Layout.alignment: Qt.AlignHCenter
                        Layout.preferredHeight: 30
                        Layout.preferredWidth: 30

                        icon.source: {
                            if(temaBoton.checked){
                                mouseAreaNacimiento.containsMouse ? "qrc:/imagenes/botones/calendarioChecked_hover.png" : "qrc:/imagenes/botones/calendarioChecked_normal.png"
                            }else{
                                mouseAreaNacimiento.containsMouse ? "qrc:/imagenes/botones/calendarioNoChecked_hover.png" : "qrc:/imagenes/botones/calendarioNoChecked_normal.png"
                            }
                        }

                        icon.width: 25
                        icon.height: 25
                        icon.color: "transparent"

                        background: Rectangle{
                            color: mouseAreaNacimiento.containsMouse ? tema.colorSobre : tema.colorBoton
                            radius: 10
                        }

                        MouseArea{
                            id: mouseAreaNacimiento
                            anchors.fill: parent
                            hoverEnabled: true

                            onClicked: {
                                calendarioNacimiento.calendarioVisible = !calendarioNacimiento.calendarioVisible
                            }
                        }
                    }
                }
            }

            ColumnLayout{
                Layout.fillWidth: true
                spacing: 10

                Label{
                    text: "Fecha de Fallecimiento"
                    color: tema.colorTexto
                    font.bold: true
                }

                RowLayout{
                    Layout.fillWidth: true

                    TextField{
                        id: fallecimiento
                        text: fechaFallecimiento
                        color: tema.colorTexto
                        placeholderText: "yyyy-MM-dd"
                        placeholderTextColor: tema.colorTexto
                        Layout.fillWidth: true
                        leftPadding: 15
                        rightPadding: 15
                        readOnly: true

                        background: Rectangle{
                            color: tema.colorSobre
                            radius: 5
                        }
                    }

                    Button{
                        Layout.alignment: Qt.AlignHCenter
                        Layout.preferredHeight: 30
                        Layout.preferredWidth: 30

                        icon.source: {
                            if(temaBoton.checked){
                                mouseAreaFallecimiento.containsMouse ? "qrc:/imagenes/botones/calendarioChecked_hover.png" : "qrc:/imagenes/botones/calendarioChecked_normal.png"
                            }else{
                                mouseAreaFallecimiento.containsMouse ? "qrc:/imagenes/botones/calendarioNoChecked_hover.png" : "qrc:/imagenes/botones/calendarioNoChecked_normal.png"
                            }
                        }

                        icon.width: 25
                        icon.height: 25
                        icon.color: "transparent"

                        background: Rectangle{
                            color: mouseAreaFallecimiento.containsMouse ? tema.colorSobre : tema.colorBoton
                            radius: 10
                        }

                        MouseArea{
                            id: mouseAreaFallecimiento
                            anchors.fill: parent
                            hoverEnabled: true

                            onClicked: {
                                calendarioFallecimiento.calendarioVisible = !calendarioFallecimiento.calendarioVisible
                            }
                        }
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
                            cerrarEdicion()
                            editarPersonaDialogo.visible = false
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
                                controlador.actualizarPersona(datos.id, nombreArtistico.text, nombreReal.text, fechaNacimiento, fechaFallecimiento, nuevaUrl)
                            cerrarEdicion()
                            editarPersonaDialogo.visible = false
                        }
                    }
                }
            }
        }
    }

    Dialog{
        id: faltaNombreArtisticoDialogo
        title: "Falta nombre artistico."
        anchors.centerIn: parent
        modal: true
        standardButtons: Dialog.Ok

        Label {
            text: "Hace falta ingresar el nombre artistico."
        }

        onAccepted: faltaNombreArtisticoDialogo.close()
    }
}
