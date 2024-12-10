cmake_minimum_required(VERSION 3.16)

project(211-Honors-Project VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find the required Qt modules
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Multimedia)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Multimedia)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        chapters.h
        player.h
        quiz.h
        gamewindow.h
        quiz.cpp
        player.cpp
        chapters.cpp
        gamewindow.cpp
        imgs.qrc
        dialoguebox.h
        dialoguebox.cpp
        storyscene.h
        storyscene.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(211-Honors-Project
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}

        storymanager.h
        storymanager.cpp
        mayanpc.h
        herbspot.h
        herbspot.cpp

    )
else()
    if(ANDROID)
        add_library(211-Honors-Project SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(211-Honors-Project
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# Link the Qt Widgets and Multimedia modules to your target
target_link_libraries(211-Honors-Project PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Multimedia)

target_link_libraries(211-Honors-Project PRIVATE Qt6::Core)

# Set target properties for iOS/macOS if needed
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.211-Honors-Project)
endif()
set_target_properties(211-Honors-Project PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Install settings
include(GNUInstallDirs)
install(TARGETS 211-Honors-Project
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Finalize for Qt 6
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(211-Honors-Project)
endif()
