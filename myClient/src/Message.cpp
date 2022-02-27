#include "Message.h"

std::ostream& operator<<(std::ostream& os, const CustomMsgTypes& type)
{
  switch(type)
  {
    /*ServerAccept,
    ServerDeny,
    ServerPing,
    MessageAll,
    ServerMessage,
    */
    case CustomMsgTypes::ServerPing:
      os << "ServerPing";
      break;
    case CustomMsgTypes::MessageAll:
      os << "MessageAll";
      break;
    case CustomMsgTypes::ServerMessage:
      break;
    case CustomMsgTypes::ServerAccept:
      os << "ServerAccept\n";
      break;
    case CustomMsgTypes::ServerDeny:
      os << 0;
  }
  return os;
}

Message::Message()
{
  Header.Size = 0;
}

Message::Message(Message_header header, std::vector<uint8_t> body):Header(header), Body(body)
{
  
}

std::ostream& operator <<(std::ostream& os, const Message_header& header)
{
  os << "Msg id: " << header.Id << ", data size: " << header.Size;
  return os;
}

std::ostream& operator<<(std::ostream& os, const Message& msg)
{
  os << msg.Header << "\n";
  if(msg.Header.Size != 0)
  {
    os << "Data: ";
    for (int i = 0; i < msg.Header.Size; ++i)
    {
      os << msg.Body[i];
    }
    os << std::endl;
  }
  return os;
}


Message::Message(const Message& msg)
{
  std::cout << "@@@ SOS Copy constructor of msg INPUT " << msg << "\n";
  
  this->Header.Id = msg.Header.Id;
  this->Header.Size = msg.Header.Size;
  for(size_t i = 0; i < msg.Header.Size; ++i)
  {
    std::cout << "### SOS Copy constructor of msg IN LOOP " << msg << "\n";
    this->Body.push_back(msg.Body.at(i));
  }
  std::cout << "@@@ SOS Copy constructor of msg OUTPUT " << *this << "\n";
}

Message& Message::operator=(const Message& rhs)
{
  std::cout << "@@@ SOS Message assignment operator " << rhs << std::endl;
  if(this == &rhs)
  {
    return *this;
  }

  Header = rhs.Header;
  Body = rhs.Body;
  return *this;
}

Message& Message::addToBody(std::string newData)
{
  for(auto& character:newData)
  {
    Body.push_back(character);
  }
  Header.Size=Body.size()+1;

  return *this;
}
