cmake_minimum_required(VERSION 3.25)
project(ProjetC_)

set(CMAKE_CXX_STANDARD 20)

# Activer la gestion automatique de MOC, UIC et RCC pour Qt6
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Spécifier le chemin de Qt6 si nécessaire
set(Qt6_DIR "/usr/lib/x86_64-linux-gnu/cmake/Qt6")

# Trouver Qt6 et le module Widgets
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Ajouter l'exécutable
add_executable(ProjetC_
        main.cpp
        MyScene.h MyScene.cpp
        MainWindow.h MainWindow.cpp
)

# Lier Qt6::Widgets au projet
target_link_libraries(ProjetC_ Qt6::Widgets)
