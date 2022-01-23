#include "StdCommon.h"


#include "Message.h"
#include "Connector.h"

Message createNewMessage(std::string type_s)
{
  int type = stoi(type_s);
  Message newMessage;
  switch(type)
  {
    case 1:
      newMessage.Header.id = CustomMsgTypes::ServerPing;
      newMessage.addToBody("Buksi");
      break;
    case 2:
      newMessage.Header.id = CustomMsgTypes::MessageAll;
      newMessage.addToBody("Buksi");
      break;
    default:
      newMessage.Header.id = CustomMsgTypes::MessageAll;
      //newMessage.addToBody("Buksi");
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
    Connector conn;
    conn.readFromSocket(socket);

    bool exitLoop = false;

    Message newMessage;
    while(!exitLoop)
    {
      std::string result = "";
      std::cout << "Waiting for input\n";
      std::cin >> result;

      if(result == "1" || result == "2" || result == "3")
      {
        newMessage = createNewMessage(result);

        std::cout << "New message has been created!\n" << newMessage << "\n";
      }
      else
      {
        exitLoop = true;
      }

      socket.write_some(asio::buffer(&newMessage.Header, 8), ec);
      socket.write_some(asio::buffer(&newMessage.Body, newMessage.Body.size()+1), ec);
    }

    context.stop();
    if(thrContext.joinable())
    {
      thrContext.join();
    }
  }


  return 0;
}
