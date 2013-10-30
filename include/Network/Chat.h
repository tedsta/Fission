#ifndef CHAT_H
#define CHAT_H

#include <string>
#include <vector>

#include <Core/ResourceManager.h>
#include <Rendering/RenderingManager.h>

struct ChatMessage
{
    std::string mMessage; // The actual message
    int mTimeStamp; // When the message was sent
};

/// \brief Implements a simple networked chat.
class Chat : public GameRef
{
    public:
        Chat(Game *game, int chatPacketID, std::string userName);
        virtual ~Chat();

        /// \brief Initialize the chat.
        void initialize();

        /// \brief Update the chat.
        void update();

        /// \brief Draw the chat.
        void draw(sf::RenderTarget *target, sf::RenderStates states);

        /// \brief Get whether or not I am currently typing.
        bool getChatting(){return mChatting;}

        /// \brief Add a message to the messages to be displayed.
        void addMessage(int playerID, std::string message);

    private:
        /// \brief The font this chat uses.
        sf::Font *mFont;

        /// If this is a client, the user name to use
        std::string mUserName;

        /// If it's a client, whether I'm talking or not
        bool mChatting;

        /// If it's a client, the message that is currently being typed.
        std::string mMessage;

        /// The packet ID for chat messages
        int mChatPacketID;

        /// All of the chat messages
        std::vector <ChatMessage> mMessages;
};

#endif // CHAT_H
