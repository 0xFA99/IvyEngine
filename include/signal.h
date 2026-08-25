#ifndef IVY_SIGNAL_H
#define IVY_SIGNAL_H

#include "types.h"

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define IVY_CONTAINER_OF(ptr, type, member) \
    ((type *)((char *)(ptr) - offsetof(type, member)))

#define IVY_FOR_EACH(pos, list, member) \
    for (pos = IVY_CONTAINER_OF((list)->next, __typeof__(*pos), member); \
        &pos->member != (list); \
        pos = IVY_CONTAINER_OF(pos->member.next, __typeof__(*pos), member))

#define IVY_FOR_EACH_SAFE(pos, temp, list, member) \
    for (pos = IVY_CONTAINER_OF((list)->next, __typeof__(*pos), member), \
        temp = IVY_CONTAINER_OF(pos->member.next, __typeof__(*pos), member); \
        &pos->member != (list); \
        pos = temp, \
        temp = IVY_CONTAINER_OF(pos->member.next, __typeof__(*pos), member))

typedef struct IvyList {
    struct IvyList *prev;
    struct IvyList *next;
} IvyList;

typedef struct IvyListener {
    IvyList link;
    void (*notify)(struct IvyListener *listener, void *data);
} IvyListener;

typedef struct IvySignal {
    IvyList listener_list;
} IvySignal;

IVY_INLINE void Ivy_List_Init(IvyList *list)
{
    list->prev = list;
    list->next = list;
}

IVY_INLINE void Ivy_List_Insert(IvyList *restrict after, IvyList *restrict element)
{
    element->prev       = after;
    element->next       = after->next;
    after->next         = element;
    element->next->prev = element;
}

IVY_INLINE void Ivy_List_Remove(IvyList *element)
{
    element->prev->next = element->next;
    element->next->prev = element->prev;
    element->prev       = NULL;
    element->next       = NULL;
}

IVY_INLINE bool Ivy_List_Empty(const IvyList *list)
{
    return list->next == list;
}

IVY_INLINE void Ivy_List_List(IvyList *restrict at, IvyList *restrict other)
{
    if (Ivy_List_Empty(other)) return;

    other->next->prev   = at;
    other->prev->next   = at->next;
    at->next->prev      = other->prev;
    at->next            = other->next;
    other->prev         = other;
    other->next         = other;
}

IVY_INLINE void Ivy_List_Insert_List(IvyList *restrict at, IvyList *restrict other)
{
    Ivy_List_List(at, other);
}

IVY_INLINE void Ivy_Listener_Init(IvyListener *listener, void (*notify)(IvyListener*, void*))
{
    listener->link.next = NULL;
    listener->link.prev = NULL;
    listener->notify = notify;
}

IVY_INLINE void Ivy_Signal_Init(IvySignal *signal)
{
    Ivy_List_Init(&signal->listener_list);
}

IVY_INLINE void Ivy_Signal_Add(const IvySignal *restrict signal, IvyListener *restrict listener)
{
    Ivy_List_Insert(signal->listener_list.prev, &listener->link);
}

IVY_INLINE void Ivy_Signal_Remove(IvyListener *listener)
{
    if (listener->link.next != NULL)
        Ivy_List_Remove(&listener->link);
}

IVY_INLINE bool Ivy_Signal_Empty(const IvySignal *signal)
{
    return Ivy_List_Empty(&signal->listener_list);
}

IVY_INLINE void Ivy_Signal_Emit(const IvySignal *restrict signal, void *restrict data)
{
    if (Ivy_Signal_Empty(signal)) return;

    IvyListener *listener, *temp;
    IVY_FOR_EACH_SAFE(listener, temp, &signal->listener_list, link) {
        listener->notify(listener, data);
    }
}

#ifdef __cplusplus
}
#endif

#endif