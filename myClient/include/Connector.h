#include "StdCommon.h"
#include "Message.h"

class Client;

enum class MessageParts
{
    Head,
    Body
};

class Connector
{
    public:
        Connector(Client& myClient);

        bool connect();

        void ioRun();

        void readFromSocket(asio::ip::tcp::socket& socket);

        void sendMessage(CustomMsgTypes customMsgType);

        void finalize();

    private:
        void grabSomeData(asio::ip::tcp::socket& socket, MessageParts messagePart, uint32_t size);
        void resetTmpMsg();
        void errorOnRead(asio::ip::tcp::socket& socket, MessageParts messagePart, std::error_code ec);

        Client& MyClient;
        Message tmpMsg;
        std::vector<Message> Queue;


        bool IsFinalized;
        // asio::ip::tcp::socket Socket;
        asio::io_context Context;
        std::thread IoThread;
};