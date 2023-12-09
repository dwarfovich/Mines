#include "gui/main_window.hpp"
#include "board_factory.hpp"
#include "rectangle_board.hpp"
#include "hex_board.hpp"
#include "graph_board/delaunay_board.hpp"
#include "Windows.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    if (AttachConsole(ATTACH_PARENT_PROCESS) || AllocConsole()) {
        freopen("CONOUT$", "w", stdout);
        freopen("CONOUT$", "w", stderr);
        freopen("CONIN$", "r", stdin);
    }

    QApplication a(argc, argv);

    // BoardFactory::registerBoard(std::make_unique<RectangleBoard>());
    // BoardFactory::registerBoard(std::make_unique<HexBoard>());
    BoardFactory::registerBoard(std::make_unique<DelaunayBoard>());

    MainWindow w;
    w.show();
    return a.exec();
}
