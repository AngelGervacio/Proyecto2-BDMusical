import QtQuick 2.15

QtObject{
    property color colorFondo: "#FFFFFF"
    property color colorTexto: "#3d3846"
    property color colorSobre: "#9a9996"
    property color colorBoton: "#C0BFBC"
    property color colorBarra: "#120F19"

    function setTemaClaro(){
        colorFondo = "#FFFFFF"
        colorTexto = "#3d3846"
        colorSobre = "#9a9996"
        colorBoton = "#C0BFBC"
        colorBarra = "#DEDDDA"
    }

    function setTemaOscuro(){
        colorFondo = "#3d3846"
        colorTexto = "#FFFFFF"
        colorSobre = "#5e5c64"
        colorBoton = "#241f31"
        colorBarra = "#120F19"
    }
}
