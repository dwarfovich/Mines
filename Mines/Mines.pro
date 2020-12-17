RC_ICONS = gfx/app_icon.ico

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++2a

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    board_factory.cpp \
    board_state.cpp \
    cell.cpp \
    graph_board/edge.cpp \
    graph_board/graph_board.cpp \
    graph_board/graph_board_parameters_widget.cpp \
    graph_board/qpointf_hasher.cpp \
    graph_board/triangle.cpp \
    graph_board/triangulator.cpp \
    gui/board_scene.cpp \
    gui/board_view.cpp \
    gui/cell_item.cpp \
    gui/game_over_dialog.cpp \
    gui/main_window.cpp \
    gui/mines_widget.cpp \
    gui/new_game_dialog.cpp \
    gui/rectangle_board_parameters_widget.cpp \
    gui/standard_cell_item.cpp \
    hex_board.cpp \
    main.cpp \
    rectangle_board.cpp \
    standard_board.cpp \
    utils.cpp

HEADERS += \
    board_factory.hpp \
    board_state.hpp \
    cell.hpp \
    game_state.hpp \
    graph_board/edge.hpp \
    graph_board/graph_board.hpp \
    graph_board/graph_board_parameters_widget.hpp \
    graph_board/qpointf_hasher.hpp \
    graph_board/triangle.hpp \
    graph_board/triangulator.hpp \
    gui/board_scene.hpp \
    gui/board_view.hpp \
    gui/cell_item.hpp \
    gui/game_over_dialog.hpp \
    gui/game_over_dialog_answer.hpp \
    gui/main_window.hpp \
    gui/mines_widget.hpp \
    gui/new_game_dialog.hpp \
    gui/rectangle_board_parameters_widget.hpp \
    gui/standard_cell_item.hpp \
    hex_board.hpp \
    iboard.hpp \
    rectangle_board.hpp \
    standard_board.hpp \
    utils.hpp

FORMS += \
    graph_board/graph_board_parameters_widget.ui \
    gui/game_over_dialog.ui \
    gui/main_window.ui \
    gui/mines_widget.ui \
    gui/new_game_dialog.ui \
    gui/rectangle_board_parameters_widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    sprites.qrc
