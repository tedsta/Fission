#ifndef ASPECT_H
#define ASPECT_H

#include <bitset>

#include "Fission/Core/config.h"
#include "Fission/Core/ComponentTypeManager.h"

namespace fsn
{
    class Component;
    class EntityRef;

    /// \brief An Aspect defines rules based on the components an entity possesses making it either
    /// compatible or incompatible with that entity. Used by systems to determine which entities should
    /// be processed.
    class Aspect
    {
        public:
            Aspect();
            virtual ~Aspect();

            /// \brief Check if an entity is compatible with this aspect.
            template <typename EntityRefT = EntityRef> // This is a template so the unit test can mock EntityRef.
            bool checkEntity(const EntityRefT& entity)
            {
                std::bitset<MaxComponents> entityBits = entity.getBits();
                if ((entityBits&mAll) == mAll && (mOne.none() || (entityBits&mOne).any()) && (entityBits&mExclude).none())
                    return true;

                return false;
            }

            /// \brief Require all of these components.
            template<typename ...components>
            void all() {
                _all(typelist<components...>());
            }

            /// \brief Require at least one of these components.
            template<typename ...components>
            void one() {
                _one(typelist<components...>());
            }

            /// \brief Require that none of these components are present.
            template<typename ...components>
            void exclude() {
                _exclude(typelist<components...>());
            }

        private:
            // Note: The bool template specialization of std::vector optimizes for space - each value
            // is stored as only a single bit.

            std::bitset<MaxComponents> mAll;         // Entity must contain all of these components.
            std::bitset<MaxComponents> mOne;         // Entity must contain at least one of these components.
            std::bitset<MaxComponents> mExclude;     // Entity must contain none of these components

            // Meta template. Helps with passing each type from the variadic template
            template<typename...>
            struct typelist {};

            // Recursively called. On each iteration  "component" is set to the next "typename" from rest pack set. .
            // We pass this component into our component manager.
            template<typename component, typename ... Rest>
            void _all(typelist<component,Rest...>)
            {
                // Add bits to all
                mAll |= ComponentTypeManager::getBit<component>();
                _all(typelist<Rest...>());
            };
            void _all(typelist<>) { }; // To end the recursion

            template<typename component, typename ... Rest>
            void _one(typelist<component,Rest...>)
            {
                // Add Bits to one
                mOne |= ComponentTypeManager::getBit<component>();
                _one(typelist<Rest...>());
            };
            void _one(typelist<>) { };

            template<typename component, typename ... Rest>
            void _exclude(typelist<component,Rest...>)
            {
                // Add bits to exclude
                mExclude |= ComponentTypeManager::getBit<component>();
                _exclude(typelist<Rest...>());
            };
            void _exclude(typelist<>) { };
    };
}


#endif // ASPECT_H
