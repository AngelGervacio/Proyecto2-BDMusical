qt6 = import('qt6')

qt6_dep = dependency(
    'qt6',
    modules: ['Core', 'Gui', 'Qml', 'Quick']
)

gui_resources = qt6.compile_resources(
    sources: 'qml.qrc' 
)

executable(
    'BDMusical',
    ['main.cpp', gui_resources],
    dependencies: qt6_dep
)