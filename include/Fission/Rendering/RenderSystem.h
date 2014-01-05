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
#include <Fission/Rendering/Transform.h>

namespace fsn
{
    class IRenderSystem : public System
    {
        friend class RenderManager;

        public:
            IRenderSystem(IEventManager* eventManager, float lockStep) : System(eventManager, lockStep)
            {
            }

        protected:
            virtual void render(RenderComponent* component, sf::RenderTarget& target, sf::RenderStates& states) = 0;

        private:
    };

    template <typename RenderComponentT>
    class RenderSystem : public IRenderSystem
    {
        friend class RenderManager;

        public:
            RenderSystem(IEventManager* eventManager, RenderManager* renderManager, float lockStep) :
                IRenderSystem(eventManager, lockStep), mRenderManager(renderManager)
            {
                mAspect.all<Transform, RenderComponentT>();
                if (mRenderManager->mRenderSystems.size() < RenderComponentT::Type())
                    mRenderManager->mRenderSystems.resize(RenderComponentT::Type()+1);
                mRenderManager->mRenderSystems[RenderComponentT::Type()] = this;
            }

        protected:

            /// \brief begin function for systems
            void begin(const float dt)
            {
            }

            /// \brief Process entity function for systems
            void processEntity(EntityRef* entity, const float dt)
            {
            }

            /// \brief end function for systems
            void end(const float dt)
            {
            }

            void onEntityAdded(EntityRef* entity)
            {
                auto rndCmp = entity->getComponent<RenderComponentT>();
                mRenderManager->addRenderableToLayer(rndCmp->getLayer(), entity, RenderComponentT::Type());
            }

            void onEntityRemoved(EntityRef* entity)
            {
                auto rndCmp = entity->getComponent<RenderComponentT>();
                mRenderManager->removeRenderableFromLayer(rndCmp->getLayer(), RenderComponentT::Type());
            }

            // Just call the derived render function
            void render(RenderComponent* component, sf::RenderTarget& target, sf::RenderStates& states)
            {
                render(static_cast<RenderComponentT*>(component), target, states);
            }

            virtual void render(RenderComponentT* component, sf::RenderTarget& target, sf::RenderStates& states) = 0;

        private:
            RenderManager* mRenderManager;
    };
}


#endif // RENDERSYSTEM_H
