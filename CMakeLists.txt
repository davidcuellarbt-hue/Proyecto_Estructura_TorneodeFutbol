cmake_minimum_required(VERSION 3.19)
project(Sistemafutbol LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets LinguistTools)

qt_standard_project_setup()

qt_add_executable(Sistemafutbol
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    form2.h form2.cpp form2.ui
    Recursos.qrc
    player.h
    playerlist.h
    playerlist.cpp
    ../../estructuras.h
    ../../estructuras.cpp
    estructuras.h
    estructuras.cpp
)

qt_add_translations(
    TARGETS Sistemafutbol
    TS_FILES Sistemafutbol_es_MX.ts
)

target_link_libraries(Sistemafutbol
    PRIVATE
        Qt::Core
        Qt::Widgets
)

install(TARGETS Sistemafutbol
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET Sistemafutbol
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
