#pragma once
class Sprite;

class InputComponent {
public:
    virtual ~InputComponent() = default;
    virtual void update(class Sprite& sprite, float elapsed) = 0;
};