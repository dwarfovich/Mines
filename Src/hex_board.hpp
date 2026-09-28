#pragma once

#include "rectangle_board.hpp"

class HexBoard : public RectangleBoard {
    Q_OBJECT

public:
    const QString& id() const override;
    const QString& name() const override;
    void           setupScene(BoardScene* scene) override;
    void           generate() override;

protected:
    std::vector<std::size_t> neighborIds(std::size_t id) const override;
};