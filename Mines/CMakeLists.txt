find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)
qt_standard_project_setup()

set(PROJECT_SOURCES
    board_collection.cpp
    board_state.cpp
    board.cpp
    buddy_notificator.cpp
    cell.cpp
    circle.cpp
    delaunay_board.cpp
    delaunay_parameters_widget.cpp
    direction.cpp
    dynamic_graph_board.cpp
    dynamic_graph_cell_item.cpp
    dynamic_graph_parameters_widget.cpp
    edge_item.cpp
    edge.cpp
    graph_board.cpp
    graph_cell_item.cpp
    graph_parameters_widget.cpp
    hex_board.cpp
    hex_parameters_widget.cpp
    main.cpp
    mec.cpp
    polyomino_board.cpp
    polyomino_cell_item.cpp
    polyomino_cell.cpp
    polyomino_parameters_widget.cpp
    qline_hasher.cpp
    qpoint_hasher.cpp
    qpointf_hasher.cpp
    rectangle_board_parameters_widget.cpp
    rectangle_board.cpp
    triangle.cpp
    triangulator.cpp
    utils.cpp

    utils.hpp
    game_state.hpp
    board_collection.hpp
    cell.hpp

    gui/board_scene.cpp
    gui/board_view.cpp
    gui/cell_item.cpp
    gui/game_over_dialog.cpp
    gui/main_window.cpp
    gui/mines_widget.cpp
    gui/new_game_dialog.cpp
    gui/sprite_cell_item.cpp

    gui/game_over_dialog.ui
    gui/main_window.ui
    gui/mines_widget.ui
    gui/new_game_dialog.ui
)

qt_add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

qt_add_resources(${PROJECT_NAME} resources
    PREFIX "/"
    FILES
        gfx/app_icon.ico
        gfx/cells_square.png
        gfx/cells_hex.png
        gfx/cells_round.png
        gfx/transparent_images.png
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/Mines
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