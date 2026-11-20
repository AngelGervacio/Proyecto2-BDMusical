import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.qmlmodels
import QtQuick.Effects

ApplicationWindow {
    id: ventana
    width: 800
    height: 600
    visible: true
    title: qsTr("Base De Datos Musical")

    property int idRep: -1
    property int filaRep: -1
    property var cancionActual: ({
        id: 0,
        ruta: "",
        titulo: "Titulo",
        artista: "Artista",
        album: "Álbum",
        pista: 0,
        anio: 0,
        genero: "Genero",
        imagen: Qt.url(""),
        imagenArtista: Qt.url("")
    })

    property int filaSeleccionada: -1
    property var dialogoAbierto: controlador.minando || agregarPersonaDialogo.visible ||
                                 editarAlbumDialogo.visible || editarCancionDialogo.visible ||
                                 tipoInterpreteDialogo.visible || editarSolistaDialogo.visible ||
                                 editarBandaDialogo.visible

    TemaInterfaz{
        id: temaInterfaz
    }

    Component.onCompleted: temaInterfaz.setTemaClaro()

    Rectangle{
        anchors.centerIn: parent
        visible: controlador.minando
        width: 250
        height: 150
        color: temaInterfaz.colorBarra
        radius: 10
        z: 10

        Column{
            anchors.centerIn: parent
            anchors.topMargin: 10
            anchors.bottomMargin: 10
            spacing: 10
            AnimatedImage{
                anchors.horizontalCenter: parent.horizontalCenter
                source: headerInterfaz.temaBoton.checked ? "qrc:/imagenes/minandoAnimChecked.gif" : "qrc:/imagenes/minandoAnimNoChecked.gif"
                width: 128
                height: 64
                playing: controlador.minando
            }

            ProgressBar{
                width: 150
                anchors.horizontalCenter: parent.horizontalCenter
                indeterminate: true
            }

            Text{
                text: "Minando directorios..."
                color: temaInterfaz.colorTexto
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }
    }

    header: HeaderInterfaz{
        id: headerInterfaz
        enabled: !dialogoAbierto

        tema: temaInterfaz

        onAgregarPersona: {
            agregarPersonaDialogo.visible = true
        }

        onEditarAlbum: {
            if(filaSeleccionada >= 0){
                let id = cancionesModelo.obtenerId(filaSeleccionada)
                let datos = controlador.obtenerDatosAlbum(id)
                editarAlbumDialogo.idCancion = id
                editarAlbumDialogo.datos = datos
                editarAlbumDialogo.imagenElegida = datos.imagen
                editarAlbumDialogo.visible = true
            }else
                editarAlbumDialogo.faltaSeleccionarCancion.open()
        }

        onEditarArtista: {
            if(filaSeleccionada >= 0){
                let id = cancionesModelo.obtenerId(filaSeleccionada)
                let datos = controlador.obtenerDatosArtista(id)
                if(datos.tipo === 2){
                    tipoInterpreteDialogo.datos = datos
                    tipoInterpreteDialogo.visible = true
                }else if(datos.tipo === 0){
                    editarSolistaDialogo.idCancion = id
                    editarSolistaDialogo.datos = datos
                    editarSolistaDialogo.imagenElegida = datos.imagen
                    editarSolistaDialogo.visible = true
                }else{
                    editarBandaDialogo.idCancion = id
                    editarBandaDialogo.idArtista = datos.id
                    let datosBanda = controlador.obtenerDatosBanda(datos.id, datos.nombre)
                    editarBandaDialogo.datos = datosBanda
                    editarBandaDialogo.fechaInicio = datosBanda.formacion
                    editarBandaDialogo.fechaFinal = datosBanda.separacion
                    editarBandaDialogo.datosArtista = datos
                    editarBandaDialogo.imagenElegida = datos.imagen
                    if(datosBanda.id !== 0){
                        miembrosBanda.personasBanda = controlador.personasEnBanda(datosBanda.id)
                        miembrosBanda.personasDisponibles = controlador.personasDisponibles(datosBanda.id)
                    }else{
                        miembrosBanda.personasBanda = []
                        miembrosBanda.personasEliminar = []
                        miembrosBanda.personasDisponibles = controlador.obtenerPersonas()
                    }
                    miembrosBanda.personasEliminar = []
                    miembrosBanda.eliminarRespaldo = miembrosBanda.personasEliminar.slice()
                    miembrosBanda.bandaRespaldo = miembrosBanda.personasBanda.slice()
                    miembrosBanda.disponiblesRespaldo = miembrosBanda.personasDisponibles.slice()
                    editarBandaDialogo.hayCambios = false
                    editarBandaDialogo.visible = true
                }
            }else
                tipoInterpreteDialogo.faltaSeleccionarCancion.open()
        }

        onEditarCancion: {
            if(filaSeleccionada >= 0){
                let id = cancionesModelo.obtenerId(filaSeleccionada)
                let datos = controlador.obtenerDatosCancion(id)
                editarCancionDialogo.idCancion = id
                editarCancionDialogo.datos = datos
                editarCancionDialogo.imagenElegida = datos.imagen
                editarCancionDialogo.hayCambios = false
                editarCancionDialogo.visible = true
            }else
                editarCancionDialogo.faltaSeleccionarCancion.open()
        }
    }

    StackView{
        id: vista
        enabled: !dialogoAbierto
        anchors.fill: parent

        background: Rectangle {
            color: temaInterfaz.colorFondo
        }

        initialItem: paginaTabla

        Component{
            id: paginaTabla

            Tabla{
                id: tabla

                tema: temaInterfaz
                vistaCuadricula: headerInterfaz.cambiarTablaCuadricula.checked
                repBoton: footerInterfaz.repBoton

                onReproducirCancion: function(id, fila){
                    ventana.idRep = id
                    ventana.filaRep = fila
                    ventana.cancionActual = controlador.obtenerDatosCancion(id)
                }

                onIdSeleccionadaChanged: {
                    filaSeleccionada = tabla.idSeleccionada
                }
            }
        }

        Component{
            id: paginaCancion

            Cancion{
                id: cancion

                tema: temaInterfaz
                cancion: ventana.cancionActual
            }
        }
    }

    footer: FooterInterfaz{
        id: footerInterfaz
        enabled: !dialogoAbierto

        tema: temaInterfaz
        vista: vista
        header: headerInterfaz
        idReproduccion: ventana.idRep
        cancion: ventana.cancionActual
        filaRep: ventana.filaRep

        onSolicitarCambio: function(id, fila){
            ventana.idRep = id
            ventana.filaRep = fila
            reproductor.reproducir(controlador.obtenerRuta(id))
            ventana.cancionActual = controlador.obtenerDatosCancion(id)
        }
    }

    DialogoAgregarPersona{
        id: agregarPersonaDialogo
        tema: temaInterfaz
        temaBoton: headerInterfaz.temaBoton
        anchors.centerIn: parent
        visible: false
        z: 10
    }

    DialogoEditarAlbum{
        id: editarAlbumDialogo
        tema: temaInterfaz
        anchors.centerIn: parent
        visible: false
        z: 10
    }

    DialogoEditarCancion{
        id: editarCancionDialogo
        tema: temaInterfaz
        anchors.centerIn: parent
        visible: false
        z: 10

        onCancionEliminada: {
            reproductor.pausar()
            reproductor.detener()
            footerInterfaz.repBoton.checked = false

            if (ventana.filaSeleccionada < cancionesModelo.rowCount()) {
                ventana.filaRep = ventana.filaSeleccionada
                ventana.idRep = cancionesModelo.obtenerId(ventana.filaRep)
                ventana.cancionActual = controlador.obtenerDatosCancion(ventana.idRep)
            } else {
                ventana.filaRep = -1
                ventana.idRep = -1
            }
        }
    }

    DialogoTipoInterprete{
        id: tipoInterpreteDialogo
        tema: temaInterfaz
        anchors.centerIn: parent
        visible: false
        z: 10

        onEditarSolista: {
            let id = cancionesModelo.obtenerId(filaSeleccionada)
            let datos = controlador.obtenerDatosArtista(id)
            editarSolistaDialogo.idCancion = id
            editarSolistaDialogo.datos = datos
            editarSolistaDialogo.imagenElegida = datos.imagen
            editarSolistaDialogo.hayCambios = false
            editarSolistaDialogo.visible = true
        }

        onEditarBanda: {
            let id = cancionesModelo.obtenerId(filaSeleccionada)
            let datos = controlador.obtenerDatosArtista(id)
            editarBandaDialogo.idCancion = id
            editarBandaDialogo.idArtista = datos.id
            let datosBanda = controlador.obtenerDatosBanda(datos.id, datos.nombre)
            editarBandaDialogo.datos = datosBanda
            editarBandaDialogo.fechaInicio = datosBanda.formacion
            editarBandaDialogo.fechaFinal = datosBanda.separacion
            editarBandaDialogo.datosArtista = datos
            editarBandaDialogo.imagenElegida = datos.imagen
            if(datosBanda.id !== 0){
                miembrosBanda.personasBanda = controlador.personasEnBanda(datosBanda.id)
                miembrosBanda.personasDisponibles = controlador.personasDisponibles(datosBanda.id)
            }else{
                miembrosBanda.personasBanda = []
                miembrosBanda.personasEliminar = []
                miembrosBanda.personasDisponibles = controlador.obtenerPersonas()
            }
            miembrosBanda.personasEliminar = []
            miembrosBanda.eliminarRespaldo = miembrosBanda.personasEliminar.slice()
            miembrosBanda.bandaRespaldo = miembrosBanda.personasBanda.slice()
            miembrosBanda.disponiblesRespaldo = miembrosBanda.personasDisponibles.slice()
            editarBandaDialogo.hayCambios = false
            editarBandaDialogo.visible = true
        }
    }

    DialogoEditarSolista{
        id: editarSolistaDialogo
        tema: temaInterfaz
        anchors.centerIn: parent
        visible: false
        z: 11
    }

    DialogoEditarBanda{
        id: editarBandaDialogo
        enabled: !miembrosBanda.visible
        tema: temaInterfaz
        temaBoton: headerInterfaz.temaBoton
        cambioMiembros: miembrosBanda.hayCambios
        enBanda: miembrosBanda.personasBanda
        aEliminar: miembrosBanda.personasEliminar
        anchors.centerIn: parent
        visible: false
        z: 11

        onEditarMiembros: {
            miembrosBanda.visible = true
        }

        onVisibleChanged: {
            if(editarBandaDialogo.visible === false)
                miembrosBanda.hayCambios = false
        }
    }

    DialogoMiembrosBanda{
        id: miembrosBanda
        tema: editarBandaDialogo.tema
        enabled: !editarPersonaDialogo.visible
        anchors.centerIn: parent
        visible: false
        z: 12

        onEditarPersona: function(persona) {
            editarPersonaDialogo.datos = persona
            editarPersonaDialogo.imagenElegida = persona.imagen
            editarPersonaDialogo.fechaNacimiento = persona.nacimiento
            editarPersonaDialogo.fechaFallecimiento = persona.fallecimiento
            editarPersonaDialogo.hayCambios = false
            editarPersonaDialogo.visible = true
        }
    }

    DialogoEditarPersona{
        id: editarPersonaDialogo
        tema: editarBandaDialogo.tema
        temaBoton: headerInterfaz.temaBoton
        anchors.centerIn: parent
        visible: false
        z: 13

        onCerrarEdicion: {
            miembrosBanda.idPersonaSeleccionada = -1
        }
    }

    Dialog{
        id: errorDialogo
        title: "Error."
        anchors.centerIn: parent
        width: 500
        modal: true
        standardButtons: Dialog.Ok
        property alias label: labelError

        Label {
            id: labelError
            text: ""
        }

        onAccepted: errorDialogo.close()
    }

    Dialog{
        id: personaYaInsertadaDialogo
        title: "Persona ya existe."
        anchors.centerIn: parent
        modal: true
        standardButtons: Dialog.Ok

        Label {
            text: "Ya existe una persona con ese nombre artistico."
        }

        onAccepted: personaYaInsertadaDialogo.close()
    }

    Connections{
        target: controlador

        function onPersonaYaInsertada(){
            personaYaInsertadaDialogo.open()
        }

        function onOcurrioError(error){
            errorDialogo.label.text = error
            errorDialogo.open()
        }

        function onCancionActualizada(){
            if(filaRep !== -1 && idRep !== -1){
                ventana.cancionActual = ({
                    id: 0,
                    ruta: "",
                    titulo: "Titulo",
                    artista: "Artista",
                    album: "Álbum",
                    pista: 0,
                    anio: 0,
                    genero: "Genero",
                    imagen: Qt.url(""),
                    imagenArtista: Qt.url("")
                })
                ventana.cancionActual = controlador.obtenerDatosCancion(idRep)
            }
        }

        function onArtistaActualizado(){
            if(filaRep !== -1 && idRep !== -1){
                ventana.cancionActual = ({
                    id: 0,
                    ruta: "",
                    titulo: "Titulo",
                    artista: "Artista",
                    album: "Álbum",
                    pista: 0,
                    anio: 0,
                    genero: "Genero",
                    imagen: Qt.url(""),
                    imagenArtista: Qt.url("")
                })
                ventana.cancionActual = controlador.obtenerDatosCancion(idRep)
            }
        }

        function onAlbumActualizado(){
            if(filaRep !== -1 && idRep !== -1){
                ventana.cancionActual = ({
                    id: 0,
                    ruta: "",
                    titulo: "Titulo",
                    artista: "Artista",
                    album: "Álbum",
                    pista: 0,
                    anio: 0,
                    genero: "Genero",
                    imagen: Qt.url(""),
                    imagenArtista: Qt.url("")
                })
                ventana.cancionActual = controlador.obtenerDatosCancion(idRep)
            }
        }
    }
}
