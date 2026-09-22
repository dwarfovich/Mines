#pragma once

class QGraphicsItem;

class BuddyNotificator {
public:
    BuddyNotificator(QGraphicsItem& owner);

    void buddyHoveringStateChanged(bool new_state);
    bool buddyIsHovered() const;

private:
    QGraphicsItem& owner_;
    bool           buddy_is_hovered_ = false;
};
