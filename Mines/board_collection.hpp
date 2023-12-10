#ifndef BOARD_COLLECTION_HPP
#define BOARD_COLLECTION_HPP

#include "board.hpp"

#include <QCoreApplication>

#include <unordered_map>
#include <memory>

class BoardCollection
{
    Q_DECLARE_TR_FUNCTIONS(BoardCollection);

public:
    BoardCollection();

    void                                                       registerBoard(std::unique_ptr<Board> board);
    Board*                                                     get(const QString& boardName);
    const std::unordered_map<QString, std::unique_ptr<Board>>& boards();

private:
    std::unordered_map<QString, std::unique_ptr<Board>> boards_;
};

#endif // BOARD_COLLECTION_HPP
