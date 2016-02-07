#include <vector>

class Observer;
class Subject
{
private:
    std::vector<Observer*> observers;
public:
    void notify();
    void addObserver(Observer& _obs);
    bool removeObserver(Observer& _obs);
};
