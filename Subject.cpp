#include "Subject.h"
#include "Observer.h"

void Subject::notify()
{
    for(auto& i : observers)
    {
        i->update();
    }
}


void Subject::addObserver(Observer& _obs)
{
    observers.push_back(&_obs);
}
bool Subject::removeObserver(Observer& _obs)
{
    for(std::vector<Observer*>::iterator it = observers.begin(); it != observers.end(); ++it)
    {
        if(*it == &_obs)
        {

            observers.erase(it);
            return true;
        }
    }
    return false;
}
