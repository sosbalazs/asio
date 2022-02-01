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
      os << 2;
      break;
    case CustomMsgTypes::MessageAll:
      os << 3;
      break;
    case CustomMsgTypes::ServerMessage:
    case CustomMsgTypes::ServerAccept:
    case CustomMsgTypes::ServerDeny:
      os << 0;
  }
  return os;
}

Message::Message()
{
  Header.size = 0;
}

Message::Message(Message_header& header, std::vector<uint8_t> body):Header(header), Body(body)
{
  
}

Message& Message::addToBody(std::string newData)
{
  for(auto& character:newData)
  {
    Body.push_back(character);
  }
  Header.size=Body.size()+1;

  return *this;
}

std::ostream& operator <<(std::ostream& os, Message_header& header)
{
  os << "Msg id: " << header.id << ", data size: " << header.size;
  return os;
}

std::ostream& operator<<(std::ostream& os, Message& msg)
{
  os << msg.Header << "\nData: ";
  for (const auto& character:msg.Body) 
  {
    os << character;
  }
  os << std::endl;
  return os;
}

