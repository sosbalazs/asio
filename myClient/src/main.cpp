#include "Client.h"


int main()
{
  std::cout << "Initialize Client...\n";
  Client myClient;
  using namespace std::chrono_literals;
  std::this_thread::sleep_for(20000ms);
  return 0;
}
