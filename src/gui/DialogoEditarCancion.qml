import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtCore

Rectangle{
    id: editarCancionDialogo
    property var tema
    width: 600
    height: 550
    color: tema.colorBarra
    radius: 10

    property alias faltaSeleccionarCancion: faltaSeleccionarCancion
    property var datos: ({
        id: 0,
        ruta: "",
        titulo: "",
        artista: "",
        album: "",
        pista: 0,
        anio: 0,
        genero: "",
        imagen: "",
        imagenArtista: ""
    })
    property url imagenElegida: datos.imagen
    property bool hayCambios: false
    property int idCancion: -1
    property var listaAlbums: []

    signal cancionEliminada()

    onVisibleChanged: {
        if(visible){
            listaAlbums = controlador.obtenerAlbums()
            campoAlbum.currentIndex = campoAlbum.indexOfValue(controlador.obtenerIdAlbum(datos.id))
        }
    }

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

    RowLayout{
        anchors.fill: parent
        anchors.margins: 10
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
            Layout.margins: 10
            spacing: 15

            ColumnLayout{
                Layout.fillWidth: true
                spacing: 10

                RowLayout{
                    Layout.fillWidth: true

                    Label{
                        text: "Ruta"
                        color: tema.colorTexto
                        font.bold: true
                    }

                    Label{
                        text: "* Obligatorio"
                        color: tema.colorSobre
                        font.pixelSize: 8
                        font.bold: true
                    }
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

                    onTextChanged: {
                        if(datos.ruta !== campoRuta.text)
                            hayCambios = true
                        else
                            hayCambios = false
                    }
                }
            }

            ColumnLayout{
                Layout.fillWidth: true
                spacing: 10

                RowLayout{
                    Layout.fillWidth: true

                    Label{
                        text: "Titulo"
                        color: tema.colorTexto
                        font.bold: true
                    }

                    Label{
                        text: "* Obligatorio"
                        color: tema.colorSobre
                        font.pixelSize: 8
                        font.bold: true
                    }
                }

                TextField{
                    id: campoTitulo
                    color: tema.colorTexto
                    text: datos.titulo
                    Layout.fillWidth: true
                    leftPadding: 15
                    rightPadding: 15

                    background: Rectangle{
                        color: tema.colorSobre
                        radius: 5
                    }

                    onTextChanged: {
                        if(datos.titulo !== campoTitulo.text)
                            hayCambios = true
                        else
                            hayCambios = false
                    }
                }
            }

            ColumnLayout{
                Layout.fillWidth: true
                spacing: 10

                RowLayout{
                    Layout.fillWidth: true

                    Label{
                        text: "Artista"
                        color: tema.colorTexto
                        font.bold: true
                    }

                    Label{
                        text: "* Obligatorio"
                        color: tema.colorSobre
                        font.pixelSize: 8
                        font.bold: true
                    }
                }

                TextField{
                    id: campoArtista
                    color: tema.colorTexto
                    text: datos.artista
                    Layout.fillWidth: true
                    leftPadding: 15
                    rightPadding: 15

                    background: Rectangle{
                        color: tema.colorSobre
                        radius: 5
                    }

                    onTextChanged: {
                        if(datos.artista !== campoArtista.text)
                            hayCambios = true
                        else
                            hayCambios = false
                    }
                }
            }

            ColumnLayout{
                Layout.fillWidth: true
                spacing: 10

                Label{
                    text: "Album"
                    color: tema.colorTexto
                    font.bold: true
                }

                ComboBox{
                    id: campoAlbum
                    Layout.fillWidth: true
                    Layout.preferredHeight: 25
                    leftPadding: 15
                    rightPadding: 15

                    textRole: "nombre"
                    valueRole: "id"

                    model: listaAlbums
                    currentIndex: campoAlbum.indexOfValue(controlador.obtenerIdAlbum(datos.id))

                    contentItem: Text {
                        text: campoAlbum.displayText
                        color: tema.colorTexto
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                    }

                    background: Rectangle{
                        color: tema.colorSobre
                        radius: 5
                    }

                    onCurrentIndexChanged: {
                        let idAlbum = controlador.obtenerIdAlbum(datos.id)
                        if(campoAlbum.currentIndex !== campoAlbum.indexOfValue(idAlbum))
                            hayCambios = true
                        else
                            hayCambios = false
                    }
                }
            }

            ColumnLayout{
                Layout.fillWidth: true
                spacing: 10

                Label{
                    text: "Pista"
                    color: tema.colorTexto
                    font.bold: true
                }

                SpinBox{
                    id: campoPista
                    Layout.preferredHeight: 20
                    Layout.preferredWidth: 100
                    editable: true

                    from: 0
                    to: 100
                    value: datos.pista

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

                    onValueChanged: {
                        if(datos.pista !== campoPista.value)
                            hayCambios = true
                        else
                            hayCambios = false
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

                    onValueChanged: {
                        if(datos.anio !== campoAnio.value)
                            hayCambios = true
                        else
                            hayCambios = false
                    }
                }
            }

            ColumnLayout{
                Layout.fillWidth: true
                spacing: 10

                Label{
                    text: "Genero"
                    color: tema.colorTexto
                    font.bold: true
                }

                TextField{
                    id: campoGenero
                    color: tema.colorTexto
                    text: datos.genero
                    Layout.fillWidth: true
                    leftPadding: 15
                    rightPadding: 15

                    background: Rectangle{
                        color: tema.colorSobre
                        radius: 5
                    }

                    onTextChanged: {
                        if(datos.genero !== campoGenero.text)
                            hayCambios = true
                        else
                            hayCambios = false
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
                            editarCancionDialogo.visible = false
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
                            controlador.eliminarCancion(datos.id)
                            editarCancionDialogo.visible = false
                            cancionEliminada()
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
                            campoPista.focus = false
                            let nuevaUrl = imagenElegida
                            if(datos.imagen.toString() === imagenElegida.toString())
                                nuevaUrl = Qt.url("")

                            if(hayCambios)
                                controlador.actualizarCancion(datos.id, campoRuta.text, campoTitulo.text, campoArtista.text, campoAlbum.currentValue, campoPista.value, campoAnio.value, campoGenero.text, nuevaUrl)
                            editarCancionDialogo.visible = false
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