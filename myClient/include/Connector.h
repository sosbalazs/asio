#ifndef CONNECTOR
#define CONNECTOR

#include "StdCommon.h"
#include "Message.h"

class Client;

class Connector
{
    public:
        Connector(Client* myClient);

        Connector& operator=(const Connector& rhs);

        void initializeConnection();

        void connected();

        void ioRun();

        void readFromSocket();

        void sendMessage(CustomMsgTypes customMsgType);

        void finalize();

    private:
        void grabSomeData(Message::MessageParts messagePart, uint32_t size);
        void resetTmpMsg();
        void errorOnRead(Message::MessageParts messagePart, std::error_code ec);

        Client* MyClient;

        std::shared_ptr<asio::ip::tcp::socket> Socket;
        asio::io_context Context;
        std::thread IoThread;
        std::thread ContextThread;
};

#endif