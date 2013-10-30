#include "Network/Chat.h"

#include <Core/InputManager.h>
#include <Network/NetworkManager.h>

Chat::Chat(Game *game, int chatPacketID, std::string userName) : GameRef(game), mFont(NULL), mChatting(false), mChatPacketID(chatPacketID), mUserName(userName)
{
    //ctor
}

Chat::~Chat()
{
    // TODO: Destroy font
}

void Chat::initialize()
{
    //if (NetworkManager()->getType() == NetworkType::CLIENT)
        mFont = getGame()->getResourceManager()->getFont("Content/Fonts/NeoGen.ttf");
}

void Chat::update()
{
    //if (NetworkManager()->getType() == NetworkType::CLIENT)
    {
        if (mChatting)
        {
            for (int k = 0; k < sf::Keyboard::KeyCount; k++)
            {
                if (getGame()->getInputManager()->getKeyState(k) == ButtonState::PRESSED)
                {
                    if (k == sf::Keyboard::BackSpace)
                    {
                        if (mMessage.size() > 0)
                            mMessage.erase(mMessage.size()-1);
                    }
                    else if (k == sf::Keyboard::Space)
                        mMessage += ' ';
                    else if (k >= sf::Keyboard::Num0 && k <= sf::Keyboard::Num9)
                    {
                        if (getGame()->getInputManager()->getKeyState(sf::Keyboard::LShift) == ButtonState::DOWN || //caps
                            getGame()->getInputManager()->getKeyState(sf::Keyboard::RShift) == ButtonState::DOWN)
                        {
                            switch (k)
                            {
                                case sf::Keyboard::Num1: mMessage+='!';break;
                                case sf::Keyboard::Num2: mMessage+='@';break;
                                case sf::Keyboard::Num3: mMessage+='#';break;
                                case sf::Keyboard::Num4: mMessage+='$';break;
                                case sf::Keyboard::Num5: mMessage+='%';break;
                                case sf::Keyboard::Num6: mMessage+='^';break;
                                case sf::Keyboard::Num7: mMessage+='&';break;
                                case sf::Keyboard::Num8: mMessage+='*';break;
                                case sf::Keyboard::Num9: mMessage+='(';break;
                                case sf::Keyboard::Num0: mMessage+=')';break;
                            }
                        }
                        else
                            mMessage += (char)(k-sf::Keyboard::Num1+'1');
                    }
                    else if (k >= sf::Keyboard::A && k <= sf::Keyboard::Z)
                    {
                        if (getGame()->getInputManager()->getKeyState(sf::Keyboard::LShift) == ButtonState::DOWN || //caps
                            getGame()->getInputManager()->getKeyState(sf::Keyboard::RShift) == ButtonState::DOWN)
                            mMessage += (char)(k-sf::Keyboard::A+'a');
                        else
                            mMessage += (char)(k-sf::Keyboard::A+'A');
                    }
                    else
                    {
                        if (getGame()->getInputManager()->getKeyState(sf::Keyboard::LShift) == ButtonState::DOWN || //caps
                            getGame()->getInputManager()->getKeyState(sf::Keyboard::RShift) == ButtonState::DOWN)
                        {
                            switch (k)
                            {
                                case sf::Keyboard::Comma: mMessage+='<';break;
                                case sf::Keyboard::Period: mMessage+='>';break;
                                case sf::Keyboard::Slash: mMessage+='?';break; //slash
                                case sf::Keyboard::Subtract: mMessage+='_';break;
                                case sf::Keyboard::SemiColon: mMessage+=':';break; //semicolon
                                case sf::Keyboard::Equal: mMessage+='+';break; //equals
                                case sf::Keyboard::Quote: mMessage+='"';break; //single quote
                                case sf::Keyboard::Tilde: mMessage+='~';break; //tilde
                            }
                        }
                        else
                        {
                            switch (k)
                            {
                                case sf::Keyboard::Comma: mMessage+=',';break;
                                case sf::Keyboard::Period: mMessage+='.';break;
                                case sf::Keyboard::Slash: mMessage+='/';break; //slash
                                case sf::Keyboard::Subtract: mMessage+='-';break;
                                case sf::Keyboard::SemiColon: mMessage+=';';break; //semicolon
                                case sf::Keyboard::Equal: mMessage+='=';break; //equals
                                case sf::Keyboard::Quote: mMessage+='\'';break; //single quote
                                case sf::Keyboard::Tilde: mMessage+='`';break; //tilde
                            }
                        }
                    }
                }
            }

            if (getGame()->getInputManager()->getKeyState(sf::Keyboard::Return) == ButtonState::PRESSED) //user hit enter - send the message!
            {
                mMessage = mUserName+": " + mMessage;
                addMessage(getGame()->getNetworkManager()->getNetworkID(), mMessage);
                mMessage = "";
                mChatting = false;
            }
        }

        //if the user wants to chat or stop chatting
        if (getGame()->getInputManager()->getKeyState(sf::Keyboard::T) == ButtonState::PRESSED && !mChatting)
        {
            mChatting = true;
            mMessage = ""; //clear the message the user was typing
        }

        for (unsigned int m = 0; m < mMessages.size(); m++)
        {
            // Don't keep messages for more than 20 seconds
            if (getGame()->getInputManager()->getTime()-mMessages[m].mTimeStamp > 20000)
            {
                mMessages.erase(mMessages.begin()+m);
                m--;
            }
        }

        // Limit the amount of messages
        while (mMessages.size() > 8)
            mMessages.erase(mMessages.begin()+0);
    }
}

void Chat::draw(sf::RenderTarget *target, sf::RenderStates states)
{
    if (mFont)
    {
        std::string text;

        // Render all of the messages
        for (unsigned int m = 0; m < mMessages.size(); m++)
        {
            if (mFont)
            {
                text += mMessages[m].mMessage;
                text += "\n";
            }
        }

        for (int i = 0; i < 8-(int)mMessages.size(); i++)
            text += "\n";

        if (mChatting)
            text += std::string("SAY: ")+mMessage;

        sf::Text contText(text, *mFont, 16);
        contText.setColor(sf::Color(255,255,255));
        contText.move(sf::Vector2f(5, 5));
        target->draw(contText, states);
    }
}

void Chat::addMessage(int playerID, std::string message)
{
    ChatMessage newMessage;
    newMessage.mMessage = message;
    newMessage.mTimeStamp = getGame()->getInputManager()->getTime();
    mMessages.push_back(newMessage);

    // Send messages
    if (getGame()->getNetworkManager()->getType() == NetworkType::SERVER ||
        (getGame()->getNetworkManager()->getType() == NetworkType::CLIENT && getGame()->getNetworkManager()->getNetworkID() == playerID))
    {
        sf::Packet packet;
        packet << mChatPacketID;
        packet << playerID;
        packet << message;
        getGame()->getNetworkManager()->send(packet);
    }
}
