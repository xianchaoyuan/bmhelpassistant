cmake_minimum_required(VERSION 3.5)

project(bmhelpassistant VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Widgets Help WebEngineWidgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets Help WebEngineWidgets)

set(PROJECT_SOURCES
    main.cpp
    helpassistant.h helpassistant.cpp
    helpfindwidget.h helpfindwidget.cpp
    globalactions.h globalactions.cpp
    centralwidget.h centralwidget.cpp
    contentwidget.h contentwidget.cpp
    openpageswidget.h openpageswidget.cpp

    collectionconfiguration.h collectionconfiguration.cpp
    openpagesmodel.h openpagesmodel.cpp
    openpagesmanager.h openpagesmanager.cpp

    helpviewer_p.h
    helpviewer.h helpviewer.cpp

    helpschemehandler.h
    helpenginewrapper_p.h
    helpenginewrapper.h helpenginewrapper.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(bmhelpassistant
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET bmhelpassistant APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(bmhelpassistant SHARED
            ${PROJECT_SOURCES}
        )
    # Define properties for Android with Qt 5 after find_package() calls as:
    #    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
else()
    add_executable(bmhelpassistant
        ${PROJECT_SOURCES}
        bmhelpassistant.qrc
    )
endif()
endif()

target_link_libraries(bmhelpassistant PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Help
    Qt${QT_VERSION_MAJOR}::WebEngineWidgets
)

set_target_properties(bmhelpassistant PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS bmhelpassistant
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(bmhelpassistant)
endif()
