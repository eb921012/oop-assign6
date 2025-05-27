// TODO implementation


#include "iconFactory.h"
#include "icon.h"
#include "unit.h"

//  玩家：用紅色底代表玩家區域
Icon IconFactory::createPlayer() {
    return {
        { Cell(RED, "RR") }
    };
}

//  石頭：紅色底，顯示 "RR"
Icon IconFactory::createRock() {
    return {
        { Cell(RED, "RR") }
    };
}

//  布：綠色底，顯示 "PP"
Icon IconFactory::createPaper() {
    return {
        { Cell(GREEN, "PP") }
    };
}

//  剪刀：藍色底，顯示 "SS"
Icon IconFactory::createScissors() {
    return {
        { Cell(BLUE, "SS") }
    };
}

