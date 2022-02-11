#include "StdCommon.h"

class Message;

class Queue
{
    public:
        void push_back(Message msg);
        Message pop_front();
        bool empty() const;

    private:
        std::vector<Message> Datas;
        //std::mutex Locker;
};