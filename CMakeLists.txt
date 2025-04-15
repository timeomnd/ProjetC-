cmake_minimum_required(VERSION 3.25)
project(ProjetC-)

set(CMAKE_CXX_STANDARD 14)

# Qt & MOC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOMOC_RELAXED_MODE ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(.)
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Multimedia)

# Ressources Qt
qt6_add_resources(RES_SOURCES assets/ressource.qrc)

# Création de l'exécutable
add_executable(ProjetC-
        main.cpp
        MainWindow.cpp
        MainWindow.hpp
        MyScene.cpp
        MyScene.hpp
        Boss.cpp
        Boss.hpp
        Item.cpp
        Item.hpp
        Monster.cpp
        Monster.hpp
        Player.cpp
        Player.hpp
        Map.cpp
        Map.hpp
        ${RES_SOURCES}
)

# Lien avec Qt
target_link_libraries(ProjetC-
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Multimedia
)
