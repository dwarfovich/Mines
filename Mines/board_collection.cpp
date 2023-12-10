#include "board_collection.hpp"

BoardCollection::BoardCollection()
{
}

void BoardCollection::registerBoard(std::unique_ptr<Board> board)
{
    const auto &name = board->name();
    auto        iter = boards_.find(name);
    if (iter == boards_.cend()) {
        boards_.insert(iter, { name, std::move(board) });
    }
}

Board *BoardCollection::get(const QString &boardName)
{
    auto iter = boards_.find(boardName);
    if (iter == boards_.cend()) {
        return nullptr;
    } else {
        return iter->second.get();
    }
}

const std::unordered_map<QString, std::unique_ptr<Board>> &BoardCollection::boards()
{
    return boards_;
}
