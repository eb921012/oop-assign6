// TODO 
// Finish the declaration of IconFactory that
// 1. offers some methods to create an Icon.
// 2. encapsulates the constructor from the caller.

#ifndef ICON_FACTORY_H
#define ICON_FACTORY_H

#include "icon.h"

class IconFactory {
public:
    static Icon createPlayer();   // 玩家
    static Icon createRock();     // 石頭
    static Icon createPaper();    // 布
    static Icon createScissors(); // 剪刀
};

#endif

