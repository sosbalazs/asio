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
        Connector() = default;
        void readFromSocket(asio::ip::tcp::socket& socket);

    private:
        void grabSomeData(asio::ip::tcp::socket& socket, MessageParts messagePart, uint32_t size);
        void resetTmpMsg();
        void errorOnRead(asio::ip::tcp::socket& socket, MessageParts messagePart, std::error_code ec);
        Message tmpMsg;
        std::vector<Message> Queue;
};