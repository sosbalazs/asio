#ifndef MESSAGE
#define MESSAGE

#include "StdCommon.h"

enum class CustomMsgTypes : uint32_t
{
  ServerAccept,
  ServerDeny,
  ServerPing,
  MessageAll,
  ServerMessage,
};

struct Message_header
{
  Message_header(){};
  Message_header(CustomMsgTypes id, uint32_t size): Id(id), Size(size) {};
  CustomMsgTypes Id = CustomMsgTypes::ServerAccept;
  uint32_t Size = 0;

  friend std::ostream& operator <<(std::ostream& os, Message_header& msg);
};

struct Message
{
  enum class MessageParts
  {
      Head,
      Body
  };

  friend std::ostream& operator<<(std::ostream& os, MessageParts& messagePart)
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


  Message();
  Message(Message_header header, std::vector<uint8_t> body);

  std::string toString();
  Message& addToBody(std::string newData);
  friend std::ostream& operator <<(std::ostream& os, Message& msg);

  Message_header Header{};
  std::vector<uint8_t> Body;
};

#endif