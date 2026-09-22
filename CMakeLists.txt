cmake_minimum_required(VERSION 3.20)

project("Mines")

include(${CMAKE_SOURCE_DIR}/qt.cmake)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/Builds-${CMAKE_BUILD_TYPE}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/Builds-${CMAKE_BUILD_TYPE}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/Builds-${CMAKE_BUILD_TYPE}")

function(deploy_qt TARGET)
    if(NOT WIN32)
        return() 
    endif()

    get_target_property(QT_QMAKE Qt6::qmake IMPORTED_LOCATION)

    if(NOT QT_QMAKE)
        message(FATAL_ERROR "Cannot find Qt6::qmake")
    endif()

    get_filename_component(QT_BIN_DIR "${QT_QMAKE}" DIRECTORY)
    set(WINDEPLOYQT "${QT_BIN_DIR}/windeployqt.exe")

    if(NOT EXISTS "${WINDEPLOYQT}")
        message(FATAL_ERROR "Cannot find windeployqt: ${WINDEPLOYQT}")
    endif()

    add_custom_command(TARGET ${TARGET} POST_BUILD
        COMMAND "${WINDEPLOYQT}"
        --no-translations
            "$<TARGET_FILE:${TARGET}>"
        COMMENT "Deploying Qt dependencies for ${TARGET}"
        VERBATIM
    )
endfunction()

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)
qt_standard_project_setup()
set(PROJECT_SOURCES
    Src/abstract_board.hpp
    Src/parameters_widget_holder.hpp
    Src/board_collection.cpp
    Src/board.hpp
    Src/buddy_notificator.cpp
    Src/cell.cpp
    Src/circle.cpp
    Src/delaunay_board.cpp
    Src/delaunay_parameters_widget.cpp
    Src/direction.cpp
    Src/dynamic_graph_board.cpp
    Src/dynamic_graph_cell_item.cpp
    Src/dynamic_graph_parameters_widget.cpp
    Src/edge_item.cpp
    Src/edge.cpp
    Src/graph_board.cpp
    Src/graph_cell_item.cpp
    Src/graph_parameters_widget.cpp
    Src/hex_board.cpp
    Src/hex_parameters_widget.cpp
    Src/main.cpp
    Src/mec.cpp
    Src/polyomino_board.cpp
    Src/polyomino_cell_item.cpp
    Src/polyomino_cell.cpp
    Src/polyomino_parameters_widget.cpp
    Src/qline_hasher.cpp
    Src/qpoint_hasher.cpp
    Src/qpointf_hasher.cpp
    Src/rectangle_board_parameters_widget.cpp
    Src/rectangle_board.cpp
    Src/triangle.cpp
    Src/triangulator.cpp
    Src/utils.cpp

    Src/utils.hpp
    Src/game_state.hpp
    Src/board_collection.hpp
    Src/cell.hpp

    Src/gui/board_scene.cpp
    Src/gui/board_view.cpp
    Src/gui/cell_item.cpp
    Src/gui/game_over_dialog.cpp
    Src/gui/main_window.cpp
    Src/gui/mines_widget.cpp
    Src/gui/new_game_dialog.cpp
    Src/gui/sprite_cell_item.cpp

    Src/gui/game_over_dialog.ui
    Src/gui/main_window.ui
    Src/gui/mines_widget.ui
    Src/gui/new_game_dialog.ui
)

qt_add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

qt_add_resources(${PROJECT_NAME} resources
    PREFIX "/"
    BASE Src
    FILES
        Src/gfx/app_icon.ico
        Src/gfx/cells_square.png
        Src/gfx/cells_hex.png
        Src/gfx/cells_round.png
        Src/gfx/transparent_images.png
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/Src
)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        WIN32_EXECUTABLE TRUE
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

deploy_qt(${PROJECT_NAME})

## Auxiliary output files folders
set_target_properties(${PROJECT_NAME} PROPERTIES
    COMPILE_PDB_OUTPUT_DIRECTORY
        "${CMAKE_SOURCE_DIR}/Builds-${CMAKE_BUILD_TYPE}/${PROJECT_NAME}_aux"

    PDB_OUTPUT_DIRECTORY
        "${CMAKE_SOURCE_DIR}/Builds-${CMAKE_BUILD_TYPE}/${PROJECT_NAME}_aux"
)
set_target_properties(${PROJECT_NAME} PROPERTIES
    LINK_FLAGS
        "/ILK:\"${CMAKE_SOURCE_DIR}/Builds-${CMAKE_BUILD_TYPE}/${PROJECT_NAME}_aux/${PROJECT_NAME}.ilk\""
)



