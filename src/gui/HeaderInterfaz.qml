import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ToolBar{

    property alias temaBoton: temaBoton
    property alias cambiarTablaCuadricula: cambiarTablaCuadricula

    property var tema

    topPadding: 10
    leftPadding: 12
    rightPadding: 12
    bottomPadding: 10

    background: Rectangle{
        color: tema.colorBarra
    }

    RowLayout{
        anchors.fill: parent
        spacing: 10

        ToolButton{
            id: temaBoton
            checked: false
            Layout.fillWidth: false

            implicitWidth: 40
            implicitHeight: 40

            icon.source: {
                if(checked){
                    mouseAreaTema.containsMouse ? "qrc:/imagenes/botones/temaChecked_hover.png" : "qrc:/imagenes/botones/temaChecked_normal.png"
                }else{
                    mouseAreaTema.containsMouse ? "qrc:/imagenes/botones/temaNoChecked_hover.png" : "qrc:/imagenes/botones/temaNoChecked_normal.png"
                }
            }

            icon.width: 32
            icon.height: 32
            icon.color: "transparent"

            background: Rectangle{
                color: mouseAreaTema.containsMouse ? tema.colorSobre : tema.colorBoton
                radius: 10
            }

            ToolTip.visible: hovered
            ToolTip.text: {
                if(checked)
                    "Cambiar a tema claro."
                else
                    "Cambiar a tema oscuro."
            }
            ToolTip.delay: 200

            MouseArea{
                id: mouseAreaTema
                anchors.fill: parent
                hoverEnabled: true

                onClicked: {
                    if(parent.checked){
                        parent.checked = false
                    }else
                        parent.checked = true
                }
            }

            onCheckedChanged: {
                if(checked){
                    tema.setTemaOscuro()
                }else{
                    tema.setTemaClaro()
                }
            }
        }

        ToolButton{
            id: mineroBoton
            Layout.fillWidth: false

            implicitWidth: 40
            implicitHeight: 40

            icon.source: {
                if(temaBoton.checked){
                    mouseAreaMinero.containsMouse ? "qrc:/imagenes/botones/mineroChecked_hover.png" : "qrc:/imagenes/botones/mineroChecked_normal.png"
                }else{
                    mouseAreaMinero.containsMouse ? "qrc:/imagenes/botones/mineroNoChecked_hover.png" : "qrc:/imagenes/botones/mineroNoChecked_normal.png"
                }
            }

            icon.width: 32
            icon.height: 32
            icon.color: "transparent"

            background: Rectangle{
                color: mouseAreaMinero.containsMouse ? tema.colorSobre : tema.colorBoton
                radius: 10
            }

            ToolTip.visible: hovered
            ToolTip.text: "Minar directorio."
            ToolTip.delay: 200

            MouseArea{
                id: mouseAreaMinero
                anchors.fill: parent
                hoverEnabled: true
            }
        }

        ToolButton{
            id: agregarPersonaBoton
            Layout.fillWidth: false

            implicitWidth: 40
            implicitHeight: 40

            icon.source: {
                if(temaBoton.checked){
                    mouseAreaAgregarPersona.containsMouse ? "qrc:/imagenes/botones/personaChecked_hover.png" : "qrc:/imagenes/botones/personaChecked_normal.png"
                }else{
                    mouseAreaAgregarPersona.containsMouse ? "qrc:/imagenes/botones/personaNoChecked_hover.png" : "qrc:/imagenes/botones/personaNoChecked_normal.png"
                }
            }

            icon.width: 32
            icon.height: 32
            icon.color: "transparent"

            background: Rectangle{
                color: mouseAreaAgregarPersona.containsMouse ? tema.colorSobre : tema.colorBoton
                radius: 10
            }

            ToolTip.visible: hovered
            ToolTip.text: "Agregar persona."
            ToolTip.delay: 200

            MouseArea{
                id: mouseAreaAgregarPersona
                anchors.fill: parent
                hoverEnabled: true
            }
        }

        ToolButton{
            id: editarArtistaBoton
            Layout.fillWidth: false

            implicitWidth: 40
            implicitHeight: 40

            icon.source: {
                if(temaBoton.checked){
                    mouseAreaEditarArtista.containsMouse ? "qrc:/imagenes/botones/artistaChecked_hover.png" : "qrc:/imagenes/botones/artistaChecked_normal.png"
                }else{
                    mouseAreaEditarArtista.containsMouse ? "qrc:/imagenes/botones/artistaNoChecked_hover.png" : "qrc:/imagenes/botones/artistaNoChecked_normal.png"
                }
            }

            icon.width: 32
            icon.height: 32
            icon.color: "transparent"

            background: Rectangle{
                color: mouseAreaEditarArtista.containsMouse ? tema.colorSobre : tema.colorBoton
                radius: 10
            }

            ToolTip.visible: hovered
            ToolTip.text: "Editar artista."
            ToolTip.delay: 200

            MouseArea{
                id: mouseAreaEditarArtista
                anchors.fill: parent
                hoverEnabled: true
            }
        }

        ToolButton{
            id: editarCancionBoton
            Layout.fillWidth: false

            implicitWidth: 40
            implicitHeight: 40

            icon.source: {
                if(temaBoton.checked){
                    mouseAreaEditarCancion.containsMouse ? "qrc:/imagenes/botones/cancionChecked_hover.png" : "qrc:/imagenes/botones/cancionChecked_normal.png"
                }else{
                    mouseAreaEditarCancion.containsMouse ? "qrc:/imagenes/botones/cancionNoChecked_hover.png" : "qrc:/imagenes/botones/cancionNoChecked_normal.png"
                }
            }

            icon.width: 32
            icon.height: 32
            icon.color: "transparent"

            background: Rectangle{
                color: mouseAreaEditarCancion.containsMouse ? tema.colorSobre : tema.colorBoton
                radius: 10
            }

            ToolTip.visible: hovered
            ToolTip.text: "Editar canción."
            ToolTip.delay: 200

            MouseArea{
                id: mouseAreaEditarCancion
                anchors.fill: parent
                hoverEnabled: true
            }
        }

        ToolButton{
            id: editarAlbumBoton
            Layout.fillWidth: false

            implicitWidth: 40
            implicitHeight: 40

            icon.source: {
                if(temaBoton.checked){
                    mouseAreaEditarAlbum.containsMouse ? "qrc:/imagenes/botones/albumChecked_hover.png" : "qrc:/imagenes/botones/albumChecked_normal.png"
                }else{
                    mouseAreaEditarAlbum.containsMouse ? "qrc:/imagenes/botones/albumNoChecked_hover.png" : "qrc:/imagenes/botones/albumNoChecked_normal.png"
                }
            }

            icon.width: 32
            icon.height: 32
            icon.color: "transparent"

            background: Rectangle{
                color: mouseAreaEditarAlbum.containsMouse ? tema.colorSobre : tema.colorBoton
                radius: 10
            }

            ToolTip.visible: hovered
            ToolTip.text: "Editar álbum."
            ToolTip.delay: 200

            MouseArea{
                id: mouseAreaEditarAlbum
                anchors.fill: parent
                hoverEnabled: true
            }
        }

        Item{
            Layout.fillWidth: true
        }

        ToolButton{
            id: cambiarTablaCuadricula
            Layout.fillWidth: false
            Layout.alignment: Qt.AlignRight
            checked: false

            implicitWidth: 40
            implicitHeight: 40

            icon.source: {
                if(temaBoton.checked && checked)
                    mouseAreaCambiar.containsMouse ? "qrc:/imagenes/botones/tablaVistaChecked_hover.png" : "qrc:/imagenes/botones/tablaVistaChecked_normal.png"
                else if(temaBoton.checked)
                    mouseAreaCambiar.containsMouse ? "qrc:/imagenes/botones/cuadriculaChecked_hover.png" : "qrc:/imagenes/botones/cuadriculaChecked_normal.png"
                else if(checked)
                    mouseAreaCambiar.containsMouse ? "qrc:/imagenes/botones/tablaVistaNoChecked_hover.png" : "qrc:/imagenes/botones/tablaVistaNoChecked_normal.png"
                else
                    mouseAreaCambiar.containsMouse ? "qrc:/imagenes/botones/cuadriculaNoChecked_hover.png" : "qrc:/imagenes/botones/cuadriculaNoChecked_normal.png"
            }

            icon.width: 32
            icon.height: 32
            icon.color: "transparent"

            background: Rectangle{
                color: mouseAreaCambiar.containsMouse ? tema.colorSobre : tema.colorBoton
                radius: 10
            }

            ToolTip.visible: hovered
            ToolTip.text: {
                if(checked)
                    "Cambiar a vista de tabla."
                else
                    "Cambiar a vista de cuadricula."
            }

            ToolTip.delay: 200

            MouseArea{
                id: mouseAreaCambiar
                anchors.fill: parent
                hoverEnabled: true

                onClicked:{
                    cambiarTablaCuadricula.checked = !cambiarTablaCuadricula.checked
                }
            }
        }
    }
}
