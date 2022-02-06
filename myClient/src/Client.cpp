#include "Client.h"

Client::Client(): Conn(std::make_unique<Connector>(this))
{
    std::cout << "Client this:" << this << ", New client created\n";
    Conn->initializeConnection();
}

Client::~Client()
{
    std::cout << "Client this:" << this << " Deleting Client\n";
}

void Client::showUserMenu()
{
    std::cout << "In function: " << __FUNCTION__ << "\n";
    while(!Finalized)
    {
        std::cout << "Enter Input:\n1) Ping\n2) Message All\n";
        std::string result;
        std::cin >> result;
        uint8_t res = std::stoi(result);
        std::cout << "\n Entered: " << result << "\n";
        switch(res)
        {
            case 1:
                std::cout << "In function: " << __FUNCTION__ << ", Case Ping\n";
                Conn->sendMessage(CustomMsgTypes::ServerPing);
                break;
            case 2:
                std::cout << "In function: " << __FUNCTION__ << ", Case Message all\n";
                Conn->sendMessage(CustomMsgTypes::MessageAll);
                break;
            default:
                std::cout << "In function: " << __FUNCTION__ << ", Default, exiting\n";
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
}

void Client::waiting()
{
    std::cout << "In function: " << __FUNCTION__ << ", waiting GUI thread to end\n";
    GuiThread.join();
    std::cout << "In function: " << __FUNCTION__ << ", GUI thread ended\n";
}