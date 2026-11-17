import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtCore

Rectangle{
    id: editarBandaDialogo
    property var tema
    property var temaBoton
    property bool cambioMiembros
    property var enBanda
    property var aEliminar
    width: 700
    height: 350
    color: tema.colorBarra
    radius: 10

    property var datosArtista: ({
        id: 0,
        tipo: 0,
        nombre: "",
        imagen: ""
    })

    property var datos: ({
        id: 0,
        nombre: "",
        formacion: "",
        separacion: "",
        imagen: Qt.url("")
    })

    property int idArtista: -1

    signal editarMiembros()

    property bool hayCambios: false
    property int idCancion: -1
    property url imagenElegida: datosArtista.imagen
    property string fechaInicio: datos.formacion
    property string fechaFinal: datos.separacion

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
        id: calendarioInicio
        tema: editarBandaDialogo.tema

        onFechaChanged: {
            fechaInicio = fecha
            if(datos.formacion !== fecha)
                hayCambios = true
        }
    }

    Calendario{
        id: calendarioFinal
        tema: editarBandaDialogo.tema

        onFechaChanged: {
            fechaFinal = fecha
            if(datos.separacion !== fecha)
                hayCambios = true
        }
    }

    RowLayout{
        anchors.fill: parent
        spacing: 10
        enabled: !calendarioInicio.visible && !calendarioFinal.visible

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
                    id: nombre
                    color: tema.colorTexto
                    text: datos.nombre
                    placeholderText: "Nombre..."
                    placeholderTextColor: tema.colorTexto
                    Layout.fillWidth: true
                    leftPadding: 15
                    rightPadding: 15

                    background: Rectangle{
                        color: tema.colorSobre
                        radius: 5
                    }

                    onTextChanged: {
                        if(datos.nombre !== nombre.text)
                            hayCambios = true
                    }
                }
            }

            ColumnLayout{
                Layout.fillWidth: true
                spacing: 10

                Label{
                    text: "Fecha de formación"
                    color: tema.colorTexto
                    font.bold: true
                }

                RowLayout{
                    Layout.fillWidth: true

                    TextField{
                        id: formacion
                        text: fechaInicio
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
                                mouseAreaInicio.containsMouse ? "qrc:/imagenes/botones/calendarioChecked_hover.png" : "qrc:/imagenes/botones/calendarioChecked_normal.png"
                            }else{
                                mouseAreaInicio.containsMouse ? "qrc:/imagenes/botones/calendarioNoChecked_hover.png" : "qrc:/imagenes/botones/calendarioNoChecked_normal.png"
                            }
                        }

                        icon.width: 25
                        icon.height: 25
                        icon.color: "transparent"

                        background: Rectangle{
                            color: mouseAreaInicio.containsMouse ? tema.colorSobre : tema.colorBoton
                            radius: 10
                        }

                        MouseArea{
                            id: mouseAreaInicio
                            anchors.fill: parent
                            hoverEnabled: true

                            onClicked: {
                                calendarioInicio.calendarioVisible = !calendarioInicio.calendarioVisible
                            }
                        }
                    }
                }
            }

            ColumnLayout{
                Layout.fillWidth: true
                spacing: 10

                Label{
                    text: "Fecha de separación"
                    color: tema.colorTexto
                    font.bold: true
                }

                RowLayout{
                    Layout.fillWidth: true

                    TextField{
                        id: separacion
                        text: fechaFinal
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
                                mouseAreaFinal.containsMouse ? "qrc:/imagenes/botones/calendarioChecked_hover.png" : "qrc:/imagenes/botones/calendarioChecked_normal.png"
                            }else{
                                mouseAreaFinal.containsMouse ? "qrc:/imagenes/botones/calendarioNoChecked_hover.png" : "qrc:/imagenes/botones/calendarioNoChecked_normal.png"
                            }
                        }

                        icon.width: 25
                        icon.height: 25
                        icon.color: "transparent"

                        background: Rectangle{
                            color: mouseAreaFinal.containsMouse ? tema.colorSobre : tema.colorBoton
                            radius: 10
                        }

                        MouseArea{
                            id: mouseAreaFinal
                            anchors.fill: parent
                            hoverEnabled: true

                            onClicked: {
                                calendarioFinal.calendarioVisible = !calendarioFinal.calendarioVisible
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
                            fechaInicio = datos.formacion
                            fechaFinal = datos.separacion
                            hayCambios = false
                            editarBandaDialogo.visible = false
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
                            controlador.eliminarBanda(datos.id)
                            controlador.eliminarArtista(idArtista)
                            fechaInicio = datos.formacion
                            fechaFinal = datos.separacion
                            hayCambios = false
                            editarBandaDialogo.visible = false
                        }
                    }
                }

                Button{
                    Layout.alignment: Qt.AlignHCenter
                    Layout.preferredHeight: 25
                    Layout.fillWidth: true

                    Label{
                        text: "Miembros"
                        font.bold: true
                        color: tema.colorTexto
                        anchors.centerIn: parent
                    }

                    background: Rectangle{
                        color: mouseAreaMiembros.containsMouse ? tema.colorSobre : tema.colorBoton
                        radius: 10
                    }

                    MouseArea{
                        id: mouseAreaMiembros
                        anchors.fill: parent
                        hoverEnabled: true

                        onClicked: {
                            editarMiembros()
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

                            if(hayCambios || cambioMiembros)
                                if(idArtista === 0){
                                    controlador.agregarArtista(1, nombre.text, nuevaUrl, idCancion)
                                    controlador.agregarBanda(nombre.text, formacion.text, separacion.text, enBanda, aEliminar)
                                }else{
                                    controlador.actualizarArtista(idArtista, 1, nombre.text, nuevaUrl)
                                    controlador.actualizarBanda(datos.id, nombre.text, formacion.text, separacion.text, enBanda, aEliminar)
                                }
                            fechaInicio = datos.formacion
                            fechaFinal = datos.separacion
                            hayCambios = false
                            editarBandaDialogo.visible = false
                        }
                    }
                }
            }
        }
    }
}
