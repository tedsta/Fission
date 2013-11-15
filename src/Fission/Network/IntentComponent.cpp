#include <Fission/Network/IntentComponent.h>

#include <iostream>

TypeBits IntentComponent::Type;

IntentComponent::IntentComponent()
{
    for (int k = 0; k < sf::Keyboard::KeyCount; k++)
        mKeyStates[k] = BtnState::UP;
    for (int b = 0; b < sf::Mouse::ButtonCount; b++)
        mMouseStates[b] = BtnState::UP;
}

IntentComponent::~IntentComponent()
{
    //dtor
}

void IntentComponent::mapKeyToIntent(const std::string& intent, int key, int state)
{
    Action in;
    in.mInputType = KEYBOARD;
    in.mValue = key;
    in.mState = state;
	mInputMap[in] = intent;
	mIntents[intent] = false;
}

void IntentComponent::mapMouseBtnToIntent(const std::string& intent, int btn, int state)
{
    Action in;
    in.mInputType = MOUSE_BTN;
    in.mValue = btn;
    in.mState = state;
	mInputMap[in] = intent;
	mIntents[intent] = false;
}

bool IntentComponent::isIntentActive(const std::string& intent)
{
    auto it = mIntents.find(intent);
    if (it != mIntents.end())
        return it->second;
    return false;
}
