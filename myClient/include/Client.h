#ifndef CLIENT
#define CLIENT

#include "StdCommon.h"
#include "Connector.h"

class Client
{
    public:
        Client();

        ~Client();

        void showUserMenu();

        void connected();        

        void finalize();

    private:

        std::unique_ptr<Connector> Conn;
        bool Finalized = false;
        bool Connected = false;
        std::thread GuiThread;
};

#endif