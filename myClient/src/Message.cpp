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
  header.size = sizeof(header);
}

Message& Message::addToBody(std::string newData)
{
  for(const auto& character:newData)
  {
    body.push_back(character);
    header.size += sizeof(character);
  }
  return *this;
}

std::string Message::toString()
{
  std::stringstream ss;
  ss << header.id << header.size;
  for(const auto& element:body)
  {
    ss << element;
  }
  return ss.str();
}