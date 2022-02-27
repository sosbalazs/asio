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

  friend std::ostream& operator <<(std::ostream& os, const Message_header& msg);
};

struct Message
{
  Message();
  Message(Message_header header, std::vector<uint8_t> body);
  Message(const Message& msg);
  Message& operator=(const Message& rhs);

  std::string toString();
  Message& addToBody(std::string newData);
  friend std::ostream& operator <<(std::ostream& os, const Message& msg);

  Message_header Header{};
  std::vector<uint8_t> Body;
};

#endif