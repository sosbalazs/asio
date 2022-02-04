#include "Client.h"

Client::Client(): Conn(std::make_unique<Connector>(this))
{
    std::cout << "New client created\n";
}

void Client::showUserMenu()
{
    std::cout << "In function: " << __FUNCTION__ << "\n";
    while(!Finalized)
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

void Client::connected()
{
    std::cout << "In function: " << __FUNCTION__ << ", Client connected\n";
    Connected = true;
    Conn->connected();
    std::cout << "In function: " << __FUNCTION__ << ", Starting Gui thread\n";
    GuiThread = std::thread(&Client::showUserMenu, this);
}

void Client::finalize()
{
    std::cout << "In function: " << __FUNCTION__ << ", Finaze called\n";
    Finalized = true;
    Conn->finalize();
    GuiThread.join();
}
