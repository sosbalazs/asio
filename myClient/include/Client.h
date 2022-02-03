#ifndef CLIENT
#define CLIENT

#include "StdCommon.h"
#include "Connector.h"

class Client
{
    public:
        Client();

        void showUserMenu();

        void connected();        

        void finalize();

        void print();

    private:

        Connector Conn;
        bool Finalized = false;
        bool Connected = false;
        std::thread GuiThread;
};

#endif