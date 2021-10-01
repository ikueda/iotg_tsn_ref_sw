#include <open62541/client_subscriptions.h>
#include "custom_datatype.h"

const UA_NodeId alsaVariableTypeId = {
     1, UA_NODEIDTYPE_NUMERIC, {4243}};

UA_NodeId alsaVariableNode, alsaBufferPubId;

void addALSADataType(UA_Server* server)
{
    UA_DataTypeAttributes attr = UA_DataTypeAttributes_default;
    attr.displayName = UA_LOCALIZEDTEXT("en-US", "ALSA Type");

    UA_Server_addDataTypeNode(
        server, AFType.typeId, UA_NODEID_NUMERIC(0, UA_NS0ID_STRUCTURE),
        UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE), UA_QUALIFIEDNAME(1, "Alsa.AlsaFormat"), attr, NULL, NULL);

}

void
addALSAVariableType(UA_Server *server) {
    UA_VariableTypeAttributes dattr = UA_VariableTypeAttributes_default;
    //dattr.description = UA_LOCALIZEDTEXT("en-US", "ALSA Format");
    dattr.displayName = UA_LOCALIZEDTEXT("en-US", "ALSA.Format");
    UA_NodeId_copy(&AFType.typeId, &dattr.dataType);
    //dattr.dataType = AFType.typeId;
    dattr.valueRank = UA_VALUERANK_SCALAR;

    AlsaFormat af;
    af.channel = 0;
    af.samplerate = 0;
    af.bitpersample = 0;
    af.frames = 0;
    UA_Variant_setScalar(&dattr.value, &af, &AFType);

    UA_Server_addVariableTypeNode(server, alsaVariableTypeId,
                                  UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
                                  UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE),
                                  UA_QUALIFIEDNAME(1, "Alsa.AlsaFormat"),
                                  UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
                                  dattr, NULL, NULL);

}

void
addALSAVariable(UA_Server *server) {

    //publisher
    UA_NodeId folderId;
    UA_ObjectAttributes oAttr = UA_ObjectAttributes_default;
    oAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Publisher");
    UA_Server_addObjectNode(server, UA_NODEID_NULL,
        UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
        UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
        UA_QUALIFIEDNAME(1, "Publisher"), UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE),
        oAttr, NULL, &folderId);


    // AlsaFormat
    AlsaFormat af;
    af.channel = DEF_CHANNEL;
    af.samplerate = DEF_SAMPLERATE;
    af.bitpersample = DEF_BITPERSAMPLE;
    af.frames = DEF_FRAMES;

    UA_NodeId_init(&alsaVariableNode);
    UA_VariableAttributes vattr = UA_VariableAttributes_default;
    UA_NodeId_copy(&AFType.typeId, &vattr.dataType);
    //vattr.description = UA_LOCALIZEDTEXT("en-US", "ALSA Format");
    vattr.displayName = UA_LOCALIZEDTEXT("en-US", "ALSA Format");
    vattr.valueRank = UA_VALUERANK_SCALAR;
    UA_Variant_setScalar(&vattr.value, &af, &AFType);

    UA_Server_addVariableNode(server, UA_NODEID_STRING(1, "Alsa.AlsaFormat"),
                              folderId,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                              UA_QUALIFIEDNAME(1, "AlsaFormat"),
                              alsaVariableTypeId, vattr, NULL, &alsaVariableNode);

    UA_Byte data[] = { 0x00 };
    UA_ByteString byteStringVal = { 1, data };

    UA_NodeId_init(&alsaBufferPubId);
    UA_VariableAttributes byteStringAttr = UA_VariableAttributes_default;
    byteStringAttr.valueRank = -1;
    UA_NodeId_copy(&UA_TYPES[UA_TYPES_BYTESTRING].typeId, &byteStringAttr.dataType);
    byteStringAttr.accessLevel = UA_ACCESSLEVELMASK_READ ^ UA_ACCESSLEVELMASK_WRITE;
    UA_Variant_setScalar(&byteStringAttr.value, &byteStringVal, &UA_TYPES[UA_TYPES_BYTESTRING]);
    byteStringAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Alsa Buffer");
    UA_Server_addVariableNode(server, UA_NODEID_STRING(1, "Alsa.Buffer"), folderId,
        UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
        UA_QUALIFIEDNAME(1, "AlsaBuffer"),
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), byteStringAttr, NULL, &alsaBufferPubId);
}

void
addMonitoredItemToCurrentTimeVariable(UA_Server *server,UA_NodeId monitorId, FUNC_POINTER p) {
    //UA_NodeId currentTimeNodeId =
    //    UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_CURRENTTIME);
    UA_MonitoredItemCreateRequest monRequest =
        //UA_MonitoredItemCreateRequest_default(currentTimeNodeId);
        UA_MonitoredItemCreateRequest_default(monitorId);
    monRequest.requestedParameters.samplingInterval = 5.0; /* 100 ms interval */
    UA_Server_createDataChangeMonitoredItem(server, UA_TIMESTAMPSTORETURN_SOURCE,
                                            monRequest, NULL, p);
}
