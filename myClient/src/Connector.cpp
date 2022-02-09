#include "Connector.h"
#include "Client.h"

namespace
{
    int HeaderSize = 8;
}

Connector::Connector(Client* myClient): MyClient(myClient), 
                                        Context()
{
  std::cout << "Creating Connector...\n";
}

void Connector::initializeConnection()
{
  std::cout << "This: " << this << ", Initialize Connection...\nMyClient is: " << MyClient << "\n";
  asio::error_code ec;

  asio::io_context::work idleWork(Context);

  ContextThread = std::thread([&]() { Context.run(); });

  asio::ip::tcp::endpoint endpoint(asio::ip::make_address("127.0.0.1", ec), 60000);
  Socket = std::make_shared<asio::ip::tcp::socket>(Context);
  
  Socket->connect(endpoint, ec);

  if(!ec)
  {
    std::cout << "Connected" << std::endl;
    readFromSocket();
    MyClient->connected();
  }
  else
  {
    std::cout << "Failed to connect to address:\n" << ec.message() << std::endl;
    MyClient->finalize();
  }
}

void Connector::connected()
{
  std::cout << "This: " << this << ", In function: " << __FUNCTION__ << ", ioRun starting\n";
  IoThread = std::thread(&Connector::ioRun, this);
}

void Connector::finalize()
{
  std::cout << "In function: " << __FUNCTION__ << "\n";
  ContextThread.join();
  IsFinalized = true;
  IoThread.join();
}

void Connector::ioRun()
{
  std::cout << "In function: " << __FUNCTION__ << "\n";
  while(!IsFinalized) 
  {
    if(!MyQueue.empty())
    {
      std::cout << "In function: " << __FUNCTION__ << ", Queue is not empty\n";
      Message msg = MyQueue.pop_front();
      std::cout << msg << std::endl;
    }
    // Waiting for messages
    //std::cout << "In function: " << __FUNCTION__ << ", before delay\n";
    using namespace std::literals::chrono_literals;
    std::this_thread::sleep_for(2s);
  }
  std::cout << "In function: " << __FUNCTION__ << ", Loop exited\n";
}

void Connector::sendMessage(CustomMsgTypes customMsgType)
{
  std::cout << "In function: " << __FUNCTION__ << ", Sending message\n";
  asio::error_code ec;
  Message newMessage;
  std::string data = "Buksi";

  newMessage.Header.id = customMsgType;
  newMessage.Header.size = data.size()+1;
  newMessage.addToBody(data);

  Socket->write_some(asio::buffer(&newMessage.Header, HeaderSize), ec);
  Socket->write_some(asio::buffer(&newMessage.Body, newMessage.Header.size), ec);
}

void Connector::readFromSocket()
{
    std::cout << "In function: " << __FUNCTION__ << "\n";
    grabSomeData(Message::MessageParts::Head, HeaderSize);
}

void Connector::grabSomeData(Message::MessageParts messagePart, uint32_t size)
{
  std::cout << "@@@ In function: " << __FUNCTION__ << "\n";
  if(messagePart == Message::MessageParts::Head)
  {
    std::cout << "@@@ Readsome\n";
    asio::async_read(*Socket, asio::buffer(&tmpMsg.Header, sizeof(tmpMsg.Header)),
      [&](std::error_code ec, std::size_t length)
      {
        if(!ec)
        {
          std::cout << "\nReading header " << length << " bytes\n";
          std::cout << tmpMsg.Header;
          if(tmpMsg.Header.size != 0)
          {
            // There is also body, let's grab that
            grabSomeData(Message::MessageParts::Body, tmpMsg.Header.size);
          }
          else
          {
            // There is no body for this message
            std::cout << "\nThere is no body to this message!\n";
            MyQueue.push_back(tmpMsg);
            resetTmpMsg();
            grabSomeData(Message::MessageParts::Head, HeaderSize);
          }
        }
        else
        {
          std::cout << "ERROR In function: " << __FUNCTION__ << "\n";
          MyClient->finalize();
        }
      });
  }
  else
  {
    asio::async_read(*Socket, asio::buffer(&tmpMsg.Body, size),
      [&](std::error_code ec, std::size_t length)
      {
        if(!ec)
        {
          std::cout << "\nReading Body " << length << " bytes\n";

          std::cout << "\nMessage arrived: " << tmpMsg << "\n";

          MyQueue.push_back(tmpMsg);
          resetTmpMsg();
          grabSomeData(Message::MessageParts::Head, HeaderSize);
        }
        else
        {
          std::cout << "ERROR In function: " << __FUNCTION__ << "\n";
          MyClient->finalize();
        }
      });
  }
}

void Connector::resetTmpMsg()
{
    //tmpMsg.Header.id = CustomMsgTypes::ServerAccept;
    //tmpMsg.Header.size = 0;
    tmpMsg.Body.clear();
}


