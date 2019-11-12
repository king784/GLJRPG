#ifndef EVENT_H
#define EVENT_H

#include "Collider.h"
#include "Debug.h"

class Event
{
public:
    Event(glm::vec3 eventPos)
    {
        collider = new Collider(eventPos - 1.0f, eventPos + 1.0f);
    }
    ~Event()
    {
        delete collider;
    }

    void DoEvent()
    {
        if(!eventDone)
        {
            Debug::GetInstance().Print("Event doned");
            eventDone = true;
        }
    }

    Collider* collider;

private:
    bool eventDone = false;
};

#endif