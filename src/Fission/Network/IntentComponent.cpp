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

void IntentComponent::serialize(sf::Packet &packet)
{
    packet << static_cast<int>(mInputMap.size());
    for (auto input : mInputMap)
    {
        packet << input.first.mInputType << input.first.mValue << input.first.mState;
        packet << input.second;
    }

    packet << static_cast<int>(mIntents.size());
    for (auto intent : mIntents)
    {
        packet << intent.first << intent.second;
    }

    packet << mNetID;
    packet << mMousePos.x << mMousePos.y;
}

void IntentComponent::deserialize(sf::Packet &packet)
{
    int inputCount;
    packet >> inputCount;
    for (int i = 0; i < inputCount; i++)
    {
        Action action;
        std::string str;
        packet >> action.mInputType >> action.mValue >> action.mState;
        packet >> str;
        mInputMap[action] = str;
    }

    int intentCount;
    packet >> intentCount;
    for (int i = 0; i < intentCount; i++)
    {
        std::string str;
        bool b;
        packet >> str >> b;
        mIntents[str] = b;
    }

    packet >> mNetID;
    packet >> mMousePos.x >> mMousePos.y;
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
