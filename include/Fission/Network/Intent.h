#ifndef FISSION_INTENT_H
#define FISSION_INTENT_H

#include <string>
#include <map>

#include <SFML/Window/Event.hpp>

#include <Fission/Core/Component.h>
#include <Fission/Core/Event.h>
#include <Fission/Input/Defs.h>

namespace fsn
{
    class Intent : public Component
    {
        FISSION_COMPONENT

        friend class IntentSystem;

        public:
            Intent();
            virtual ~Intent();

            void serialize(sf::Packet &packet);
            void deserialize(sf::Packet &packet);

            /// \brief Map a key to an intent
            void mapKeyToIntent(const std::string& intent, int key, int state);

            /// \brief Map a mouse button to an intent
            void mapMouseBtnToIntent(const std::string& intent, int btn, int state);

            /// \brief Set the net ID.
            void setNetID(int netID){mNetID=netID;}

            /// \brief Get whether or not an intent is active
            bool isIntentActive(const std::string& intent);

            /// \brief Get the position of the mouse cursor
            sf::Vector2f getMousePos(){return mMousePos;}

        private:
            enum
            {
                KEYBOARD,
                MOUSE_BTN,
                MOUSE_MOVE
            };

            struct Action
            {
                Action(int type = 0, int value = 0, int state = 0) : mInputType(type), mValue(value), mState(state) {}

                bool operator<(const Action& other) const
                {
                    if (mValue == other.mValue)
                    {
                        if (mState == other.mState)
                            return mInputType < other.mInputType;

                        return mState < other.mState;
                    }

                    return mValue < other.mValue;
                }

                int mInputType;
                int mValue;
                int mState;
            };

            std::map<Action, std::string> mInputMap; // Maps input to intent names
            std::map<std::string, bool> mIntents; // Map intent names to bools
            int mNetID;

            int mKeyStates[sf::Keyboard::KeyCount]; // Map key codes to intent names
            int mMouseStates[sf::Mouse::ButtonCount];
            sf::Vector2f mMousePos; // Mouse coords
    };
}


#endif // INTENTCOMPONENT_H
