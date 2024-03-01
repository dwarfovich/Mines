#include "gui/main_window.hpp"
#include "board_collection.hpp"
#include "rectangle_board.hpp"
#include "hex_board.hpp"
#include "polyomino_board.hpp"
#include "graph_board.hpp"
#include "delaunay_board.hpp"
#include "dynamic_graph_board.hpp"

#ifdef win32
#include "Windows.h"
#endif

#include <QApplication>

int main(int argc, char *argv[])
{
#if defined(QT_DEBUG) && defined(win32)
    if (AttachConsole(ATTACH_PARENT_PROCESS) || AllocConsole()) {
        freopen("CONOUT$", "w", stdout);
        freopen("CONOUT$", "w", stderr);
        freopen("CONIN$", "r", stdin);
    }
#endif

    QApplication a { argc, argv };

    auto collection = std::make_unique<BoardCollection>();
    collection->registerBoard(std::make_unique<RectangleBoard>());
    collection->registerBoard(std::make_unique<HexBoard>());
    collection->registerBoard(std::make_unique<GraphBoard>());
    collection->registerBoard(std::make_unique<DelaunayBoard>());
    collection->registerBoard(std::make_unique<PolyominoBoard>());
    collection->registerBoard(std::make_unique<DynamicGraphBoard>());

    MainWindow w { std::move(collection) };
    w.show();

    return a.exec();
}
