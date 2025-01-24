#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>

#include "Constants.h"

namespace rg {
    class SpriteManager {
    private:
        std::unordered_map<Constants::EntityActionState, unsigned int> entityActionStateToImageRow;
        static SpriteManager* instance;

    public:
        // filling the entityActionStateToImageRow map
        ~SpriteManager();

        SpriteManager(const SpriteManager&) = delete;

        void operator=(const SpriteManager&) = delete;

        static SpriteManager* getInstance() noexcept;

        unsigned int getRowOfEntityActionState(Constants::EntityActionState actionState);

        unsigned int getColOfDirection(Constants::SpriteMoveDirection moveDirection) const noexcept;

    private:
        SpriteManager() noexcept;

        void insertEntityActionState(Constants::EntityActionState state, unsigned int row) noexcept;
    };
} // namespace rg
