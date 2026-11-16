src_archivos = files(
    'minero/Minero.cpp',
    'bd/AdministraBD.cpp',
    'bd/BaseDatosDAO.cpp',
    'bd/Metadatos.cpp',
    'bd/Album.cpp',
    'bd/Artista.cpp',
    'bd/Banda.cpp',
    'bd/Persona.cpp'
)

executable(
    'Minero',
    'minero/MineroMain.cpp',
    src_archivos,
    dependencies: [sqlite, utfcpp, taglib],
    include_directories: include_dir
)

subdir('gui')