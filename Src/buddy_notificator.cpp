#include "buddy_notificator.hpp"
#include "graph_cell_item.hpp"

#include <QGraphicsItem>

BuddyNotificator::BuddyNotificator(QGraphicsItem& owner) : owner_{owner} {}

void BuddyNotificator::buddyHoveringStateChanged(bool new_state)
{
    buddy_is_hovered_ = new_state;
    owner_.update();
}

bool BuddyNotificator::buddyIsHovered() const
{
    return buddy_is_hovered_;
}
