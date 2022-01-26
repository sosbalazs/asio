#include "StdCommon.h"
#include "Connector.h"

class Client
{
    public:
        Client();

        void showUserMenu();

    private:
        std::unique_ptr<Connector> Conn;
        bool Finalized = false;
};