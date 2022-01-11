#include <iostream>
#include <sstream>
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
  header.size = 0;
}

Message& Message::addToBody(std::string newData)
{
  memcpy(&newData, &body, newData.size());
  header.size += body.size();

  return *this;
}

std::string Message::toString()
{
  std::stringstream ss;
  ss << header.id << header.size;
  for(const auto& element:body)
  {
    ss << static_cast<char>(element);
  }
  return ss.str();
}