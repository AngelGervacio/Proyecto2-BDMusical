src_archivos = files(
    'minero/Minero.cpp',
    'bd/AdministraBD.cpp',
    'bd/Metadatos.cpp',
    'bd/Album.cpp',
    'bd/Artista.cpp',
    'bd/Banda.cpp',
    'bd/Persona.cpp',
    'bd/dao/AlbumsDAO.cpp',
    'bd/dao/ArtistasDAO.cpp',
    'bd/dao/CancionesDAO.cpp',
    'bd/dao/BandasDAO.cpp',
    'bd/dao/PersonasDAO.cpp'
)

executable(
    'Minero',
    'minero/MineroMain.cpp',
    src_archivos,
    dependencies: [sqlite, utfcpp, taglib],
    include_directories: include_dir
)

subdir('gui')