#include "Core/Events/event.h"
#include "Core/Memory/Fmemory.h"
#include "Containers/darray.h"

typedef struct registerd_event
{
    void* listner;
    PFN_on_event callback;
} registerd_event;

typedef struct event_code_entry
{
    registerd_event* events;
} event_code_entry;

// Shoud be more than enought, may its chaning in the future.
#define MAX_MESSAGE_CODES 16384

//Static structure
typedef struct event_system_state
{
    // LOOKUP table for event codes.
    event_code_entry registerd[MAX_MESSAGE_CODES];
}event_system_state;

// EVENT SYSTEM INTERNAL STATE
static b8 is_initialsized = False;
static event_system_state state;

b8 event_initialize()
{
    if(is_initialsized == True)
    {
        return False;
    }
    is_initialsized = False;
    fzero_memory(&state,  sizeof(state));

    is_initialsized = True;
    return True;
}

void event_shutdown()
{
    //Free the Events array. And Object Pointed to should be destroyed on their own.
    for(u16 i = 0; i < MAX_EVENTCODE; i++)
    {
        if(state.registerd[i].events != 0)
        {
            darray_destroy(state.registerd[i].events);
            state.registerd[i].events = 0;
        }
    } 
}

b8 event_register(u16 code, void* listener, PFN_on_event on_event)
{
    if(is_initialsized == False)
    {
        return False;
    }
    if(state.registerd[code].events == 0)
    {
        state.registerd[code].events = darray_create(registerd_event);
    }   
    u64 registered_count = darray_lenght(state.registerd[code].events);
    for(u64 i = 0; i < registered_count; i++)
    {
        if(state.registerd[code].events[i].listner == listener)
        {
            //TODO: warn
            return False;
        }
    }

    //If at this point, no duplicate was found. Proceed with Regestration
    registerd_event event;
    event.listner = listener;
    event.callback = on_event;
    darray_push(state.registerd[code].events, event);

    return True;
}

b8 event_unregister(u16 code, void* listner, PFN_on_event on_event)
{
    if(is_initialsized == False)
    {
        return False;
    }

    // On nothing is registered for the code, boot out.
    if(state.registerd[code].events == 0)
    {
        //TODO: Warn
        return False;
    }

    u64 registered_count = darray_lenght(state.registerd[code].events);
    for(u64 i = 0; i < registered_count; i++)
    {
        registerd_event e = state.registerd[code].events[i];
        if(e.listner == listner && e.callback == on_event)
        {
            //Found One, Remove it now.
            registerd_event popped_event;
            darray_pop_at(state.registerd[code].events, i, &popped_event);
            return True;
        }
    }

    //NONE FOUND
    return False;
}

b8 event_fire(u16 code, void* sender, event_context context)
{
    if(is_initialsized == False)
    {
        return False;
    }

    //If nothing is registered for the code, boot out.
    if(state.registerd[code].events == 0)
    {
        return False;
    }

    u64 registerd_count = darray_lenght(state.registerd[code].events); 
    for(u64 i = 0; i < registerd_count; i++)
    {
        registerd_event e = state.registerd[code].events[i];
        if(e.callback(code, sender , e.listner, context))
        {
            //MESSAGE has been handeled, do not send to other listners.
            return True;
        }
    }

    //NONE FOUND
    return False;
}