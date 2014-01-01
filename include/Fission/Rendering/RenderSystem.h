#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include <vector>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Font.hpp>

#include <Fission/Core/System.h>
#include <Fission/Rendering/RenderComponent.h>
#include <Fission/Rendering/RenderManager.h>
#include <Fission/Rendering/TransformComponent.h>

class RenderSystem : public System
{
    public:
        RenderSystem(IEventManager* eventManager, RenderManager* renderManager, float lockStep) :
            System(eventManager, lockStep), mRenderManager(renderManager)
        {
        }

        virtual ~RenderSystem()
        {
        }

    protected:
        template<typename RenderComponentT>
        void initialize()
        {
            mComponentID = RenderComponentT::Type;
            mAspect.all<TransformComponent, RenderComponentT>();
        }

        /// \brief begin function for systems
        void begin(const float dt)
        {
        }

        /// \brief Process entity function for systems
        void processEntity(EntityRef* entity, const float dt)
        {
            auto transCmp = entity->getComponent<TransformComponent>();
            auto rndCmp = static_cast<RenderComponent*>(entity->getComponent(mComponentID));

            sf::RenderStates states;
            states.transform = transCmp->getTransform();

            render(rndCmp, mRenderManager->getWindow(), states);
        }

        /// \brief end function for systems
        void end(const float dt)
        {
        }

        virtual void render(RenderComponent* component, sf::RenderTarget& target, sf::RenderStates& states) = 0;

    private:
        RenderManager* mRenderManager;
        int mComponentID; // Type ID of the render component to process
};

#endif // RENDERSYSTEM_H
