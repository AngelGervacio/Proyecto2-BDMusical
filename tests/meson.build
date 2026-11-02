test_dir = '-DTEST_DIR="' +  meson.current_source_dir() + '/fixtures"'

test_administraBD = executable(
    'TestAdministraBD',
    'bd/TestAdministraBD.cpp',
    '../src/bd/AdministraBD.cpp',
    dependencies: [gtest, sqlite],
    include_directories: include_dir,
    cpp_args: [test_dir]
)

test_metadatos = executable(
    'TestMetadatos',
    'bd/TestMetadatos.cpp',
    '../src/bd/Metadatos.cpp',
    dependencies: gtest,
    include_directories: include_dir,
    cpp_args: [test_dir]
)

test_basedatosDAO = executable(
    'TestBaseDatosDAO',
    'bd/TestBaseDatosDAO.cpp',
    '../src/bd/BaseDatosDAO.cpp',
    '../src/bd/AdministraBD.cpp',
    '../src/bd/Metadatos.cpp',
    dependencies: [gtest, sqlite],
    include_directories: include_dir,
    cpp_args: [test_dir]
)

test_minero = executable(
    'TestMinero',
    'minero/TestMinero.cpp',
    '../src/minero/Minero.cpp',
    '../src/bd/BaseDatosDAO.cpp',
    '../src/bd/AdministraBD.cpp',
    '../src/bd/Metadatos.cpp',
    dependencies: [gtest, sqlite, utfcpp, taglib],
    include_directories: include_dir,
    cpp_args: [test_dir]
)

test('Pruebas AdministraBD', test_administraBD)
test('Pruebas Metadatos', test_metadatos)
test('Pruebas BaseDatosDAO', test_basedatosDAO)
test('Pruebas Minero', test_minero)