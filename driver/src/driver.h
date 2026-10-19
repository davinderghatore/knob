#ifndef KNOB_DRIVER_H
#define KNOB_DRIVER_H
#include <CoreAudio/AudioServerPlugIn.h>
#include <CoreAudio/AudioHardware.h>
#include <CoreFoundation/CFPlugInCOM.h>
#include <stdatomic.h>
#include <stdint.h>
#include <string.h>

enum { KN_DEVICE=2, KN_INPUT=3, KN_OUTPUT=4, KN_VOLUME=5, KN_MUTE=6, KN_SOURCE=7 };
enum { KN_CAPACITY=32768, KN_PERIOD=512, KN_DELAY=512, KN_VERSION='knvr' };
extern AudioServerPlugInDriverRef kn_driver;
extern AudioServerPlugInHostRef kn_host;
extern _Atomic UInt32 kn_running, kn_gain_bits, kn_muted, kn_input_active, kn_output_active;
extern _Atomic UInt64 kn_anchor, kn_epoch;
extern double kn_ticks_per_frame;
Boolean kn_object(AudioObjectID object);
OSStatus kn_value(AudioObjectID, const AudioObjectPropertyAddress*, UInt32, const void*, UInt32*, void*);
OSStatus kn_device_value(AudioObjectID, const AudioObjectPropertyAddress*, UInt32, const void*, UInt32*, void*);
OSStatus kn_stream_control_value(AudioObjectID, const AudioObjectPropertyAddress*, UInt32*, void*);
Boolean kn_settable(AudioObjectID, AudioObjectPropertySelector);
void kn_notify(AudioObjectID, AudioObjectPropertySelector);
AudioStreamBasicDescription kn_format(void);
OSStatus kn_copy(UInt32*, void*, const void*, UInt32);
OSStatus kn_string(UInt32*, void*, CFStringRef);
OSStatus kn_set(AudioObjectID, const AudioObjectPropertyAddress*, UInt32, const void*);
OSStatus kn_start(AudioServerPlugInDriverRef,AudioObjectID,UInt32);
OSStatus kn_stop(AudioServerPlugInDriverRef,AudioObjectID,UInt32);
OSStatus kn_zero(AudioServerPlugInDriverRef,AudioObjectID,UInt32,Float64*,UInt64*,UInt64*);
OSStatus kn_will(AudioServerPlugInDriverRef,AudioObjectID,UInt32,UInt32,Boolean*,Boolean*);
OSStatus kn_cycle(AudioServerPlugInDriverRef,AudioObjectID,UInt32,UInt32,UInt32,const AudioServerPlugInIOCycleInfo*);
OSStatus kn_io(AudioServerPlugInDriverRef,AudioObjectID,AudioObjectID,UInt32,UInt32,UInt32,const AudioServerPlugInIOCycleInfo*,void*,void*);
#endif
