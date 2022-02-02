#ifndef CONNECTOR
#define CONNECTOR

#include "StdCommon.h"
#include "Message.h"

class Client;

class Connector
{
    public:
        Connector(Client* myClient);

        void initialiseConnection();

        void connected();

        void ioRun();

        void readFromSocket(asio::ip::tcp::socket& socket);

        void sendMessage(CustomMsgTypes customMsgType);

        void finalize();

    private:
        void grabSomeData(asio::ip::tcp::socket& socket, Message::MessageParts messagePart, uint32_t size);
        void resetTmpMsg();
        void errorOnRead(asio::ip::tcp::socket& socket, Message::MessageParts messagePart, std::error_code ec);

        Client* MyClient;
        Message tmpMsg;
        std::vector<Message> Queue;


        bool IsFinalized;
        asio::ip::tcp::socket Socket;
        asio::io_context Context;
        std::thread IoThread;
        std::thread ContextThread;
};

#endif