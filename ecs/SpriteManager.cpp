#include "SpriteManager.hpp"

#include <stdexcept>

namespace rg {
    SpriteManager* SpriteManager::instance{nullptr};

    SpriteManager::SpriteManager() noexcept {
        // fill the map
        entityActionStateToImageRow[Constants::EntityActionState::CLIMB] = 0;
        entityActionStateToImageRow[Constants::EntityActionState::COMBAT_BACKSLASH_ONE_HANDED] = 1;
        entityActionStateToImageRow[Constants::EntityActionState::COMBAT_HALFSLASH_ONE_HANDED] = 5;
        entityActionStateToImageRow[Constants::EntityActionState::COMBAT_IDLE_ONE_HANDED] = 9;
        entityActionStateToImageRow[Constants::EntityActionState::COMBAT_SLASH_ONE_HANDED] = 37;
        entityActionStateToImageRow[Constants::EntityActionState::WALK] = 33;
        entityActionStateToImageRow[Constants::EntityActionState::JUMP] = 21;
        entityActionStateToImageRow[Constants::EntityActionState::SIT] = 29;
        entityActionStateToImageRow[Constants::EntityActionState::RUN] = 25;
        entityActionStateToImageRow[Constants::EntityActionState::IDLE] = 17;
    }

    SpriteManager* SpriteManager::getInstance() noexcept {
        if (instance) {
            return instance;
        }
        instance = new SpriteManager();
        return instance;
    }

    SpriteManager::~SpriteManager() {
        delete instance;
        instance = nullptr;
    }

    unsigned int SpriteManager::getRowOfEntityActionState(const Constants::EntityActionState actionState) {
        if (entityActionStateToImageRow.find(actionState) == entityActionStateToImageRow.end()) {
            throw std::runtime_error("Can't retrieve row for action state!");
        }
        return entityActionStateToImageRow[actionState];
    }

    unsigned int SpriteManager::getColOfDirection(const Constants::SpriteMoveDirection moveDirection) const noexcept {
        switch (moveDirection) {
            case Constants::SpriteMoveDirection::TOP:
                return 0;
            case Constants::SpriteMoveDirection::LEFT:
                return 1;
            case Constants::SpriteMoveDirection::BOTTOM:
                return 2;
            case Constants::SpriteMoveDirection::RIGHT:
                return 3;
        }
        // default will be top (for climb for instance)
        return 0;
    }

    void SpriteManager::insertEntityActionState(const Constants::EntityActionState state,
                                                const unsigned int row) noexcept {
        if (entityActionStateToImageRow.find(state) == entityActionStateToImageRow.end()) {
            entityActionStateToImageRow[state] = row;
        }
    }
} // namespace rg
