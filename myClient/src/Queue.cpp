#include "Queue.h"
#include "Message.h"

void Queue::push_back(Message msg)
{
    //std::scoped_lock lock(Locker);
    Datas.push_back(msg);
}

Message Queue::pop_front()
{
    //std::scoped_lock lock(Locker);
    Message msg = Datas.at(0);
    Datas.erase(Datas.begin());
    return msg;
}

bool Queue::empty() const
{
    //std::scoped_lock lock(Locker);
    return Datas.empty();
}