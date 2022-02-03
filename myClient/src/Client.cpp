#include "Client.h"

Client::Client(): Conn(this)
{
    std::cout << "New client created\n";
}

void Client::showUserMenu()
{
    while(!Finalized)
    {
        std::cout << "Enter Input:\n1) Ping\n2) Message All\n";
        uint8_t result;
        std::cin >> result;
        switch(result)
        {
            case 1:
                Conn.sendMessage(CustomMsgTypes::ServerPing);
                break;
            case 2:
                Conn.sendMessage(CustomMsgTypes::MessageAll);
                break;
            default:
                Finalized = true;
        }
    }
}

void Client::connected()
{
    std::cout << __FUNCTION__ << "Client connected\n";
    Connected = true;
    Conn.connected();
    GuiThread = std::thread(&Client::showUserMenu, this);
}

void Client::finalize()
{
    std::cout << __FUNCTION__ << "Finaze called\n";
    Finalized = true;
    Conn.finalize();
    GuiThread.join();
}

void Client::print()
{
    std::cout << "print\n";
}
