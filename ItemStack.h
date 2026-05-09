#pragma once

struct ItemStack {
    int itemID = 0;
    int count = 0;

    bool isEmpty() const {
        return itemID == 0 || count <= 0;
    }
};