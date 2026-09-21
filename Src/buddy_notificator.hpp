#pragma once

class QGraphicsItem;

class BuddyNotificator
{
public:
    BuddyNotificator(QGraphicsItem& owner);

    void BuddyHoveringStateChanged(bool new_state);
    bool BuddyIsHovered() const { return buddy_is_hovered_; }

private:
    QGraphicsItem& owner_;
    bool           buddy_is_hovered_ = false;
};
