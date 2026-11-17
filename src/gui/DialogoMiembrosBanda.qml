import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtCore

Rectangle{
    id: miembrosBanda
    property var tema
    width: 600
    height: 450
    color: tema.colorBarra
    radius: 10

    property var personasDisponibles: []
    property var personasBanda: []
    property var personasEliminar: []

    property var disponiblesRespaldo: []
    property var bandaRespaldo: []
    property var eliminarRespaldo: []

    property int idPersonaSeleccionada: -1
    property bool personaEnDisponibles: true

    property bool hayCambios: false

    signal editarPersona(var persona)

    onVisibleChanged: {
        if(miembrosBanda.visible === true){
            disponiblesRespaldo = personasDisponibles.slice()
            bandaRespaldo = personasBanda.slice()
            eliminarRespaldo = personasEliminar.slice()
        }
    }

    ColumnLayout{
        anchors.fill: parent
        anchors.margins: 20
        spacing: 10

        RowLayout{
            Layout.fillWidth: true
            spacing: 10

            ColumnLayout{
                Layout.fillWidth: true

                Label{
                    text: "Personas disponibles"
                    color: tema.colorTexto
                }

                ListView{
                    id: listaDisponibles
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    clip: true
                    spacing: 5

                    model: miembrosBanda.personasDisponibles

                    delegate: ItemDelegate{
                        width: ListView.view.width
                        text: modelData.artistico
                        palette.text: tema.colorTexto

                        background: Rectangle{
                            color: mouseAreaDisponible.containsMouse ? tema.colorSobre : tema.colorBoton
                            anchors.fill: parent
                            radius: 10
                        }

                        MouseArea{
                            id: mouseAreaDisponible
                            anchors.fill: parent
                            hoverEnabled: true

                            onClicked: {
                                miembrosBanda.idPersonaSeleccionada = modelData.id
                                miembrosBanda.personaEnDisponibles = true
                            }
                        }
                    }
                }
            }

            ColumnLayout{
                Layout.fillWidth: true

                Label{
                    text: "Personas en la banda"
                    color: tema.colorTexto
                }

                ListView{
                    id: listaMiembros
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    clip: true
                    spacing: 5

                    model: miembrosBanda.personasBanda

                    delegate: ItemDelegate{
                        width: ListView.view.width
                        text: modelData.artistico
                        palette.text: tema.colorTexto

                        background: Rectangle{
                            color: mouseAreaMiembro.containsMouse ? tema.colorSobre : tema.colorBoton
                            anchors.fill: parent
                            radius: 10
                        }

                        MouseArea{
                            id: mouseAreaMiembro
                            anchors.fill: parent
                            hoverEnabled: true

                            onClicked: {
                                miembrosBanda.idPersonaSeleccionada = modelData.id
                                miembrosBanda.personaEnDisponibles = false
                            }
                        }
                    }
                }

            }
        }

        RowLayout{
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignLeft
            spacing: 10

            Button{
                Layout.alignment: Qt.AlignHCenter
                Layout.preferredHeight: 25
                Layout.fillWidth: true

                Label{
                    text: miembrosBanda.personaEnDisponibles ? "Agregar" : "Remover"
                    font.bold: true
                    color: tema.colorTexto
                    anchors.centerIn: parent
                }

                background: Rectangle{
                    color: mouseAreaAgregarRemover.containsMouse ? tema.colorSobre : tema.colorBoton
                    radius: 10
                }

                MouseArea{
                    id: mouseAreaAgregarRemover
                    anchors.fill: parent
                    hoverEnabled: true

                    onClicked: {
                        if (idPersonaSeleccionada === -1)
                            return

                        let disponibles = personasDisponibles.slice()
                        let enBanda = personasBanda.slice()

                        if (personaEnDisponibles) {
                            let indiceDisponible = disponibles.findIndex(persona => persona.id === idPersonaSeleccionada)

                            if (indiceDisponible !== -1) {
                                let persona = disponibles.splice(indiceDisponible, 1)[0]
                                enBanda.push(persona)
                            }

                        }else{
                            let indiceBanda = enBanda.findIndex(persona => persona.id === idPersonaSeleccionada)

                            if (indiceBanda !== -1) {
                                let persona = enBanda.splice(indiceBanda, 1)[0]
                                disponibles.push(persona)
                            }
                        }

                        personasDisponibles = disponibles
                        personasBanda = enBanda
                        hayCambios = true
                        idPersonaSeleccionada = -1
                    }
                }
            }

            Button{
                Layout.alignment: Qt.AlignHCenter
                Layout.preferredHeight: 25
                Layout.fillWidth: true

                Label{
                    text: "Editar"
                    font.bold: true
                    color: tema.colorTexto
                    anchors.centerIn: parent
                }

                background: Rectangle{
                    color: mouseAreaEditar.containsMouse ? tema.colorSobre : tema.colorBoton
                    radius: 10
                }

                MouseArea{
                    id: mouseAreaEditar
                    anchors.fill: parent
                    hoverEnabled: true

                    onClicked: {
                        if (idPersonaSeleccionada === -1)
                            return

                        let persona

                        if (personaEnDisponibles) {
                            persona = personasDisponibles.find(p => p.id === idPersonaSeleccionada)
                        }else{
                            persona = personasBanda.find(p => p.id === idPersonaSeleccionada)
                        }

                        if (persona !== undefined) {
                            miembrosBanda.editarPersona(persona)
                        }
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
                        if (idPersonaSeleccionada === -1)
                            return

                        let id = idPersonaSeleccionada

                        if (!personasEliminar.includes(id)) {
                            let eliminadas = personasEliminar.slice()
                            eliminadas.push(id)
                            personasEliminar = eliminadas
                        }

                        personasDisponibles = personasDisponibles.filter(persona => persona.id !== id)
                        personasBanda = personasBanda.filter(persona => persona.id !== id)
                        hayCambios = true
                        idPersonaSeleccionada = -1
                    }
                }
            }
        }

        RowLayout{
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignRight
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
                        personasDisponibles = disponiblesRespaldo.slice()
                        personasBanda = bandaRespaldo.slice()
                        personasEliminar = eliminarRespaldo.slice()
                        hayCambios = false
                        miembrosBanda.visible = false
                    }
                }
            }

            Button{
                Layout.alignment: Qt.AlignHCenter
                Layout.preferredHeight: 25
                Layout.fillWidth: true

                Label{
                    text: "Guardar"
                    font.bold: true
                    color: tema.colorTexto
                    anchors.centerIn: parent
                }

                background: Rectangle{
                    color: mouseAreaGuardar.containsMouse ? tema.colorSobre : tema.colorBoton
                    radius: 10
                }

                MouseArea{
                    id: mouseAreaGuardar
                    anchors.fill: parent
                    hoverEnabled: true

                    onClicked: {
                        miembrosBanda.visible = false
                    }
                }
            }
        }
    }

    Connections{
        target: controlador

        function onPersonaActualizada(){
            let personasActualizadas = controlador.obtenerPersonas()

            miembrosBanda.personasDisponibles = miembrosBanda.personasDisponibles.map(p => {
                let personaActualizada = personasActualizadas.find(actualizada => actualizada.id === p.id)
                return personaActualizada !== undefined ? personaActualizada : p
            })

            miembrosBanda.personasBanda = miembrosBanda.personasBanda.map(p => {
                let personaActualizada = personasActualizadas.find(actualizada => actualizada.id === p.id)
                return personaActualizada !== undefined ? personaActualizada : p
            })
        }
    }
}