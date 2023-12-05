#include "gui/main_window.hpp"
#include "board_factory.hpp"
#include "rectangle_board.hpp"
#include "hex_board.hpp"
#include "graph_board/graph_board.hpp"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // BoardFactory::registerBoard(std::make_unique<RectangleBoard>());
    // BoardFactory::registerBoard(std::make_unique<HexBoard>());
    BoardFactory::registerBoard(std::make_unique<GraphBoard>());

    MainWindow w;
    w.show();
    return a.exec();
}
