/* Copyright G. Hemingway @ 2019, All Rights Reserved */
#ifndef _REACTOR_CPP
#define _REACTOR_CPP

#include "Reactor.h"
#include "Event_Handler.h"
#include <algorithm>
#include <functional>

Reactor* Reactor::inst = nullptr;

Reactor::Reactor()
    : running_event_loop(true)
{
}

/**
 * @class Remove_Handler_Adapter
 * @brief This functor implements the Adapter pattern so an @a
 *        Reactor's remove_input_handler() method can be called
 *        with the appropriate Event_Handler in the context of the
 *        std:for_each() algorithm.
 */
struct Remove_Handler_Adapter {
    // Remote event_handler from the singleton Reactor.
    bool operator()(Event_Handler* event_handler)
    {
        Reactor::instance()->remove_input_handler(event_handler);
        return true;
    }
};

Reactor::~Reactor()
{
    std::for_each(dispatch_table.begin(), dispatch_table.end(), Remove_Handler_Adapter());
}

Reactor* Reactor::instance()
{
    if (Reactor::inst == nullptr)
        Reactor::inst = new Reactor;
    return Reactor::inst;
}

void Reactor::register_input_handler(Event_Handler* eh)
{
    dispatch_table.push_back(eh);
}

void Reactor::remove_input_handler(Event_Handler* eh)
{
    auto iter = std::remove(dispatch_table.begin(), dispatch_table.end(), eh);
    dispatch_table.erase(iter);
    delete eh;
}

void Reactor::run_event_loop()
{
    while (running_event_loop)
        std::for_each(dispatch_table.begin(), dispatch_table.end(),
            std::mem_fn(&Event_Handler::handle_input));
}

void Reactor::end_event_loop()
{
    running_event_loop = false;
}

#endif // REACTOR_CPP
