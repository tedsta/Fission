#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include <vector>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Font.hpp>

#include <Fission/Core/System.h>

class DebugDisplay;

class RenderSystem : public System
{
    public:
        RenderSystem(IEventManager* eventManager, float lockStep);
        virtual ~RenderSystem();

    protected:
        /// \brief begin function for systems
        void begin(const float dt);

        /// \brief Process entity function for systems
        void processEntity(EntityRef* entity, const float dt);

        /// \brief end function for systems
        void end(const float dt);

    private:
};

#endif // RENDERSYSTEM_H
