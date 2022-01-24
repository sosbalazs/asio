#include "StdCommon.h"
#include "Connector.h"

class Client
{
    public:
        Client();

    private:
        std::unique_ptr<Connector> Conn;
};