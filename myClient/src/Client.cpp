#include "Client.h"

Client::Client : Conn(std::make_unique<Connector>())
{

}

void Client::showUserMenu()
{
    while(Finalized)
    {
        std::cout << "Enter Input:\n1) Ping\n2) Message All\n";
        uint8_t result;
        std::cin >> result;
        switch(result)
        {
            case 1:
                Conn->sendMessage(CustomMsgTypes::ServerPing);
                break;
            case 2:
                Conn->sendMessage(CustomMsgTypes::MessageAll);
                break;
            default:
                Finalized = true;
        }
    }
}