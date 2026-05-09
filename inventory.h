#pragma once
#include <vector>
#include "ItemStack.h"

class Inventory {
public:
    Inventory(int size);

    void addItem(int itemID, int count);

    ItemStack& getSlot(int index);

    int size() const;

private:
    std::vector<ItemStack> slots;
};