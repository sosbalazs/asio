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
      //Message msg = MyQueue.pop_front();
      Message msg = MyQueue.at(0);
      std::cout << msg << std::endl;
      MyQueue.erase(MyQueue.begin());
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

  newMessage.Header.Id = customMsgType;
  newMessage.Header.Size = data.size()+1;
  newMessage.addToBody(data);

  Socket->write_some(asio::buffer(&newMessage.Header, HeaderSize), ec);
  Socket->write_some(asio::buffer(&newMessage.Body, newMessage.Header.Size), ec);
}

void Connector::readFromSocket()
{
    std::cout << "In function: " << __FUNCTION__ << "\n";
    grabHeader();
}

void Connector::grabHeader()
{
  std::cout << "@@@ In function: " << __FUNCTION__ << "\n";
  asio::async_read(*Socket, asio::buffer(&tmpMsg.Header, sizeof(tmpMsg.Header)),
  [&](std::error_code ec, std::size_t length)
  {
    if(!ec)
    {
      std::cout << "\nReading header " << length << " bytes\n";
      std::cout << tmpMsg.Header;
      if(tmpMsg.Header.Size != 0)
      {
        grabBody();
      }
      else
      {
        addToQueue();
      }
    }
    else
    {
      std::cout << "ERROR in read header \n";
      MyClient->finalize();
    }
  });
}

void Connector::grabBody()
{
  std::cout << "@@@ In function: " << __FUNCTION__ << "\n";
  asio::async_read(*Socket, asio::buffer(tmpMsg.Body.data(), tmpMsg.Header.Size),
    [&](std::error_code ec, std::size_t length)
    {
      if(!ec)
      {
        std::cout << "\nReading Body Length: " << length << " bytes\n";
        std::cout << "\nReading Body Size: " << tmpMsg.Header.Size << " bytes\n";

        addToQueue();
      }
      else
      {
        std::cout << "ERROR in read body\n";
        MyClient->finalize();
      }
    });
}

void Connector::addToQueue()
{
  std::cout << "@@@ SOS before bush back, Msg: " << tmpMsg << std::endl;
  MyQueue.push_back(tmpMsg);
  std::cout << "@@@ SOS after bush back, Msg: " << MyQueue.at(MyQueue.size()-1);
  grabHeader();
}


