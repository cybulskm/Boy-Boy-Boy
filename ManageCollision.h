#pragma once
#include "TextureCache.h"
#include "SteamMan.h"
#include "Knight.h"
#include <vector>
#include "SDL3/SDL.h"
#include "CharacterData.h"
#include "Player.h"
#include <iostream>
#include <string>

void handleCollision(Sprite* a, Sprite* b) {
    Player* pA = dynamic_cast<Player*>(a);
    Player* pB = dynamic_cast<Player*>(b);

    if (pA && pB) {
        bool aAttacking = (pA->currentState == SpriteState::ATTACK1 || pA->currentState == SpriteState::ATTACK2);
        bool bAttacking = (pB->currentState == SpriteState::ATTACK1 || pB->currentState == SpriteState::ATTACK2);

        if (aAttacking && !pB->isHurt && !pB->isRolling) {
            pB->hitCount++;
            pB->isHurt = true;
            pB->setColour(255, 0, 0);
            std::cout << pB->name << " HP Hit! Count: " << pB->hitCount << std::endl;
        }
        if (bAttacking && !pA->isHurt && !pA->isRolling) {
            pA->hitCount++;
            pA->isHurt = true;
            pA->setColour(255, 0, 0);
            std::cout << pA->name << " HP Hit! Count: " << pA->hitCount << std::endl;
        }
        return;
    }

    Player* p = pA ? pA : pB;
    Sprite* obj = (p == pA) ? b : a;

    if (p && obj) {
        SDL_FRect pHit = p->getHitbox();
        SDL_FRect oHit = obj->getHitbox();

        float overlapX = (pHit.x + pHit.w / 2 > oHit.x + oHit.w / 2)
            ? (oHit.x + oHit.w) - pHit.x
            : pHit.x + pHit.w - oHit.x;

        float overlapY = (pHit.y + pHit.h / 2 > oHit.y + oHit.h / 2)
            ? (oHit.y + oHit.h) - pHit.y
            : pHit.y + pHit.h - oHit.y;

        if (overlapX < overlapY) {
            p->x += (pHit.x + pHit.w / 2 > oHit.x + oHit.w / 2) ? overlapX : -overlapX;
        }
        else {
            if (p->velocityY > 0 && pHit.y + pHit.h / 2 < oHit.y) {
                p->y -= overlapY;
                p->velocityY = 0;
                p->isJumping = false;
            }
            else {
                p->y += (pHit.y + pHit.h / 2 > oHit.y + oHit.h / 2) ? overlapY : -overlapY;
            }
        }
    }
}

void resolveCollisions(std::vector<Sprite*>& sprites) {
    for (size_t i = 0; i < sprites.size(); ++i) {
        for (size_t j = i + 1; j < sprites.size(); ++j) {
            if (sprites[i]->canCollide && sprites[j]->canCollide) {
                if (sprites[i]->intersects(sprites[j])) {
                    handleCollision(sprites[i], sprites[j]);
                }
            }
        }
    }
}