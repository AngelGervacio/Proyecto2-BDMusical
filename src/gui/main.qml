import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.qmlmodels
import QtQuick.Effects

ApplicationWindow {
    width: 800
    height: 600
    visible: true
    title: qsTr("Base De Datos Musical")

    TemaInterfaz{
        id: temaInterfaz
    }

    Component.onCompleted: temaInterfaz.setTemaClaro()

    header: HeaderInterfaz{
        id: headerInterfaz

        tema: temaInterfaz
    }

    StackView{
        id: vista
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
            }
        }

        Component{
            id: paginaCancion

            Cancion{
                id: cancion

                tema: temaInterfaz
            }
        }
    }

    footer: FooterInterfaz{
        id: footerInterfaz

        tema: temaInterfaz
        vista: vista
        header: headerInterfaz
    }
}
