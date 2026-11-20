gui_resources = qt6.compile_resources(
    sources: 'qml.qrc' 
)

moc_archivos = qt6.preprocess(
    moc_headers: [
    '../../include/proyecto2/gui/CancionesModelo.hpp',
    '../../include/proyecto2/gui/Controlador.hpp',
    '../../include/proyecto2/gui/Reproductor.hpp'
    ]
)

executable(
    'BDMusical',
    ['main.cpp', gui_resources, moc_archivos],
    src_archivos,
    'Controlador.cpp',
    'CancionesModelo.cpp',
    'Reproductor.cpp',
    dependencies: [qt6_dep, sqlite, utfcpp, taglib],
    include_directories: include_dir
)