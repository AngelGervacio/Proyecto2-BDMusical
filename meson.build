project(
    'proyecto2',
    'cpp',
    default_options: ['cpp_std=c++23']
)

gtest = dependency(
    'gtest',
    fallback: ['gtest', 'gtest_dep']
)

executable(
    'proyecto2',
    'src/main.cpp'
)