#include "Connector.h"

namespace
{
    int HeaderSize = 8;

    std::ostream& operator<<(std::ostream& os, MessageParts& messagePart)
    {
      switch(messagePart)
      {
        case MessageParts::Body:
          os << "Body";
          break;
        case MessageParts::Head:
          os << "Head";
          break;    
      }
      return os;
    }

}

Connector::Connector() : IsFinalized(false), 
                        Context()
{
  IoThread = std::thread(Connector::ioRun, this);
}

void Connector::ioRun()
{
  //while(!IsFinalized) {
    asio::error_code ec;
    Context.run();

    asio::ip::tcp::socket socket(Context);
    asio::ip::tcp::endpoint endpoint(asio::ip::make_address("127.0.0.1", ec), 60000);
    socket.connect(endpoint, ec);
    
    if(!ec)
    {
      std::cout << "Connected" << std::endl;
    }
    else
    {
      std::cout << "Failed to connect to address:\n" << ec.message() << std::endl;
    }
  //}
}

void sendMessage(CustomMsgTypes customMsgType)
{
  
}

void Connector::readFromSocket(asio::ip::tcp::socket& socket)
{
    grabSomeData(socket, MessageParts::Head, HeaderSize);
}

void Connector::grabSomeData(asio::ip::tcp::socket& socket, MessageParts messagePart, uint32_t size)
{
  if(messagePart == MessageParts::Head)
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
            resetTmpMsg();
            // There is also body, let's grab that
            grabSomeData(socket, MessageParts::Body, tmpMsg.Header.size);
          }
          else
          {
            // There is no body for this message
            std::cout << "\nThere is no body to this message!\n";
            grabSomeData(socket, MessageParts::Head, HeaderSize);
          }
        }
        else
        {
          errorOnRead(socket, MessageParts::Head, ec);
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

        grabSomeData(socket, MessageParts::Head, HeaderSize);
      }
      else
      {
        errorOnRead(socket, MessageParts::Body, ec);
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

void Connector::errorOnRead(asio::ip::tcp::socket& socket, MessageParts messagePart, std::error_code ec)
{
    std::cout << "\nERROR\nError in: " << messagePart << ", error no: " << ec << std::endl;
    resetTmpMsg();
    /*if(socket.is_open())
    {
        grabSomeData(socket, MessageParts::Head, HeaderSize);
    }
    else
    {
        std::cout << "Socket closed, exiting...\n";
    }*/
}

