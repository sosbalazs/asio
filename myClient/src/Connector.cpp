#include "Connector.h"
#include "Client.h"

namespace
{
    int HeaderSize = 8;
}

Connector::Connector(Client* myClient) : MyClient(myClient), 
                                        IsFinalized(false), 
                                        Context(),
                                        Socket(Context)
{
  initialiseConnection();
}

void Connector::initialiseConnection()
{
  std::cout << "Initialize Connection...\n";
  asio::error_code ec;

  asio::io_context::work idleWork(Context);

  ContextThread = std::thread([&]() { Context.run(); });

  asio::ip::tcp::endpoint endpoint(asio::ip::make_address("127.0.0.1", ec), 60000);
  Socket.connect(endpoint, ec);

  if(!ec)
  {
    std::cout << "Connected" << std::endl;
    MyClient->connected();
    readFromSocket(Socket);
  }
  else
  {
    std::cout << "Failed to connect to address:\n" << ec.message() << std::endl;
    MyClient->finalize();
  }
}

void Connector::connected()
{
  IoThread = std::thread(&Connector::ioRun, this);
}

void Connector::finalize()
{
  std::cout << __FUNCTION__ << "Finaze called\n";
  ContextThread.join();
  IsFinalized = true;
  IoThread.join();
}

void Connector::ioRun()
{
  while(!IsFinalized) 
  {
    if(!Queue.empty())
    {
      Message msg = Queue.at(0);
      Queue.erase(Queue.begin());
      std::cout << msg << std::endl;
    }
    // Waiting for messages
  }
}

void Connector::sendMessage(CustomMsgTypes customMsgType)
{
  
}

void Connector::readFromSocket(asio::ip::tcp::socket& socket)
{
    grabSomeData(socket, Message::MessageParts::Head, HeaderSize);
}

void Connector::grabSomeData(asio::ip::tcp::socket& socket, Message::MessageParts messagePart, uint32_t size)
{
  if(messagePart == Message::MessageParts::Head)
  {
    socket.async_read_some(asio::buffer(&tmpMsg.Header, size),
      [&](std::error_code ec, std::size_t length)
      {
        if(!ec)
        {
          std::cout << "\nReading header " << length << " bytes\n";
          std::cout << tmpMsg.Header;
          if(tmpMsg.Header.size != 0)
          {
            // There is also body, let's grab that
            grabSomeData(socket, Message::MessageParts::Body, tmpMsg.Header.size);
          }
          else
          {
            // There is no body for this message
            std::cout << "\nThere is no body to this message!\n";
            Queue.push_back(tmpMsg);
            resetTmpMsg();
            grabSomeData(socket, Message::MessageParts::Head, HeaderSize);
          }
        }
        else
        {
          MyClient->finalize();
        }
      });
  }
  else
  {
    socket.async_read_some(asio::buffer(tmpMsg.Body, size),
    [&](std::error_code ec, std::size_t length)
    {
      if(!ec)
      {
        std::cout << "\nReading Body " << length << " bytes\n";

        std::cout << "\nMessage arrived: " << tmpMsg << "\n";
        
        Queue.push_back(tmpMsg);
        resetTmpMsg();
        grabSomeData(socket, Message::MessageParts::Head, HeaderSize);
      }
      else
      {
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


