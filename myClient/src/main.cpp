
#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif

#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

#include <iostream>
#include <chrono>

#include "Message.h"

std::vector<char> vBuffer(20 * 1024);

void GrabSomeData(asio::ip::tcp::socket& socket)
{
  socket.async_read_some(asio::buffer(vBuffer.data(), vBuffer.size()),
    [&](std::error_code ec, std::size_t length)
    {
      if(!ec)
      {
        std::cout << "\n\nRead " << length << " bytes\n\n";

        for (int i = 0; i < length; ++i)
        {
          std::cout << vBuffer[i];

          GrabSomeData(socket);
        }
      }
    }
  );
}

Message createNewMessage(std::string type_s)
{
  int type = stoi(type_s);
  Message newMessage;
  switch(type)
  {
    case 1:
      newMessage.header.id = CustomMsgTypes::ServerPing;
      newMessage.addToBody("Buksi");
      break;
    case 2:
      newMessage.header.id = CustomMsgTypes::MessageAll;
      newMessage.addToBody("Buksi");
      break;
    default:
      newMessage.header.id = CustomMsgTypes::MessageAll;
      newMessage.addToBody("Buksi");
  }
  return newMessage;
}

int main()
{
  asio::error_code ec;

  // Create a "context" - essentially the platform specific interface
  asio::io_context context;

  // Give some fake tasks to asio so the context does not finish
  asio::io_context::work idleWork(context);

  // Start the context
  std::thread thrContext = std::thread([&]() { context.run(); });

  // asio::ip::tcp::endpoint endpoint(asio::ip::make_address("93.184.216.34", ec), 80);
  asio::ip::tcp::endpoint endpoint(asio::ip::make_address("127.0.0.1", ec), 60000);
  //asio::ip::tcp::endpoint endpoint(asio::ip::make_address("51.38.81.49", ec), 80);

  asio::ip::tcp::socket socket(context);

  socket.connect(endpoint, ec);

  if(!ec)
  {
    std::cout << "Connected" << std::endl;
  }
  else
  {
    std::cout << "Failed to connect to address:\n" << ec.message() << std::endl;
  }

  if (socket.is_open())
  {
    GrabSomeData(socket);

    bool exitLoop = false;
    while(!exitLoop)
    {
      std::string result = "";
      Message newMessage;
      std::cout << "Waiting for input\n";
      std::cin >> result;

      if(result == "1" || result == "2" || result == "3")
      {
        newMessage = createNewMessage(result);

        std::cout << "New message has been created! " << newMessage.toString() << "\n";
      }
      else
      {
        exitLoop = true;
      }

      std::string sRequest = newMessage.toString();

      socket.write_some(asio::buffer(sRequest.data(), sRequest.size()), ec);
    }

    context.stop();
    if(thrContext.joinable())
    {
      thrContext.join();
    }
  }


  return 0;
}
