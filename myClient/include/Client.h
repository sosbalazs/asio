#ifndef CLIENT
#define CLIENT

#include "StdCommon.h"
#include "Connector.h"

class Client
{
    public:
        Client();
        Client(const Client& ) = delete;
        Client(const Client&& ) = delete;
        Client& operator=(const Client& ) = delete;

        ~Client();

        void connected();
        void finalize();
        void waiting();

    private:
        void showUserMenu();

        std::unique_ptr<Connector> Conn;
        bool Finalized = false;
        bool Connected = false;
        std::thread GuiThread;
};

#endif