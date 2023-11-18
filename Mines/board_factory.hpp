#ifndef BOARDFACTORY_HPP
#define BOARDFACTORY_HPP

#include "iboard.hpp"

#include <QCoreApplication>

#include <unordered_map>
#include <memory>

class BoardFactory
{
    Q_DECLARE_TR_FUNCTIONS(BoardFactory);

public:
    BoardFactory();

    static void registerBoard(std::unique_ptr<IBoard> board);
    static std::unique_ptr<IBoard> create(const QString& boardName);
    static const std::unordered_map<QString, std::unique_ptr<IBoard>>& boards();

private:
    static std::unordered_map<QString, std::unique_ptr<IBoard>> boards_;
};

#endif // BOARDFACTORY_HPP
