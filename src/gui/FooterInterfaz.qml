import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle{

    property var tema
    property var vista
    property var header

    property bool ocultar: false

    width: parent.width
    height: barraReproduccion.implicitHeight + toolBarFooter.implicitHeight
    color: tema.colorBarra

    ColumnLayout{
        anchors.fill: parent

        Slider{
            id: barraReproduccion
            Layout.fillWidth: true

            from: 0
            to: 100
            value: 35

            handle: Rectangle{
                width: 15
                height: 15
                radius: 6
                color: tema.colorSobre

                x: barraReproduccion.leftPadding + barraReproduccion.visualPosition * (barraReproduccion.availableWidth - width)
                y: (barraReproduccion.height - height) / 2
            }
        }

        ToolBar{
            id: toolBarFooter
            Layout.fillWidth: true
            topPadding: 10
            leftPadding: 12
            rightPadding: 12
            bottomPadding: 10

            background: Rectangle{
                color: tema.colorBarra
            }

            GridLayout{
                anchors.fill: parent
                columns: 4
                columnSpacing: 10
                rowSpacing: 0

                RowLayout{
                    Layout.column: 0
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.alignment: Qt.AlignLeft
                    Layout.minimumWidth: 260
                    Layout.maximumWidth: 260

                    Image{
                        id: portadaFooter
                        Layout.alignment: Qt.AlignLeft
                        Layout.preferredHeight: 80
                        Layout.preferredWidth: 80
                        source: "qrc:/imagenes/cover.png"
                        fillMode: Image.PreserveAspectFit

                        y: ocultar ? 100 : 0

                        Behavior on y{
                            NumberAnimation{
                                duration: 400
                                easing.type: Easing.InCurve
                            }
                        }
                    }

                    Column{
                        Layout.topMargin: 10
                        Layout.bottomMargin: 10
                        Layout.alignment: Qt.AlignLeft
                        spacing: 10

                        y: ocultar ? 100 : 20

                        Behavior on y{
                            NumberAnimation{
                                duration: 400
                                easing.type: Easing.InCurve
                            }
                        }

                        Label{
                            text: "Canción"
                            color: tema.colorTexto
                            font.pixelSize: 16
                            font.bold: true
                            width: 180
                            elide: Text.ElideRight
                        }

                        Label{
                            text: "Artista"
                            color: tema.colorTexto
                            font.pixelSize: 14
                            width: 180
                            elide: Text.ElideRight
                        }
                    }
                }

                RowLayout{
                    Layout.column: 1
                    Layout.alignment: Qt.AlignCenter
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    ToolButton{
                        id: anteriorBoton
                        Layout.fillWidth: false
                        Layout.alignment: Qt.AlignRight

                        implicitWidth: 60
                        implicitHeight: 60

                        icon.source: {
                            if(header.temaBoton.checked){
                                mouseAreaAnterior.containsMouse ? "qrc:/imagenes/botones/anteriorChecked_hover.png" : "qrc:/imagenes/botones/anteriorChecked_normal.png"
                            }else{
                                mouseAreaAnterior.containsMouse ? "qrc:/imagenes/botones/anteriorNoChecked_hover.png" : "qrc:/imagenes/botones/anteriorNoChecked_normal.png"
                            }
                        }

                        icon.width: 48
                        icon.height: 48
                        icon.color: "transparent"

                        background: Rectangle{
                            color: mouseAreaAnterior.containsMouse ? tema.colorSobre : tema.colorBoton
                            radius: 10
                        }

                        ToolTip.visible: hovered
                        ToolTip.text: "Anterior."
                        ToolTip.delay: 200

                        MouseArea{
                            id: mouseAreaAnterior
                            anchors.fill: parent
                            hoverEnabled: true
                        }
                    }

                    ToolButton{
                        id: repBoton
                        checked: false
                        Layout.alignment: Qt.AlignCenter

                        width: 80
                        height: 80

                        icon.source: {
                            if(header.temaBoton.checked && checked)
                                mouseAreaRep.containsMouse ? "qrc:/imagenes/botones/pauseChecked_hover.png" : "qrc:/imagenes/botones/pauseChecked_normal.png"
                            else if(header.temaBoton.checked)
                                mouseAreaRep.containsMouse ? "qrc:/imagenes/botones/playChecked_hover.png" : "qrc:/imagenes/botones/playChecked_normal.png"
                            else if(checked)
                                mouseAreaRep.containsMouse ? "qrc:/imagenes/botones/pauseNoChecked_hover.png" : "qrc:/imagenes/botones/pauseNoChecked_normal.png"
                            else
                                mouseAreaRep.containsMouse ? "qrc:/imagenes/botones/playNoChecked_hover.png" : "qrc:/imagenes/botones/playNoChecked_normal.png"
                        }

                        icon.width: 64
                        icon.height: 64
                        icon.color: "transparent"

                        background: Rectangle{
                            color: mouseAreaRep.containsMouse ? tema.colorSobre : tema.colorBoton
                            radius: 10
                        }

                        ToolTip.visible: hovered
                        ToolTip.text: {
                            if(checked)
                                "Reproducir canción."
                            else
                                "Pausar canción."
                        }
                        ToolTip.delay: 500

                        MouseArea{
                            id: mouseAreaRep
                            anchors.fill: parent
                            hoverEnabled: true

                            onClicked: {
                                if(parent.checked){
                                    parent.checked = false
                                }else
                                    parent.checked = true
                            }
                        }
                    }

                    ToolButton{
                        id: siguienteBoton
                        Layout.fillWidth: false

                        implicitWidth: 60
                        implicitHeight: 60

                        icon.source: {
                            if(header.temaBoton.checked){
                                mouseAreaSiguiente.containsMouse ? "qrc:/imagenes/botones/siguienteChecked_hover.png" : "qrc:/imagenes/botones/siguienteChecked_normal.png"
                            }else{
                                mouseAreaSiguiente.containsMouse ? "qrc:/imagenes/botones/siguienteNoChecked_hover.png" : "qrc:/imagenes/botones/siguienteNoChecked_normal.png"
                            }
                        }

                        icon.width: 48
                        icon.height: 48
                        icon.color: "transparent"

                        background: Rectangle{
                            color: mouseAreaSiguiente.containsMouse ? tema.colorSobre : tema.colorBoton
                            radius: 10
                        }

                        ToolTip.visible: hovered
                        ToolTip.text: "Siguiente."
                        ToolTip.delay: 200

                        MouseArea{
                            id: mouseAreaSiguiente
                            anchors.fill: parent
                            hoverEnabled: true
                        }
                    }
                }

                RowLayout{
                    Layout.column: 2
                    Layout.alignment: Qt.AlignRight
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    Column{
                        Layout.topMargin: -20
                        width: parent.width
                        spacing: 20

                        Label{
                            text: "Volumen"
                            color: tema.colorTexto
                            font.pixelSize: 14
                            anchors.horizontalCenter: parent.horizontalCenter
                        }

                        Slider{
                            id: barraVolumen

                            from: 0
                            to: 100
                            value: 35

                            handle: Rectangle{
                                width: 15
                                height: 15
                                radius: 6
                                color: tema.colorSobre

                                x: barraVolumen.leftPadding + barraVolumen.visualPosition * (barraVolumen.availableWidth - width)
                                y: (barraVolumen.height - height) / 2
                            }
                        }
                    }
                }

                RowLayout{
                    Layout.column: 3
                    Layout.alignment: Qt.AlignRight
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    ToolButton{
                        id: vistaBoton
                        checked: false
                        Layout.alignment: Qt.AlignRight

                        Layout.preferredWidth: 80
                        Layout.preferredHeight: 80

                        icon.source: {
                            if(header.temaBoton.checked && checked)
                                mouseAreaVista.containsMouse ? "qrc:/imagenes/botones/tablaChecked_hover.png" : "qrc:/imagenes/botones/tablaChecked_normal.png"
                            else if(header.temaBoton.checked)
                                mouseAreaVista.containsMouse ? "qrc:/imagenes/botones/infoChecked_hover.png" : "qrc:/imagenes/botones/infoChecked_normal.png"
                            else if(checked)
                                mouseAreaVista.containsMouse ? "qrc:/imagenes/botones/tablaNoChecked_hover.png" : "qrc:/imagenes/botones/tablaNoChecked_normal.png"
                            else
                                mouseAreaVista.containsMouse ? "qrc:/imagenes/botones/infoNoChecked_hover.png" : "qrc:/imagenes/botones/infoNoChecked_normal.png"
                        }

                        icon.width: 64
                        icon.height: 64
                        icon.color: "transparent"

                        background: Rectangle{
                            color: mouseAreaVista.containsMouse ? tema.colorSobre : tema.colorBoton
                            radius: 10
                        }

                        ToolTip.visible: hovered
                        ToolTip.text: {
                            if(checked)
                                "Abrir la base de datos."
                            else
                                "Abrir la información de la canción."
                        }
                        ToolTip.delay: 500

                        MouseArea{
                            id: mouseAreaVista
                            anchors.fill: parent
                            hoverEnabled: true

                            onClicked: {
                                if(parent.checked){
                                    parent.checked = false
                                    vista.pop()
                                    ocultar = false
                                }else{
                                    parent.checked = true
                                    vista.push(paginaCancion)
                                    ocultar = true
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
