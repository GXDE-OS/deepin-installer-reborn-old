# Vendored copy of qtermwidget 2.4.0 (https://github.com/lxqt/qtermwidget).
# Slimmed down for a static build inside deepin-installer: no lxqt-build-tools,
# no pkg-config file, no example/python bindings. Data files (kb-layouts and
# color-schemes) are installed to ${CMAKE_INSTALL_DATADIR}/qtermwidget6 and
# loaded from there at runtime (upstream dropped the bundled .qrc resources).
cmake_minimum_required(VERSION 3.18.0 FATAL_ERROR)

project(qtermwidget VERSION 2.4.0)

include(GNUInstallDirs)
include(GenerateExportHeader)
include(CheckFunctionExists)

option(USE_UTF8PROC "Use libutf8proc for better Unicode support. Default OFF" OFF)
if(USE_UTF8PROC)
    find_package(Utf8Proc REQUIRED)
endif()

set(QTERMWIDGET_LIBRARY_NAME qtermwidget6)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(SRCS
    lib/BlockArray.cpp
    lib/ColorScheme.cpp
    lib/Emulation.cpp
    lib/Filter.cpp
    lib/History.cpp
    lib/HistorySearch.cpp
    lib/KeyboardTranslator.cpp
    lib/konsole_wcwidth.cpp
    lib/kprocess.cpp
    lib/kpty.cpp
    lib/kptydevice.cpp
    lib/kptyprocess.cpp
    lib/Pty.cpp
    lib/qtermwidget.cpp
    lib/Screen.cpp
    lib/ScreenWindow.cpp
    lib/SearchBar.cpp
    lib/Session.cpp
    lib/ShellCommand.cpp
    lib/TerminalCharacterDecoder.cpp
    lib/TerminalDisplay.cpp
    lib/tools.cpp
    lib/Vt102Emulation.cpp
)

# Only the headers that need to be moc'd go here.
set(HDRS
    lib/Emulation.h
    lib/Filter.h
    lib/HistorySearch.h
    lib/kprocess.h
    lib/kptydevice.h
    lib/kptyprocess.h
    lib/Pty.h
    lib/qtermwidget.h
    lib/ScreenWindow.h
    lib/SearchBar.h
    lib/Session.h
    lib/TerminalDisplay.h
    lib/Vt102Emulation.h
)

set(UI_FILES lib/SearchBar.ui)

# Data dirs, kept in sync with the install rules below.
# These files are private to the installer: installing them into the shared
# ${CMAKE_INSTALL_DATADIR}/qtermwidget6 would conflict with qtermwidget-data.
set(QTERMWIDGET_DATA_DIR "${CMAKE_INSTALL_DATADIR}/deepin-installer/${QTERMWIDGET_LIBRARY_NAME}")
set(KB_LAYOUT_DIR "${CMAKE_INSTALL_FULL_DATADIR}/deepin-installer/${QTERMWIDGET_LIBRARY_NAME}/kb-layouts")
set(COLORSCHEMES_DIR "${CMAKE_INSTALL_FULL_DATADIR}/deepin-installer/${QTERMWIDGET_LIBRARY_NAME}/color-schemes")
set(TRANSLATIONS_DIR "${CMAKE_INSTALL_FULL_DATADIR}/deepin-installer/${QTERMWIDGET_LIBRARY_NAME}/translations")

add_definitions("-DHAVE_POSIX_OPENPT" "-DHAVE_SYS_TIME_H")
CHECK_FUNCTION_EXISTS(updwtmpx HAVE_UPDWTMPX)
if(HAVE_UPDWTMPX)
    add_definitions("-DHAVE_UPDWTMPX")
endif()

add_library(${QTERMWIDGET_LIBRARY_NAME} STATIC
            ${SRCS}
            ${HDRS}
            ${UI_FILES}
            )
target_link_libraries(${QTERMWIDGET_LIBRARY_NAME} Qt6::Widgets)
target_compile_definitions(${QTERMWIDGET_LIBRARY_NAME} PRIVATE
                           "KB_LAYOUT_DIR=\"${KB_LAYOUT_DIR}\""
                           "COLORSCHEMES_DIR=\"${COLORSCHEMES_DIR}\""
                           "TRANSLATIONS_DIR=\"${TRANSLATIONS_DIR}\""
                           )
target_include_directories(${QTERMWIDGET_LIBRARY_NAME} PUBLIC
                           "${CMAKE_CURRENT_SOURCE_DIR}/lib"
                           "${CMAKE_CURRENT_BINARY_DIR}/lib"
                           )

generate_export_header(${QTERMWIDGET_LIBRARY_NAME}
                       EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/lib/qtermwidget_export.h"
                       BASE_NAME QTERMWIDGET
                       )
configure_file(lib/qtermwidget_version.h.in
               "${CMAKE_CURRENT_BINARY_DIR}/lib/qtermwidget_version.h"
               @ONLY
               )

if(UTF8PROC_FOUND)
    target_compile_definitions(${QTERMWIDGET_LIBRARY_NAME} PRIVATE "HAVE_UTF8PROC")
    target_include_directories(${QTERMWIDGET_LIBRARY_NAME} PUBLIC "${UTF8PROC_INCLUDE_DIRS}")
    target_link_libraries(${QTERMWIDGET_LIBRARY_NAME} ${UTF8PROC_LIBRARIES})
endif()

# Terminal data files used by the widget at runtime.
install(DIRECTORY lib/kb-layouts/
        DESTINATION "${QTERMWIDGET_DATA_DIR}/kb-layouts"
        FILES_MATCHING PATTERN "*.keytab"
        )
install(DIRECTORY lib/color-schemes/
        DESTINATION "${QTERMWIDGET_DATA_DIR}/color-schemes"
        FILES_MATCHING PATTERN "*.*schem*"
        )
