#ifndef MESSAGE
#define MESSAGE

#include <vector>
#include <string>


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
  CustomMsgTypes id{};
  uint32_t size = 0;
};

struct Message
{
  Message();

  std::string toString();
  Message& addToBody(std::string newData);

  Message_header header{};
  std::vector<uint8_t> body;
};

#endif