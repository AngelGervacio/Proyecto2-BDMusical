import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle{
    property var tema
    property string fecha: ""
    property bool calendarioVisible: false
    property int mesActual: fecha !== "" ? Number(Qt.formatDate(fecha, "MM")) - 1 : (new Date().getMonth()) - 1
    property int añoActual: fecha !== "" ? Number(Qt.formatDate(fecha, "yyyy")) : new Date().getFullYear()

    width: 400
    height: 300
    visible: calendarioVisible
    clip: true
    color: tema.colorBarra
    anchors.centerIn: parent

    ColumnLayout{
        anchors.fill: parent

        RowLayout{
            Layout.fillHeight: true
            Layout.fillWidth: true

            Button{
                Text{
                    anchors.fill: parent
                    text: "<<"
                    color: tema.colorTexto
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                Layout.preferredWidth: 50
                Layout.preferredHeight: 20

                background: Rectangle{
                    color: mouseAreaAnioAnterior.containsMouse ? tema.colorSobre : tema.colorBoton
                    radius: 10
                }

                MouseArea{
                    id: mouseAreaAnioAnterior
                    anchors.fill: parent
                    hoverEnabled: true

                    onClicked: {
                        if(añoActual > 1)
                            añoActual--
                    }
                }
            }

            Button{
                Text{
                    anchors.fill: parent
                    text: "<"
                    color: tema.colorTexto
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                Layout.preferredWidth: 50
                Layout.preferredHeight: 20

                background: Rectangle{
                    color: mouseAreaMesAnterior.containsMouse ? tema.colorSobre : tema.colorBoton
                    radius: 10
                }

                MouseArea{
                    id: mouseAreaMesAnterior
                    anchors.fill: parent
                    hoverEnabled: true

                    onClicked: {
                        if(mesActual > 0)
                            mesActual--
                        else{
                            mesActual = 11
                            if(añoActual > 1)
                                añoActual--
                        }
                    }
                }
            }

            Label{
                Layout.fillWidth: true
                text: calendario.title
                color: tema.colorTexto
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }

            Button{
                Layout.preferredHeight: 20
                Layout.preferredWidth: 50
                Text{
                    anchors.fill: parent
                    text: ">"
                    color: tema.colorTexto
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                background: Rectangle{
                    color: mouseAreaMesSiguiente.containsMouse ? tema.colorSobre : tema.colorBoton
                    radius: 10
                }

                MouseArea{
                    id: mouseAreaMesSiguiente
                    anchors.fill: parent
                    hoverEnabled: true

                    onClicked: {
                        if(mesActual < 11)
                            mesActual++
                        else{
                            mesActual = 0
                            if(añoActual < new Date().getFullYear())
                                añoActual++
                        }
                    }
                }
            }

            Button{
                Text{
                    anchors.fill: parent
                    text: ">>"
                    color: tema.colorTexto
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                Layout.preferredWidth: 50
                Layout.preferredHeight: 20

                background: Rectangle{
                    color: mouseAreaAnioSiguiente.containsMouse ? tema.colorSobre : tema.colorBoton
                    radius: 10
                }

                MouseArea{
                    id: mouseAreaAnioSiguiente
                    anchors.fill: parent
                    hoverEnabled: true

                    onClicked: {
                        if(añoActual < new Date().getFullYear())
                            añoActual++
                    }
                }
            }
        }

        MonthGrid{
            id: calendario
            Layout.fillWidth: true
            Layout.fillHeight: true

            month: mesActual
            year: añoActual

            delegate: Label {
                text: model.day
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter

                color: model.month === calendario.month ? tema.colorTexto : "gray"
                font.bold: model.today
            }

            onClicked: function(date){
                fecha = Qt.formatDate(date, "yyyy-MM-dd")
                calendarioVisible = false
            }
        }

        Button{
            Text{
                anchors.fill: parent
                text: "Cerrar"
                color: tema.colorTexto
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            Layout.fillWidth: true
            Layout.preferredHeight: 20

            background: Rectangle{
                color: mouseAreaCerrarCalendario.containsMouse ? tema.colorSobre : tema.colorBoton
                radius: 10
            }

            MouseArea{
                id: mouseAreaCerrarCalendario
                anchors.fill: parent
                hoverEnabled: true

                onClicked: {
                    calendarioVisible = false
                }
            }
        }
    }

    z: 11
}
