#pragma once
#include <vector>
#include <set>
#include <string>
#include <iostream>
#include <algorithm>
#include "Sprite.h"
#include "Player.h"

class CollisionManager {
private:
    void handlePhysics(Sprite* a, Sprite* b) {
        Player* pA = dynamic_cast<Player*>(a);
        Player* pB = dynamic_cast<Player*>(b);

        // CASE 1: Player vs Player
        if (pA && pB) {
            bool aAttacking = (pA->currentState == SpriteState::ATTACK1 || pA->currentState == SpriteState::ATTACK2);
            bool bAttacking = (pB->currentState == SpriteState::ATTACK1 || pB->currentState == SpriteState::ATTACK2);

            if (aAttacking && !pB->isHurt && !pB->isSliding && !pB->isCrouching) {
                pB->hitCount++;
                pB->isHurt = true;
                pB->hurtTimer = pB->hurtDuration; 
            }
            if (bAttacking && !pA->isHurt && !pA->isSliding && !pA->isCrouching) {
                pA->hitCount++;
                pA->isHurt = true;
                pA->hurtTimer = pA->hurtDuration;
            }
            return;
        }

        // CASE 2: Player vs Object
        Player* p = pA ? pA : pB;
        Sprite* obj = (p == pA) ? b : a;
        if (p && obj) {
            SDL_FRect pHit = p->getHitbox();
            SDL_FRect oHit = obj->getHitbox();
            float dx = (pHit.x + pHit.w / 2.0f) - (oHit.x + oHit.w / 2.0f);
            float dy = (pHit.y + pHit.h / 2.0f) - (oHit.y + oHit.h / 2.0f);
            float overlapX = ((pHit.w + oHit.w) / 2.0f) - std::abs(dx);
            float overlapY = ((pHit.h + oHit.h) / 2.0f) - std::abs(dy);

            if (overlapX < overlapY) {
                p->x += (dx > 0) ? overlapX : -overlapX;
            }
            else {
                if (dy < 0) { p->y -= overlapY; p->velocityY = 0; p->isGrounded = true; }
                else { p->y += overlapY; p->velocityY = 0; }
            }
        }
    }

public:
    // We now use a set of STRINGS so memory addresses don't matter
    void update(std::vector<Sprite*>& sprites, std::set<std::string>& persistentNames) {
        std::set<std::string> currentFrameNames;

        for (size_t i = 0; i < sprites.size(); ++i) {
            for (size_t j = i + 1; j < sprites.size(); ++j) {
                Sprite* a = sprites[i];
                Sprite* b = sprites[j];

                if (a->canCollide && b->canCollide && a->intersects(b)) {
                    handlePhysics(a, b);

                    // Create a unique string key: "Alpha:Beta" (sorted alphabetically)
                    std::string nameA = a->name;
                    std::string nameB = b->name;
                    if (nameA > nameB) std::swap(nameA, nameB);
                    std::string collisionKey = nameA + ":" + nameB;

                    currentFrameNames.insert(collisionKey);

                    // If this string wasn't in the set last frame, it's NEW
                    //if (persistentNames.find(collisionKey) == persistentNames.end()) {
                        //if (a->name != b->name) {
                            //std::cout << "[NEW HIT] " << a->name << " + " << b->name << std::endl;
                        //}
                    //}
                }
            }
        }
        // Sync the main.cpp memory with what we found this frame
       // persistentNames = currentFrameNames;
    }
};