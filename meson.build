project(
    'proyecto2',
    'cpp',
    default_options: ['cpp_std=c++23']
)

include_dir = include_directories('include')

datos_dir = get_option('prefix') / get_option('datadir') / 'proyecto2'

install_data(
    'sql/rolas.sql',
    install_dir: datos_dir
)

add_project_arguments(
    '-DDATOS_DIR="' + datos_dir + '"',
    language: 'cpp'
)

gtest = dependency(
    'gtest_main',
    fallback: ['gtest', 'gtest_main_dep']
)

sqlite = dependency(
    'sqlite3',
    fallback: ['sqlite3', 'sqlite3_dep']
)

test_administraBD = executable(
    'TestAdministraBD',
    'tests/bd/TestAdministraBD.cpp',
    'src/bd/AdministraBD.cpp',
    dependencies: [gtest, sqlite],
    include_directories: [include_directories('src'), include_dir]
)

executable(
    'proyecto2',
    'src/main.cpp'
)

test('Pruebas AdministraBD', test_administraBD)