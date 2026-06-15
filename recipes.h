#pragma once 
#include "ItemStack.h"
#include "itemData.h"
 
struct Recipe {
    ItemsName name;
    ItemStack pattern[3][3]; 
    ItemStack result;
};