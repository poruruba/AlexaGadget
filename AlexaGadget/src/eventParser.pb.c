/* Automatically generated nanopb constant definitions */
/* Generated by nanopb-0.3.9.6 at Wed Jul 15 00:57:12 2020. */

#include "eventParser.pb.h"

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif



const pb_field_t event_EventParserProto_fields[2] = {
    PB_FIELD(  1, MESSAGE , SINGULAR, STATIC  , FIRST, event_EventParserProto, event, event, &event_EventParserProto_Event_fields),
    PB_LAST_FIELD
};

const pb_field_t event_EventParserProto_Event_fields[3] = {
    PB_FIELD(  1, MESSAGE , SINGULAR, STATIC  , FIRST, event_EventParserProto_Event, header, header, &header_EventHeaderProto_fields),
    PB_FIELD(  2, BYTES   , SINGULAR, STATIC  , OTHER, event_EventParserProto_Event, payload, header, 0),
    PB_LAST_FIELD
};


/* Check that field information fits in pb_field_t */
#if !defined(PB_FIELD_32BIT)
/* If you get an error here, it means that you need to define PB_FIELD_32BIT
 * compile-time option. You can do that in pb.h or on compiler command line.
 * 
 * The reason you need to do this is that some of your messages contain tag
 * numbers or field sizes that are larger than what can fit in 8 or 16 bit
 * field descriptors.
 */
PB_STATIC_ASSERT((pb_membersize(event_EventParserProto, event) < 65536 && pb_membersize(event_EventParserProto_Event, payload) < 65536 && pb_membersize(event_EventParserProto_Event, header) < 65536), YOU_MUST_DEFINE_PB_FIELD_32BIT_FOR_MESSAGES_event_EventParserProto_event_EventParserProto_Event)
#endif

#if !defined(PB_FIELD_16BIT) && !defined(PB_FIELD_32BIT)
#error Field descriptor for event_EventParserProto_Event.payload is too large. Define PB_FIELD_16BIT to fix this.
#endif


/* @@protoc_insertion_point(eof) */
