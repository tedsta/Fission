#ifndef COMPONENTTYPEMANAGER_H
#define COMPONENTTYPEMANAGER_H

#include <vector>
#include <bitset>

#include "Fission/Core/config.h"
#include "Fission/Core/Component.h"

namespace fsn
{
    class ComponentTypeManager
    {
        public:
            /// \brief Register a component type.
            template<typename c>
            static void add()
            {
                // Set the component's type ID
                c::Type() = mNextID;
                mNextID++;

                // Create the component bit
                std::bitset<MaxComponents> bit;
                bit.set(c::Type());
                mBits.push_back(bit);

                // Create the factory function
                mFactories.push_back(&Component::factory<c>);
            }

            /// \brief Create a new component by it's type ID
            static Component* createComponent(ComponentType type);

            /// \brief Gets the bit set of a component.
            template<typename c>
            static std::bitset<MaxComponents> getBit()
            {
                return mBits[c::Type()];
            }

            /// \brief Gets the bit set of a component.
            static std::bitset<MaxComponents> getBit(int componentID)
            {
                return mBits[componentID];
            }

        private:
            static std::vector<std::bitset<MaxComponents>> mBits;
            static std::vector<ComponentFactory> mFactories;
            static int mNextID;
    };
}


#endif // COMPONENTTYPEMANAGER_H
