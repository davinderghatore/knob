#include "driver.h"
#include <mach/mach_time.h>

AudioServerPlugInHostRef kn_host=NULL;
static _Atomic UInt32 refs=1;
static HRESULT query(void* d,REFIID uuid,LPVOID* out) {
    if(d!=kn_driver || !out) return E_INVALIDARG;
    *out=NULL; CFUUIDRef id=CFUUIDCreateFromUUIDBytes(NULL,uuid);
    Boolean supported=CFEqual(id,IUnknownUUID) || CFEqual(id,kAudioServerPlugInDriverInterfaceUUID);
    CFRelease(id);
    if(!supported) return E_NOINTERFACE;
    atomic_fetch_add(&refs,1); *out=kn_driver; return S_OK;
}
static ULONG add(void* d) { return d==kn_driver?atomic_fetch_add(&refs,1)+1:0; }
static ULONG release(void* d) {
    if(d!=kn_driver) return 0;
    UInt32 current=atomic_load(&refs);
    while(current && !atomic_compare_exchange_weak(&refs,&current,current-1)) {}
    return current?current-1:0;
}
static OSStatus initialize(AudioServerPlugInDriverRef d,AudioServerPlugInHostRef host) {
    if(d!=kn_driver || !host) return kAudioHardwareIllegalOperationError;
    mach_timebase_info_data_t time; if(mach_timebase_info(&time)!=KERN_SUCCESS) return kAudioHardwareIllegalOperationError;
    kn_ticks_per_frame=(1e9/48000)*time.denom/time.numer;
    atomic_store(&kn_anchor,mach_absolute_time()); kn_host=host; return noErr;
}
static OSStatus create(AudioServerPlugInDriverRef d,CFDictionaryRef desc,const AudioServerPlugInClientInfo* c,AudioObjectID* out) {
    (void)d;(void)desc;(void)c;(void)out; return kAudioHardwareUnsupportedOperationError;
}
static OSStatus destroy(AudioServerPlugInDriverRef d,AudioObjectID o) {
    (void)d;(void)o; return kAudioHardwareUnsupportedOperationError;
}
static OSStatus client(AudioServerPlugInDriverRef d,AudioObjectID o,const AudioServerPlugInClientInfo* c) {
    if(d!=kn_driver || o!=KN_DEVICE) return kAudioHardwareBadObjectError;
    return c?noErr:kAudioHardwareIllegalOperationError;
}
static OSStatus config(AudioServerPlugInDriverRef d,AudioObjectID o,UInt64 action,void* info) {
    (void)action;(void)info;
    if(d!=kn_driver || o!=KN_DEVICE) return kAudioHardwareBadObjectError;
    return kAudioHardwareUnsupportedOperationError;
}
static Boolean has(AudioServerPlugInDriverRef d,AudioObjectID o,pid_t pid,const AudioObjectPropertyAddress* a) {
    (void)pid; UInt32 size=0;
    if(d!=kn_driver) return false;
    return kn_value(o,a,0,NULL,&size,NULL)==noErr;
}
static OSStatus settable(AudioServerPlugInDriverRef d,AudioObjectID o,pid_t pid,const AudioObjectPropertyAddress* a,Boolean* out) {
    if(!out) return kAudioHardwareIllegalOperationError;
    if(!has(d,o,pid,a)) return kn_object(o)?kAudioHardwareUnknownPropertyError:kAudioHardwareBadObjectError;
    *out=kn_settable(o,a->mSelector); return noErr;
}
static OSStatus size(AudioServerPlugInDriverRef d,AudioObjectID o,pid_t pid,const AudioObjectPropertyAddress* a,
                     UInt32 qn,const void* q,UInt32* out) {
    (void)pid;
    if(d!=kn_driver) return kAudioHardwareBadObjectError;
    if(!out) return kAudioHardwareIllegalOperationError;
    return kn_value(o,a,qn,q,out,NULL);
}
static OSStatus get(AudioServerPlugInDriverRef d,AudioObjectID o,pid_t pid,const AudioObjectPropertyAddress* a,
                    UInt32 qn,const void* q,UInt32 capacity,UInt32* out,void* data) {
    (void)pid;
    if(d!=kn_driver) return kAudioHardwareBadObjectError;
    if(!out || (!data && capacity)) return kAudioHardwareIllegalOperationError;
    *out=capacity; return kn_value(o,a,qn,q,out,data);
}
static OSStatus set(AudioServerPlugInDriverRef d,AudioObjectID o,pid_t pid,const AudioObjectPropertyAddress* a,
                    UInt32 qn,const void* q,UInt32 n,const void* data) {
    (void)pid;(void)qn;(void)q;
    if(d!=kn_driver || !kn_object(o)) return kAudioHardwareBadObjectError;
    if(!a || a->mScope!=kAudioObjectPropertyScopeGlobal || a->mElement!=kAudioObjectPropertyElementMain)
        return kAudioHardwareUnknownPropertyError;
    return kn_set(o,a,n,data);
}
static AudioServerPlugInDriverInterface interface={NULL,query,add,release,initialize,create,destroy,
    client,client,config,config,has,settable,size,get,set,kn_start,kn_stop,kn_zero,kn_will,kn_cycle,kn_io,kn_cycle};
static AudioServerPlugInDriverInterface* pointer=&interface;
AudioServerPlugInDriverRef kn_driver=&pointer;
__attribute__((visibility("default"))) void* Knob_Create(CFAllocatorRef allocator,CFUUIDRef type) {
    (void)allocator;
    return type && CFEqual(type,kAudioServerPlugInTypeUUID)?kn_driver:NULL;
}
