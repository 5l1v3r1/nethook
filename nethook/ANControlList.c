//
//  ANControlList.c
//  nethook
//
//  Created by Alex Nichol on 5/11/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "ANControlList.h"


__private_extern__ ANControlList * ANControlListCreate(OSMallocTag tag) {
    ANControlList * list = (ANControlList *)OSMalloc(sizeof(ANControlList), tag);
    if (!list) return NULL;
    bzero(list, sizeof(ANControlList));
    list->tag = tag;
    return list;
}

__private_extern__ void ANControlListFree(ANControlList * list) {
    if (!list) return;
    ANControlListEntry * entry = list->first;
    while (entry) {
        ANControlListEntry * next = entry->next;
        ANControlListEntryFree(list->tag, entry);
        entry = next;
    }
    OSFree(list, sizeof(ANControlList), list->tag);
}

__private_extern__ ANControlListEntry * ANControlListAdd(ANControlList * list, uint32_t unit, kern_ctl_ref ctlref) {
    if (!list) return NULL;
    ANControlListEntry * entry = ANControlListEntryCreate(list->tag, unit, ctlref);
    if (!entry) return NULL;
    entry->next = list->first;
    list->first = entry;
    list->count++;
    return entry;
}

__private_extern__ void ANControlListRemove(ANControlList * list, ANControlListEntry * entry) {
    if (!list) return;
    ANControlListEntry * anEntry = list->first;
    if (anEntry == entry) {
        list->first = (ANControlListEntry *)entry->next;
        return;
    }
    
    boolean_t removed = FALSE;
    while (anEntry) {
        if (anEntry->next == entry) {
            anEntry->next = entry->next;
            removed = TRUE;
            break;
        }
        anEntry = anEntry->next;
    }
    
    if (removed) {
        list->count--;
        ANControlListEntryFree(list->tag, entry);
    }
}

__private_extern__ ANControlListEntry * ANControlListLookupByUnit(ANControlList * list, uint32_t unit) {
    if (!list) return NULL;
    ANControlListEntry * entry = list->first;
    while (entry) {
        if (entry->unit == unit) return entry;
        entry = entry->next;
    }
    return NULL;
}

__private_extern__ ANControlListEntry * ANControlListEntryCreate(OSMallocTag tag, uint32_t unit, kern_ctl_ref ctlref) {
    ANControlListEntry * entry = (ANControlListEntry *)OSMalloc(sizeof(ANControlListEntry), tag);
    if (!entry) return NULL;
    bzero(entry, sizeof(ANControlListEntry));
    entry->unit = unit;
    entry->ctlref = ctlref;
    entry->connected = TRUE;
    return entry;
}

__private_extern__ void ANControlListEntryFree(OSMallocTag tag, ANControlListEntry * entry) {
    OSFree(entry, sizeof(ANControlListEntry), tag);
}