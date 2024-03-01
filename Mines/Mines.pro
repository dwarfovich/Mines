RC_ICONS = gfx/app_icon.ico
RC_FILE = icon.rc

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++2a

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    board.cpp \
    board_collection.cpp \
    board_state.cpp \
    buddy_notificator.cpp \
    cell.cpp \
    circle.cpp \
    delaunay_board.cpp \
    delaunay_parameters_widget.cpp \
    direction.cpp \
    dynamic_graph_board.cpp \
    dynamic_graph_cell_item.cpp \
    dynamic_graph_parameters_widget.cpp \
    edge.cpp \
    edge_item.cpp \
    graph_board.cpp \
    graph_cell_item.cpp \
    graph_parameters_widget.cpp \
    gui/sprite_cell_item.cpp \
    hex_parameters_widget.cpp \
    mec.cpp \
    polyomino_board.cpp \
    polyomino_cell.cpp \
    polyomino_cell_item.cpp \
    polyomino_parameters_widget.cpp \
    qpoint_hasher.cpp \
    qpointf_hasher.cpp \
    triangle.cpp \
    triangulator.cpp \
    gui/board_scene.cpp \
    gui/board_view.cpp \
    gui/cell_item.cpp \
    gui/game_over_dialog.cpp \
    gui/main_window.cpp \
    gui/mines_widget.cpp \
    gui/new_game_dialog.cpp \
    rectangle_board_parameters_widget.cpp \
    hex_board.cpp \
    main.cpp \
    rectangle_board.cpp \
    utils.cpp

HEADERS += \
    board.hpp \
    board_collection.hpp \
    board_state.hpp \
    buddy_notificator.hpp \
    cell.hpp \
    circle.hpp \
    constants.hpp \
    delaunay_board.hpp \
    delaunay_parameters_widget.hpp \
    direction.hpp \
    dynamic_graph_board.hpp \
    dynamic_graph_cell_item.hpp \
    dynamic_graph_parameters_widget.h \
    edge_item.hpp \
    game_state.hpp \
    edge.hpp \
    graph_board.hpp \
    graph_parameters_widget.hpp \
    graph_boards_constants.hpp \
    graph_boards_parameters.h \
    graph_cell_item.hpp \
    graph_parameters_widget.h \
    gui/sprite_cell_item.hpp \
    hex_parameters_widget.hpp \
    polyomino_board.hpp \
    polyomino_board_constants.hpp \
    polyomino_cell.hpp \
    polyomino_cell_item.hpp \
    polyomino_parameters_widget.hpp \
    qpoint_hasher.hpp \
    qpointf_hasher.hpp \
    triangle.hpp \
    triangulator.hpp \
    gui/board_scene.hpp \
    gui/board_view.hpp \
    gui/cell_item.hpp \
    gui/game_over_dialog.hpp \
    gui/game_over_dialog_answer.hpp \
    gui/main_window.hpp \
    gui/mines_widget.hpp \
    gui/new_game_dialog.hpp \
    rectangle_board_parameters_widget.hpp \
    hex_board.hpp \
    iboard.hpp \
    rectangle_board.hpp \
    utils.hpp

FORMS += \
    delaunay_parameters_widget.ui \
    dynamic_graph_parameters_widget.ui \
    graph_parameters_widget.ui \
    graph_parameters_widget.ui \
    gui/game_over_dialog.ui \
    gui/main_window.ui \
    gui/mines_widget.ui \
    gui/new_game_dialog.ui \
    hex_parameters_widget.ui \
    polyomino_parameters_widget.ui \
    rectangle_board_parameters_widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    mines.qrc

DISTFILES += \
    gfx/app_icon.ico
