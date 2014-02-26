#include <Fission/Network/Connection.h>

#include <iostream>
#include <Fission/Core/EventManager.h>
#include <Fission/Core/Component.h>
#include <Fission/Network/NetworkEvents.h>

namespace fsn
{
    Connection::Connection(EventManager& eventManager) : mEventManager(eventManager), mNetType(NetType::NONE), mNextID(1)
    {
        enet_initialize();
    }

    Connection::~Connection()
    {
        enet_deinitialize();
    }

    void Connection::hostServer(int port, int incomingBandwidth, int outgoingBandwidth)
    {
        ENetAddress serverAddress;
        serverAddress.host = ENET_HOST_ANY;
        serverAddress.port = port;

        mHost = enet_host_create(&serverAddress, 32, 2, incomingBandwidth, outgoingBandwidth);

        if (!mHost) // All servers have to do is bind the port
        {
            std::cout << "Error starting server.\n";
        }
        else
        {
            std::cout << "Successfully started server.\n";
            mNetType = NetType::SERVER;
        }
    }

    void Connection::connectClient(std::string ipAddress, int port, int incomingBandwidth, int outgoingBandwidth)
    {
        mPeer = new Peer;

        ENetAddress serverAddress;
        enet_address_set_host(&serverAddress, ipAddress.c_str());
        serverAddress.port = port;

        // Create the enet host
        //mHost = enet_host_create(NULL, 1, 2, 57600 / 8, 14400 / 8);
        mHost = enet_host_create(NULL, 1, 2, incomingBandwidth, outgoingBandwidth);
        mPeer->mPeer = enet_host_connect(mHost, &serverAddress, 2, 0);

        // Send the connection request packet
        if (!mHost || !mPeer)
        {
            std::cout << "Failed to connect to " << ipAddress << std::endl;
            return;
        }

        // Wait up to 5 seconds for the connection attempt to succeed.
        ENetEvent event;
        if (enet_host_service(mHost, &event, 5000) > 0 &&
            event.type == ENET_EVENT_TYPE_CONNECT)
        {
            // Successfully opened connection
        }
        else
        {
            enet_peer_reset(mPeer->mPeer);
            std::cout << "Connection to " << ipAddress << " failed.\n";
            return;
        }

        // Wait five seconds for network ID
        if (enet_host_service(mHost, &event, 10000) > 0 &&
            event.type == ENET_EVENT_TYPE_RECEIVE)
        {
            Packet packet;
            packet.append(event.packet->data+1, event.packet->dataLength-1); // offset of 1 is for handler ID tagged onto packets
            packet >> mPeer->mID;
            std::cout << "Connection to " << ipAddress << " with ID " << mPeer->mID <<  " succeeded.\n";
        }
        else
        {
            enet_peer_reset(mPeer->mPeer);
            std::cout << "Connection to " << ipAddress << " failed.\n";
            return;
        }

        // We are now connected
        mNetType = NetType::CLIENT;
    }

    void Connection::disconnect(int netID)
    {
        if (mNetType == NetType::CLIENT)
        {
            enet_peer_disconnect(mPeer->mPeer, 0);
        }
        else if (mNetType == NetType::SERVER)
        {
            if (netID > 0)
                enet_peer_disconnect(findPeer(netID)->mPeer, 0);
        }
    }

    void Connection::update()
    {
        if (mNetType == NetType::NONE)
            return;

        ENetEvent event;
        char ipcstr[100];
        std::string IP;

        // Check for events, but don't wait
        for (int netIteration = 0; netIteration < 100; netIteration++)
        {
            if (!(enet_host_service(mHost, &event, 1) > 0))
                break;

            enet_address_get_host_ip(&event.peer->address, ipcstr, 100);
            IP = ipcstr;

            switch (event.type)
            {
                case ENET_EVENT_TYPE_CONNECT:
                {
                    std::cout << "New peer " << mNextID << " from " << IP << ":" << event.peer->address.port << std::endl;

                    // Add the new peer
                    Peer *peer = new Peer;
                    peer->mID = mNextID;
                    peer->mIPAddress = IP;
                    peer->mPeer = event.peer;
                    mPeers.push_back(peer);
                    mNextID++;

                    event.peer->data = peer;

                    // Send the client its ID
                    Packet idPacket;
                    idPacket << peer->mID;
                    send(idPacket, peer->mID);
                    enet_host_flush(mHost);

                    // TODO: Fire client connected event.

                    break;
                }

                case ENET_EVENT_TYPE_RECEIVE:
                {
                    Peer *peer = reinterpret_cast<Peer*>(event.peer->data);

                    Packet packet;
                    packet.append(event.packet->data+0, event.packet->dataLength-0);

                    sf::Int8 hndID;
                    packet >> hndID;
                    if (hndID >= 0 && hndID < int(mHandlers.size()))
                    {
                        if (mNetType == NetType::SERVER)
                        {
                            mHandlers[hndID]->handlePacket(packet, peer->mID);
                        }
                        else
                        {
                            mHandlers[hndID]->handlePacket(packet, 0);
                        }
                    }

                    enet_packet_destroy(event.packet);

                    break;
                }

                case ENET_EVENT_TYPE_DISCONNECT:
                {
                    if (mNetType == NetType::CLIENT)
                    {
                        std::cout << "Disconnected from server.\n";
                        mNetType = NetType::NONE;
                    }
                    else if (mNetType == NetType::SERVER)
                    {
                        Peer *peer = reinterpret_cast<Peer*>(event.peer->data);

                        if(peer)
                        {
                            // TODO: Fire client disconnected event.

                            std::cout << "Peer " << peer->mID << " has disconnected.\n";
                            peer->mPeer = NULL;
                            removePeer(peer->mID);
                            event.peer->data = NULL;
                        }
                    }

                    break;
                }

                default:
                {
                    break;
                }
            }
        }
    }

    void Connection::send(Packet& packet, int hndID, int peerID, int excludeID, bool reliable)
    {
        if (mNetType == NetType::NONE)
            return;

        Packet finalPacket;
        finalPacket << sf::Int8(hndID);
        finalPacket << packet; // Append other packet onto final packet

        // Create the enet packet
        unsigned int flags = ENET_PACKET_FLAG_NO_ALLOCATE;
        if (reliable)
            flags |= ENET_PACKET_FLAG_RELIABLE;

        ENetPacket *enetPacket = enet_packet_create(finalPacket.getData(), finalPacket.getDataSize(), flags);

        if (mNetType == NetType::CLIENT) // Clients send data to server only
        {
            enet_host_broadcast(mHost, 0, enetPacket);
        }
        else if (peerID > 0) // It's a server and the client is specified. Tell only that client!
        {
            enet_peer_send(findPeer(peerID)->mPeer, 0, enetPacket);
        }
        else // It's a server and the client is unspecified. Broadcast to everyone
        {
            if (excludeID > 0)
            {
                for (unsigned int i = 0; i < mPeers.size(); i++)
                {
                    if (mPeers[i]->mID != excludeID && mPeers[i]->mPeer)
                        enet_peer_send(mPeers[i]->mPeer, 0, enetPacket);
                }
            }
            else
                enet_host_broadcast(mHost, 0, enetPacket);
        }
    }

    int Connection::findPeerID(std::string IP)
    {
        for (unsigned int i = 0; i < mPeers.size(); i++)
        {
            if (mPeers[i]->mIPAddress == IP)
                return mPeers[i]->mID;
        }

        return -1;
    }

    Peer *Connection::findPeer(int netID)
    {
        for (unsigned int i = 0; i < mPeers.size(); i++)
        {
            if (mPeers[i]->mID == netID)
                return mPeers[i];
        }

        return NULL; // Return invalid peer
    }

    void Connection::removePeer(int netID)
    {
        if (mNetType == NetType::NONE)
            return;

        for (unsigned int i = 0; i < mPeers.size(); i++)
        {
            if (mPeers[i]->mID == netID)
            {
                delete mPeers[i];
                mPeers.erase(mPeers.begin()+i);
                return;
            }
        }
    }

    int Connection::registerHandlerAuto(IPacketHandler* handler)
    {
        handler->mHndID = mHandlers.size();
        mHandlers.push_back(handler);
        return mHandlers.size()-1;
    }

    void Connection::registerHandler(int hndID, IPacketHandler* handler)
    {
        if (hndID >= static_cast<int>(mHandlers.size()))
            mHandlers.resize(hndID+1);

        handler->mHndID = hndID;
        mHandlers[hndID] = handler;
    }
}
