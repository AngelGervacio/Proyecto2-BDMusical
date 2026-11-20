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
    'bd/dao/PersonasDAO.cpp',
    'compilador/Token.cpp',
    'compilador/Lexer.cpp',
    'compilador/Vertice.cpp',
    'compilador/Parser.cpp',
    'compilador/ValidadorSemantico.cpp',
    'compilador/Generador.cpp',
    'compilador/Compilador.cpp'
)

executable(
    'Minero',
    'minero/MineroMain.cpp',
    src_archivos,
    dependencies: [sqlite, utfcpp, taglib, qt6_dep],
    include_directories: include_dir
)

subdir('gui')