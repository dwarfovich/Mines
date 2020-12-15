#include "board_factory.hpp"

std::unordered_map<QString, std::unique_ptr<IBoard>> BoardFactory::boards_{};

BoardFactory::BoardFactory()
{}

void BoardFactory::registerBoard(std::unique_ptr<IBoard> board)
{
    const auto& name = board->name();
    auto iter = boards_.find(name);
    if (iter == boards_.cend()) {
        boards_.insert(iter, {name, std::move(board)});
    } else {
        // TODO: handle error
      }
}

std::unique_ptr<IBoard> BoardFactory::create(const QString &boardName)
{
    auto iter = boards_.find(boardName);
    if (iter == boards_.cend()) {
        return nullptr;
    } else {
        return iter->second->clone();
    }
}

const std::unordered_map<QString, std::unique_ptr<IBoard>> &BoardFactory::boards()
{
    return boards_;
}
