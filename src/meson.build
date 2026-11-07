src_archivos = files(
    'minero/Minero.cpp',
    'bd/AdministraBD.cpp',
    'bd/BaseDatosDAO.cpp',
    'bd/Metadatos.cpp'
)

executable(
    'Minero',
    'minero/MineroMain.cpp',
    src_archivos,
    dependencies: [sqlite, utfcpp, taglib],
    include_directories: include_dir
)

subdir('gui')