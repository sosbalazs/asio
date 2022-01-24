#include "StdCommon.h"
#include "Message.h"

enum class MessageParts
{
    Head,
    Body
};

class Connector
{
    public:
        Connector();
        bool connect();

        void ioRun();

        void readFromSocket(asio::ip::tcp::socket& socket);

    private:
        void grabSomeData(asio::ip::tcp::socket& socket, MessageParts messagePart, uint32_t size);
        void resetTmpMsg();
        void errorOnRead(asio::ip::tcp::socket& socket, MessageParts messagePart, std::error_code ec);
        Message tmpMsg;
        std::vector<Message> Queue;


        bool IsFinalized;
        // asio::ip::tcp::socket Socket;
        asio::io_context Context;
        std::thread IoThread;
};