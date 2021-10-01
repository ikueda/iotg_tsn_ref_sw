#pragma once

#include <open62541/server.h>
#include <open62541/server_config_default.h>
#include <alsa/asoundlib.h>

/* The binary encoding id's for the datatypes */
#define Alsa_binary_encoding_id        1

/* Alsa setting variables */
#define DEF_CHANNEL         1
#define DEF_SAMPLERATE      16000
#define DEF_BITPERSAMPLE    16
#define DEF_FRAMES          800
#define DEF_FORMAT          SND_PCM_FORMAT_S16_LE

typedef struct {
    UA_UInt16   channel;
    UA_UInt16   samplerate;
    UA_UInt16   bitpersample;
    UA_UInt16   frames;
} AlsaFormat;

typedef void (* FUNC_POINTER)(UA_Server *, UA_UInt32,  void *, const UA_NodeId *, void *, UA_UInt32,  const UA_DataValue *);

/* The datatype description for the Point datatype */
#define AF_padding_sr offsetof(AlsaFormat,samplerate) - offsetof(AlsaFormat,channel) - sizeof(UA_UInt16)
#define AF_padding_bps offsetof(AlsaFormat,bitpersample) - offsetof(AlsaFormat,samplerate) - sizeof(UA_UInt16)
#define AF_padding_frames offsetof(AlsaFormat,frames) - offsetof(AlsaFormat,bitpersample) - sizeof(UA_UInt16)

static UA_DataTypeMember AF_members[4] = {
    /* channel */
    {
        UA_TYPENAME("channel") /* .memberName */
        //&UA_TYPES[UA_TYPES_UINT16], /* .memberTypeIndex, points into UA_TYPES since namespaceZero is true */
        UA_TYPES_UINT16, /* .memberTypeIndex, points into UA_TYPES since namespaceZero is true */
        0,               /* .padding */
        false,           /* .isArray */
        false            /* .isOptional */
    },

    /* samplerate */
    {
        UA_TYPENAME("samplerate") /* .memberName */
        //&UA_TYPES[UA_TYPES_UINT16], /* .memberTypeIndex, points into UA_TYPES since namespaceZero is true */
        UA_TYPES_UINT16, /* .memberTypeIndex, points into UA_TYPES since namespaceZero is true */
        AF_padding_sr,               /* .padding */
        false,           /* .isArray */
        false            /* .isOptional */
    },

    /* bitpersample */
    {
        UA_TYPENAME("bitpersample") /* .memberName */
        //&UA_TYPES[UA_TYPES_UINT16], /* .memberTypeIndex, points into UA_TYPES since namespaceZero is true */
        UA_TYPES_UINT16, /* .memberTypeIndex, points into UA_TYPES since namespaceZero is true */
        AF_padding_bps,               /* .padding */
        false,           /* .isArray */
        false            /* .isOptional */
    },

    /* frames */
    {
        UA_TYPENAME("frames") /* .memberName */
        //&UA_TYPES[UA_TYPES_UINT16], /* .memberTypeIndex, points into UA_TYPES since namespaceZero is true */
        UA_TYPES_UINT16, /* .memberTypeIndex, points into UA_TYPES since namespaceZero is true */
        AF_padding_frames,               /* .padding */
        false,           /* .isArray */
        false            /* .isOptional */
    }
};

static const UA_DataType AFType = {
        UA_TYPENAME("AlsaFormat")                /* .tyspeName */
        {1, UA_NODEIDTYPE_NUMERIC, {4242}}, /* .typeId */
        sizeof(AlsaFormat),                      /* .memSize */
	0,
        UA_DATATYPEKIND_STRUCTURE,          /* .typeKind */
        true,                               /* .pointerFree */
        false,                              /* .overlayable (depends on endianness and
                                            the absence of padding) */
        4,                                  /* .membersSize */
        Alsa_binary_encoding_id, /* .binaryEncodingId, the numeric
                                            identifier used on the wire (the
                                            namespaceindex is from .typeId) */
        AF_members
};

//extern UA_NodeId connectionIdent, publishedDataSetIdent, writerGroupIdent, alsaVariableNode, alsaBufferId;
extern UA_NodeId alsaVariableNode, alsaBufferPubId, alsaBufferSubId;

void addALSADataType(UA_Server* server);
void addALSAVariableType(UA_Server *server);
void addALSAVariable(UA_Server *server);
void addMonitoredItemToCurrentTimeVariable(UA_Server *server,UA_NodeId monitorId, FUNC_POINTER p);

