#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <enet/enet.h>

#include "Fission/Core/Packet.h"

namespace fsn
{
    namespace NetType
    {
        enum
        {
            NONE,
            SERVER,
            CLIENT
        };
    };

    struct Peer
    {
        int mID;
        std::string mIPAddress;
        ENetPeer *mPeer;
    };

    class Entity;
    class Component;
    class EventManager;
    class Scene;

    class IPacketHandler
    {
        friend class Connection;

        public:
            virtual void handlePacket(Packet& packet, int netID) = 0;

            /// \brief Get the packet handler ID
            int getHndID() const {return mHndID;}

        private:
            int mHndID;
    };

    class Connection
    {
        public:
            Connection(EventManager& eventManager);
            virtual ~Connection();

            /// \brief Begins hosting a server.
            /// \param port The port to host on.
            /// \param incomingBandwidth The cap on incoming bandwidth. Leave at 0 for unlimited.
            /// \param outgoingBandwidth The cap on outgoing bandwidth. Leave at 0 for unlimited.
            void hostServer(int port, int incomingBandwidth = 0, int outgoingBandwidth = 0);

            /// \brief Connects to a server.
            /// \param ipAddress The address of the server to connect to.
            /// \param port The port the server is hosting on.
            /// \param incomingBandwidth The cap on incoming bandwidth. Leave at 0 for unlimited.
            /// \param outgoingBandwidth The cap on outgoing bandwidth. Leave at 0 for unlimited.
            void connectClient(std::string ipAddress, int port, int incomingBandwidth = 57600 / 8, int outgoingBandwidth = 14400 / 8);

            /// \brief If it's a server, disconnect a client. If it's a client, disconnect from the
            /// server.
            /// \note netID is only relevant to servers.
            void disconnect(int netID = 0);

            /// \brief Update the networking manager.
            void update();

            /// \brief Send a packet over the network.
            /// \param packet The packet to send.
            /// \param netID If it's a server, the ID of the peer to send the packet to. Set
            /// to 0 to broadcast to all clients.
            /// \param excludeID If it's a server and the packet is sent to all clients, the ID of the
            /// peer to exclude when sending. Set to 0 to exclude no one.
            /// \param reliable Whether or not to send the packet reliably.
            void send(Packet& packet, int hndID, int netID = 0, int excludeID = 0, bool reliable = true);

            /// \brief Find the ID of a peer with the specified IP address.
            int findPeerID(std::string IP);

            /// \brief Find a peer by ID.
            Peer* findPeer(int netID);

            /// \brief Remove a peer
            void removePeer(int netID);

            /// \brief Automatically register the handler to the  next unused ID and return the ID
            int registerHandlerAuto(IPacketHandler* handler);

            /// \brief Register a handler to a specific handler ID
            void registerHandler(int hndID, IPacketHandler* handler);

            /// \brief Get the network role - server or client.
            int getType() const {return mNetType;}

            /// \brief If this is a client, get the network ID.
            int getNetID() const {if (mPeer){return mPeer->mID;} return 0;}

        private:
            void sendRaw(Packet& packet, int netID = 0, int excludeID = 0, bool reliable = true);

            // Some dependencies
            EventManager& mEventManager;

            // Server or client?
            int mNetType;

            // The network host
            ENetHost *mHost;

            // If it's a client, the peer
            Peer *mPeer;

            // The list of clients if I'm a server
            std::vector <Peer*> mPeers;

            // The ID for the next peer
            int mNextID;

            // The packet handlers
            std::vector<IPacketHandler*> mHandlers;
    };
}


#endif // NETWORKMANAGER_H
