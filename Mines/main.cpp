#include "gui/main_window.hpp"
#include "board_collection.hpp"
#include "rectangle_board.hpp"
#include "hex_board.hpp"
#include "graph_boards/graph_board.hpp"
#include "graph_boards/delaunay_board.hpp"

#include "Windows.h"

#include <QApplication>

int main(int argc, char *argv[])
{
#ifdef QT_DEBUG
    if (AttachConsole(ATTACH_PARENT_PROCESS) || AllocConsole()) {
        freopen("CONOUT$", "w", stdout);
        freopen("CONOUT$", "w", stderr);
        freopen("CONIN$", "r", stdin);
    }
#endif

    QApplication a(argc, argv);

    auto collection = std::make_unique<BoardCollection>();
    collection->registerBoard(std::make_unique<RectangleBoard>());
    collection->registerBoard(std::make_unique<HexBoard>());
    collection->registerBoard(std::make_unique<GraphBoard>());
    collection->registerBoard(std::make_unique<DelaunayBoard>());

    MainWindow w { std::move(collection) };
    w.show();

    return a.exec();
}
