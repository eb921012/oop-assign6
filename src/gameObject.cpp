// TODO implementation

#include "gameObject.h"

Position GameObject::getPosition() const {
    return _pos;
}

Icon GameObject::getIcon() const {
    return _icon;
}

void GameObject::setPosition(const Position& pos) {
    _pos = pos;
}

void GameObject::setIcon(const Icon& icon) {
    _icon = icon;
}

void GameObject::update() {
    // Do nothing by default
}

void GameObject::move(int dx, int dy) {
    _pos.x() += dx;
    _pos.y() += dy;
}

