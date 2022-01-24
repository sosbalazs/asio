#include "Client.h"

Client::Client : Conn(std::make_unique<Connector>())
{

}