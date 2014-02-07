#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include <vector>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Font.hpp>

#include <Fission/Core/Systems/ComponentSystem.h>
#include <Fission/Rendering/RenderComponent.h>
#include <Fission/Rendering/RenderManager.h>
#include <Fission/Rendering/Transform.h>

namespace fsn
{
    class IRenderSystem : public ComponentSystem
    {
        friend class RenderManager;

        public:
            IRenderSystem(EntityManager& entityMgr) : ComponentSystem(entityMgr)
            {
            }

            virtual void render(const EntityRef& entity, RenderComponent* component, sf::RenderTarget& target, sf::RenderStates& states) = 0;

        private:
    };

    template <typename RenderComponentT>
    class RenderSystem : public IRenderSystem
    {
        friend class RenderManager;

        public:
            RenderSystem(EntityManager& entityMgr, RenderManager* renderManager) :
                IRenderSystem(entityMgr), mRenderManager(renderManager)
            {
                mAspect.all<RenderComponentT>();
                if (mRenderManager->mRenderSystems.size() <= RenderComponentT::Type())
                    mRenderManager->mRenderSystems.resize(RenderComponentT::Type()+1);
                mRenderManager->mRenderSystems[RenderComponentT::Type()] = this;
            }

            /// \brief begin function for systems
            void begin(const float dt)
            {
            }

            /// \brief Process entity function for systems
            void processEntity(const EntityRef& entity, const float dt)
            {
            }

            /// \brief end function for systems
            void end(const float dt)
            {
            }

            void onEntityAdded(const EntityRef& entity)
            {
                auto rndCmp = entity.getComponent<RenderComponentT>();
                mRenderManager->addRenderableToLayer(rndCmp->getLayer(), entity, RenderComponentT::Type());
            }

            void onEntityRemoved(const EntityRef& entity)
            {
                auto rndCmp = entity.getComponent<RenderComponentT>();
                mRenderManager->removeRenderableFromLayer(rndCmp->getLayer(), RenderComponentT::Type());
            }

            // Just call the derived render function
            void render(const EntityRef& entity, RenderComponent* component, sf::RenderTarget& target, sf::RenderStates& states)
            {
                render(entity, static_cast<RenderComponentT*>(component), target, states);
            }

            virtual void render(const EntityRef& entity, RenderComponentT* component, sf::RenderTarget& target, sf::RenderStates& states) = 0;

        private:
            RenderManager* mRenderManager;
    };
}


#endif // RENDERSYSTEM_H
