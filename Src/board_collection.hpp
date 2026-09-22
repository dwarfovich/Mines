#ifndef BOARD_COLLECTION_HPP
#define BOARD_COLLECTION_HPP

#include "board.hpp"

#include <memory>
#include <unordered_map>

class BoardCollection {
public:
    using BoardsMap = std::unordered_map<QString, std::unique_ptr<Board>>;

    BoardCollection() = default;
    BoardCollection(const BoardCollection&) = delete;
    BoardCollection(BoardCollection&&) = delete;
    BoardCollection& operator=(const BoardCollection&) = delete;
    BoardCollection& operator=(BoardCollection&&) = delete;

    void             registerBoard(std::unique_ptr<Board> board);
    Board*           get(const QString& boardName);
    const BoardsMap& boards();

private:
    BoardsMap boards_;
};

#endif  // BOARD_COLLECTION_HPP
