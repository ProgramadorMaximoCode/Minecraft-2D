#include "Inventory.h"

Inventory::Inventory(int size) {
    slots.resize(size);
}

ItemStack& Inventory::getSlot(int index) {
    return slots[index];
}

int Inventory::size() const {
    return slots.size();
}

void Inventory::addItem(int itemID, int count) {
    for (auto& slot : slots) {
        if (!slot.isEmpty() && slot.itemID == itemID && slot.count < 64) {
            slot.count += count;
            return;
        }
    }

    for (auto& slot : slots) {
        if (slot.isEmpty()) {
            slot.itemID = itemID;
            slot.count = count;
            return;
        }
    }
}

void Inventory::setCorrectSize() {
    for (auto& slot : slots) {
        if (slot.count > 64) {
            int excess = slot.count - 64;
            slot.count = 64;
            if(excess > 0) {
                for(auto& slotB : slots) {
                    if (slotB.isEmpty()) {
                        slotB.itemID = slot.itemID;
                        slotB.count = excess;
                        break;
                    } else if(slotB.itemID == slot.itemID && slotB.count < 64) {
                        slotB.count += excess;
                        break;
                    }
                }
            }
            
        }
    }
}
