#ifndef CONNECTOR
#define CONNECTOR

#include "StdCommon.h"
#include "Message.h"
#include "Queue.h"

class Client;

class Connector
{
    public:
        Connector(Client* myClient);

        Connector(const Connector& rhs) = delete;
        Connector& operator=(const Connector& rhs) = delete;

        void initializeConnection();

        void connected();

        void ioRun();

        void readFromSocket();

        void sendMessage(CustomMsgTypes customMsgType);

        void finalize();

        Message tmpMsg;
        std::vector<Message> MyQueue;

    private:
        void grabHeader();
        void grabBody();
        void addToQueue();

        void resetTmpMsg();

        Client* MyClient;

        std::shared_ptr<asio::ip::tcp::socket> Socket;
        asio::io_context Context;
        std::thread IoThread;
        std::thread ContextThread;

        bool IsFinalized = false;

};

#endif