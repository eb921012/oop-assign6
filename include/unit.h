#ifndef UNIT_H
#define UNIT_H
struct Vec2{
    int e1;
    int e2;
    int& x(){ return e1;}
    int& y(){ return e2;}
    int& width(){return e1;}
    int& height(){return e2;}
};

using Position = Vec2;

enum Color {BLACK=0, RED, GREEN, YELLOW, BLUE, PINK, CYAN, WHITE, NOCHANGE};

inline bool operator==(const Vec2& a, const Vec2& b) {
    return a.e1 == b.e1 && a.e2 == b.e2;
}

#endif
