/* soapStub.h
   Generated by gSOAP 2.7.7 from cwmp.h
   Copyright (C) 2000-2005, Robert van Engelen, Genivia Inc. All Rights Reserved.
   This part of the software is released under one of the following licenses:
   GPL, the gSOAP public license, or Genivia's license for commercial use.
*/

#ifndef soapStub_H
#define soapStub_H
#include "stdsoap2.h"
#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************\
 *                                                                            *
 * Enumerations                                                               *
 *                                                                            *
\******************************************************************************/


#ifndef SOAP_TYPE_mybool
#define SOAP_TYPE_mybool (6)
/* mybool */
enum mybool {_0 = 0, _1 = 1, _true = 1, _false = 0};
#endif

/******************************************************************************\
 *                                                                            *
 * Classes and Structs                                                        *
 *                                                                            *
\******************************************************************************/


#ifndef SOAP_TYPE_xsd__base64
#define SOAP_TYPE_xsd__base64 (12)
/* Base64 schema type: */
struct xsd__base64
{
	unsigned char *__ptr;
	int __size;
};
#endif

#ifndef SOAP_TYPE_xsd__hexBinary
#define SOAP_TYPE_xsd__hexBinary (15)
/* hexBinary schema type: */
struct xsd__hexBinary
{
	unsigned char *__ptr;
	int __size;
};
#endif

#ifndef SOAP_TYPE_cwmp__SetParameterValuesFault
#define SOAP_TYPE_cwmp__SetParameterValuesFault (16)
/* cwmp:SetParameterValuesFault */
struct cwmp__SetParameterValuesFault
{
	char *ParameterName;	/* optional element of type xsd:string */
	unsigned int FaultCode;	/* required element of type xsd:unsignedInt */
	char *FaultString;	/* optional element of type xsd:string */
};
#endif

#ifndef SOAP_TYPE_cwmp__Fault
#define SOAP_TYPE_cwmp__Fault (17)
/* cwmp:Fault */
struct cwmp__Fault
{
	unsigned int FaultCode;	/* required element of type xsd:unsignedInt */
	char *FaultString;	/* optional element of type xsd:string */
	int __sizeSPVF;	/* sequence of elements <SetParameterValuesFault> */
	struct cwmp__SetParameterValuesFault *SetParameterValuesFault;	/* optional element of type cwmp:SetParameterValuesFault */
};
#endif

#ifndef SOAP_TYPE_SOAP_ENV__Detail
#define SOAP_TYPE_SOAP_ENV__Detail (19)
/* SOAP-ENV:Detail */
struct SOAP_ENV__Detail
{
	struct cwmp__Fault *cwmp__Fault;	/* optional element of type cwmp:Fault */
	char *__any;
};
#endif

#ifndef SOAP_TYPE_SOAP_ENV__Header
#define SOAP_TYPE_SOAP_ENV__Header (21)
/* SOAP Header: */
struct SOAP_ENV__Header
{
	char *cwmp__ID;	/* mustUnderstand */
	enum mybool *cwmp__HoldRequests;	/* mustUnderstand */
	enum mybool *cwmp__NoMoreRequests;	/* optional element of type xsd:boolean */
};
#endif

#ifndef SOAP_TYPE_cwmp__ParameterValueStruct
#define SOAP_TYPE_cwmp__ParameterValueStruct (23)
/* cwmp:ParameterValueStruct */
struct cwmp__ParameterValueStruct
{
	char *Name;	/* optional element of type xsd:string */
	int __type;	/* any type of element <Value> (defined below) */
	void *Value;	/* transient */
};
#endif

#ifndef SOAP_TYPE_ParameterValueList
#define SOAP_TYPE_ParameterValueList (26)
/* SOAP encoded array of cwmp:ParameterValueStruct schema type: */
struct ParameterValueList
{
	struct cwmp__ParameterValueStruct *__ptrParameterValueStruct;
	int __size;
};
#endif

#ifndef SOAP_TYPE_ArrayOfStrings
#define SOAP_TYPE_ArrayOfStrings (28)
/* SOAP encoded array of xsd:string schema type: */
struct ArrayOfStrings
{
	char **__ptrstring;
	int __size;
};
#endif

#ifndef SOAP_TYPE_cwmp__GetRPCMethodsResponse
#define SOAP_TYPE_cwmp__GetRPCMethodsResponse (30)
/* cwmp:GetRPCMethodsResponse */
struct cwmp__GetRPCMethodsResponse
{
	struct ArrayOfStrings MethodList;	/* RPC return element */	/* required element of type ArrayOfstring */
};
#endif

#ifndef SOAP_TYPE_cwmp__DeviceldStruct
#define SOAP_TYPE_cwmp__DeviceldStruct (31)
/* cwmp:DeviceldStruct */
struct cwmp__DeviceldStruct
{
	char *Manufacturer;	/* optional element of type xsd:string */
	char *OUI;	/* optional element of type xsd:string */
	char *ProductClass;	/* optional element of type xsd:string */
	char *SerialNumber;	/* optional element of type xsd:string */
};
#endif

#ifndef SOAP_TYPE_cwmp__EventStruct
#define SOAP_TYPE_cwmp__EventStruct (32)
/* cwmp:EventStruct */
struct cwmp__EventStruct
{
	char *EventCode;	/* optional element of type xsd:string */
	char *CommandKey;	/* optional element of type xsd:string */
};
#endif

#ifndef SOAP_TYPE_Event
#define SOAP_TYPE_Event (33)
/* SOAP encoded array of cwmp:EventStruct schema type: */
struct Event
{
	struct cwmp__EventStruct *__ptrEventStruct;
	int __size;
};
#endif

#ifndef SOAP_TYPE_cwmp__X_OUI_DeviceInfoStruct
#define SOAP_TYPE_cwmp__X_OUI_DeviceInfoStruct (35)
/* cwmp:X-OUI-DeviceInfoStruct */
struct cwmp__X_OUI_DeviceInfoStruct
{
	char *X_OUI_DeviceManufacturerOUI;	/* optional element of type xsd:string */
	char *X_OUI_DeviceSerialNumber;	/* optional element of type xsd:string */
	char *X_OUI_DeviceProductClass;	/* optional element of type xsd:string */
};
#endif

#ifndef SOAP_TYPE_X_OUI_DeviceInfoStruct
#define SOAP_TYPE_X_OUI_DeviceInfoStruct (36)
/* SOAP encoded array of cwmp:X-OUI-DeviceInfoStruct schema type: */
struct X_OUI_DeviceInfoStruct
{
	struct cwmp__X_OUI_DeviceInfoStruct *__ptrX_OUI_DeviceInfoStruct;
	int __size;
};
#endif

#ifndef SOAP_TYPE_cwmp__InformResponse
#define SOAP_TYPE_cwmp__InformResponse (38)
/* cwmp:InformResponse */
struct cwmp__InformResponse
{
	unsigned int MaxEnvelopes;	/* RPC return element */	/* required element of type xsd:unsignedInt */
};
#endif

#ifndef SOAP_TYPE_cwmp__FaultStruct
#define SOAP_TYPE_cwmp__FaultStruct (39)
/* cwmp:FaultStruct */
struct cwmp__FaultStruct
{
	unsigned int FaultCode;	/* required element of type xsd:unsignedInt */
	char *FaultString;	/* optional element of type xsd:string */
};
#endif

#ifndef SOAP_TYPE_cwmp__TransferCompleteResponse
#define SOAP_TYPE_cwmp__TransferCompleteResponse (40)
/* cwmp:TransferCompleteResponse */
struct cwmp__TransferCompleteResponse
{
	void *a;	/* RPC return element */	/* transient */
};
#endif

#ifndef SOAP_TYPE_cwmp__SetParameterValuesResponse
#define SOAP_TYPE_cwmp__SetParameterValuesResponse (41)
/* cwmp:SetParameterValuesResponse */
struct cwmp__SetParameterValuesResponse
{
	int Status;	/* RPC return element */	/* required element of type xsd:int */
};
#endif

#ifndef SOAP_TYPE_cwmp__GetParameterValuesResponse
#define SOAP_TYPE_cwmp__GetParameterValuesResponse (42)
/* cwmp:GetParameterValuesResponse */
struct cwmp__GetParameterValuesResponse
{
	struct ParameterValueList *ParameterList;	/* RPC return element */	/* optional element of type ArrayOfParameterValueStruct */
	char *__any;
};
#endif

#ifndef SOAP_TYPE_cwmp__ParameterInfoStruct
#define SOAP_TYPE_cwmp__ParameterInfoStruct (44)
/* cwmp:ParameterInfoStruct */
struct cwmp__ParameterInfoStruct
{
	char *Name;	/* optional element of type xsd:string */
	enum mybool Writable;	/* required element of type xsd:boolean */
};
#endif

#ifndef SOAP_TYPE_ParameterNameList
#define SOAP_TYPE_ParameterNameList (45)
/* SOAP encoded array of cwmp:ParameterInfoStruct schema type: */
struct ParameterNameList
{
	struct cwmp__ParameterInfoStruct *__ptrParameterInfoStruct;
	int __size;
};
#endif

#ifndef SOAP_TYPE_cwmp__GetParameterNamesResponse
#define SOAP_TYPE_cwmp__GetParameterNamesResponse (47)
/* cwmp:GetParameterNamesResponse */
struct cwmp__GetParameterNamesResponse
{
	struct ParameterNameList *ParameterList;	/* RPC return element */	/* optional element of type ArrayOfParameterInfoStruct */
	char *__any;
};
#endif

#ifndef SOAP_TYPE_cwmp__SetParameterAttributesStruct
#define SOAP_TYPE_cwmp__SetParameterAttributesStruct (49)
/* cwmp:SetParameterAttributesStruct */
struct cwmp__SetParameterAttributesStruct
{
	char *Name;	/* optional element of type xsd:string */
	enum mybool NotificationChange;	/* required element of type xsd:boolean */
	int Notification;	/* required element of type xsd:int */
	enum mybool AccessListChange;	/* required element of type xsd:boolean */
	struct ArrayOfStrings AccessList;	/* required element of type ArrayOfstring */
};
#endif

#ifndef SOAP_TYPE_ParameterAttributesList
#define SOAP_TYPE_ParameterAttributesList (50)
/* SOAP encoded array of cwmp:SetParameterAttributesStruct schema type: */
struct ParameterAttributesList
{
	struct cwmp__SetParameterAttributesStruct *__ptrSetParameterAttributesStruct;
	int __size;
};
#endif

#ifndef SOAP_TYPE_cwmp__SetParameterAttributesResponse
#define SOAP_TYPE_cwmp__SetParameterAttributesResponse (52)
/* cwmp:SetParameterAttributesResponse */
struct cwmp__SetParameterAttributesResponse
{
	void *a;	/* RPC return element */	/* transient */
};
#endif

#ifndef SOAP_TYPE_cwmp__ParameterAttributeStruct
#define SOAP_TYPE_cwmp__ParameterAttributeStruct (53)
/* cwmp:ParameterAttributeStruct */
struct cwmp__ParameterAttributeStruct
{
	char *Name;	/* optional element of type xsd:string */
	int Notification;	/* required element of type xsd:int */
	struct ArrayOfStrings AccessList;	/* required element of type ArrayOfstring */
};
#endif

#ifndef SOAP_TYPE_ParameterAttributesStructList
#define SOAP_TYPE_ParameterAttributesStructList (54)
/* SOAP encoded array of cwmp:ParameterAttributeStruct schema type: */
struct ParameterAttributesStructList
{
	struct cwmp__ParameterAttributeStruct *__ptrParameterAttributeStruct;
	int __size;
};
#endif

#ifndef SOAP_TYPE_cwmp__GetParameterAttributesResponse
#define SOAP_TYPE_cwmp__GetParameterAttributesResponse (56)
/* cwmp:GetParameterAttributesResponse */
struct cwmp__GetParameterAttributesResponse
{
	struct ParameterAttributesStructList *ParameterList;	/* RPC return element */	/* optional element of type ArrayOfParameterAttributeStruct */
	char *__any;
};
#endif

#ifndef SOAP_TYPE_cwmp__AddObjectResponse
#define SOAP_TYPE_cwmp__AddObjectResponse (58)
/* cwmp:AddObjectResponse */
struct cwmp__AddObjectResponse
{
	unsigned int InstanceNumber;	/* RPC return element */	/* required element of type xsd:unsignedInt */
	int Status;	/* required element of type xsd:int */
};
#endif

#ifndef SOAP_TYPE_cwmp__DeleteObjectResponse
#define SOAP_TYPE_cwmp__DeleteObjectResponse (59)
/* cwmp:DeleteObjectResponse */
struct cwmp__DeleteObjectResponse
{
	int Status;	/* RPC return element */	/* required element of type xsd:int */
};
#endif

#ifndef SOAP_TYPE_cwmp__DownloadResponse
#define SOAP_TYPE_cwmp__DownloadResponse (60)
/* cwmp:DownloadResponse */
struct cwmp__DownloadResponse
{
	int Status;	/* RPC return element */	/* required element of type xsd:int */
	time_t StartTime;	/* required element of type xsd:dateTime */
	time_t CompleteTime;	/* required element of type xsd:dateTime */
};
#endif

#ifndef SOAP_TYPE_cwmp__RebootResponse
#define SOAP_TYPE_cwmp__RebootResponse (62)
/* cwmp:RebootResponse */
struct cwmp__RebootResponse
{
	void *a;	/* RPC return element */	/* transient */
};
#endif

#ifndef SOAP_TYPE_cwmp__UploadResponse
#define SOAP_TYPE_cwmp__UploadResponse (63)
/* cwmp:UploadResponse */
struct cwmp__UploadResponse
{
	int Status;	/* RPC return element */	/* required element of type xsd:int */
	time_t StartTime;	/* required element of type xsd:dateTime */
	time_t CompleteTime;	/* required element of type xsd:dateTime */
};
#endif

#ifndef SOAP_TYPE_cwmp__FactoryResetResponse
#define SOAP_TYPE_cwmp__FactoryResetResponse (64)
/* cwmp:FactoryResetResponse */
struct cwmp__FactoryResetResponse
{
	void *a;	/* RPC return element */	/* transient */
};
#endif

#ifndef SOAP_TYPE_cwmp__ScheduleInformResponse
#define SOAP_TYPE_cwmp__ScheduleInformResponse (65)
/* cwmp:ScheduleInformResponse */
struct cwmp__ScheduleInformResponse
{
	void *a;	/* RPC return element */	/* transient */
};
#endif

#ifndef SOAP_TYPE_cwmp__TransferStruct
#define SOAP_TYPE_cwmp__TransferStruct (66)
/* cwmp:TransferStruct */
struct cwmp__TransferStruct
{
	char *CommandKey;	/* optional element of type xsd:string */
	int *State;	/* optional element of type xsd:int */
	int *IsDownload;	/* optional element of type xsd:int */
	char *FileType;	/* optional element of type xsd:string */
	unsigned int *FileSize;	/* optional element of type xsd:unsignedInt */
	char *TargetFileName;	/* optional element of type xsd:string */
};
#endif

#ifndef SOAP_TYPE_AllQueuedTransferList
#define SOAP_TYPE_AllQueuedTransferList (69)
/* SOAP encoded array of cwmp:TransferStruct schema type: */
struct AllQueuedTransferList
{
	struct cwmp__TransferStruct *__ptrTransferStruct;
	int __size;
};
#endif

#ifndef SOAP_TYPE_cwmp__GetAllQueuedTransfersResponse
#define SOAP_TYPE_cwmp__GetAllQueuedTransfersResponse (71)
/* cwmp:GetAllQueuedTransfersResponse */
struct cwmp__GetAllQueuedTransfersResponse
{
	struct AllQueuedTransferList *TransferList;	/* RPC return element */	/* optional element of type ArrayOfTransferStruct */
	char *__any;
};
#endif

#ifndef SOAP_TYPE_cwmp__QueuedTransferStruct
#define SOAP_TYPE_cwmp__QueuedTransferStruct (73)
/* cwmp:QueuedTransferStruct */
struct cwmp__QueuedTransferStruct
{
	char *CommandKey;	/* optional element of type xsd:string */
	int *State;	/* optional element of type xsd:int */
};
#endif

#ifndef SOAP_TYPE_QueuedTransferList
#define SOAP_TYPE_QueuedTransferList (74)
/* SOAP encoded array of cwmp:QueuedTransferStruct schema type: */
struct QueuedTransferList
{
	struct cwmp__QueuedTransferStruct *__ptrTransferStruct;
	int __size;
};
#endif

#ifndef SOAP_TYPE_cwmp__GetQueuedTransfersResponse
#define SOAP_TYPE_cwmp__GetQueuedTransfersResponse (76)
/* cwmp:GetQueuedTransfersResponse */
struct cwmp__GetQueuedTransfersResponse
{
	struct QueuedTransferList *TransferList;	/* RPC return element */	/* optional element of type ArrayOfQueuedTransferStruct */
	char *__any;
};
#endif

#ifndef SOAP_TYPE_cwmp__CancelTransferResponse
#define SOAP_TYPE_cwmp__CancelTransferResponse (78)
/* cwmp:CancelTransferResponse */
struct cwmp__CancelTransferResponse
{
	void *a;	/* RPC return element */	/* transient */
};
#endif

#ifndef SOAP_TYPE_cwmp__GetRPCMethods
#define SOAP_TYPE_cwmp__GetRPCMethods (81)
/* cwmp:GetRPCMethods */
struct cwmp__GetRPCMethods
{
	void *Req;	/* transient */
};
#endif

#ifndef SOAP_TYPE_cwmp__Inform
#define SOAP_TYPE_cwmp__Inform (85)
/* cwmp:Inform */
struct cwmp__Inform
{
	struct cwmp__DeviceldStruct DeviceId;	/* required element of type cwmp:DeviceldStruct */
	struct Event Event;	/* required element of type ArrayOfEventStruct */
	unsigned int MaxEnvelopes;	/* required element of type xsd:unsignedInt */
	time_t CurrentTime;	/* required element of type xsd:dateTime */
	unsigned int RetryCount;	/* required element of type xsd:unsignedInt */
	struct ParameterValueList ParameterList;	/* required element of type ArrayOfParameterValueStruct */
	struct X_OUI_DeviceInfoStruct *X_OUI_AssocDevice;	/* optional element of type ArrayOfX-OUI-DeviceInfoStruct */
};
#endif

#ifndef SOAP_TYPE_cwmp__TransferComplete
#define SOAP_TYPE_cwmp__TransferComplete (88)
/* cwmp:TransferComplete */
struct cwmp__TransferComplete
{
	char *CommandKey;	/* optional element of type xsd:string */
	struct cwmp__FaultStruct FaultStruct;	/* required element of type cwmp:FaultStruct */
	time_t StartTime;	/* required element of type xsd:dateTime */
	time_t CompleteTime;	/* required element of type xsd:dateTime */
};
#endif

#ifndef SOAP_TYPE_cwmp__SetParameterValues
#define SOAP_TYPE_cwmp__SetParameterValues (91)
/* cwmp:SetParameterValues */
struct cwmp__SetParameterValues
{
	struct ParameterValueList ParameterList;	/* required element of type ArrayOfParameterValueStruct */
	char *ParameterKey;	/* optional element of type xsd:string */
};
#endif

#ifndef SOAP_TYPE_cwmp__GetParameterValues
#define SOAP_TYPE_cwmp__GetParameterValues (94)
/* cwmp:GetParameterValues */
struct cwmp__GetParameterValues
{
	struct ArrayOfStrings ParameterNames;	/* required element of type ArrayOfstring */
};
#endif

#ifndef SOAP_TYPE_cwmp__GetParameterNames
#define SOAP_TYPE_cwmp__GetParameterNames (97)
/* cwmp:GetParameterNames */
struct cwmp__GetParameterNames
{
	char *ParameterPath;	/* optional element of type xsd:string */
	enum mybool NextLevel;	/* required element of type xsd:boolean */
};
#endif

#ifndef SOAP_TYPE_cwmp__SetParameterAttributes
#define SOAP_TYPE_cwmp__SetParameterAttributes (100)
/* cwmp:SetParameterAttributes */
struct cwmp__SetParameterAttributes
{
	struct ParameterAttributesList ParameterList;	/* required element of type ArrayOfSetParameterAttributesStruct */
};
#endif

#ifndef SOAP_TYPE_cwmp__GetParameterAttributes
#define SOAP_TYPE_cwmp__GetParameterAttributes (103)
/* cwmp:GetParameterAttributes */
struct cwmp__GetParameterAttributes
{
	struct ArrayOfStrings ParameterNames;	/* required element of type ArrayOfstring */
};
#endif

#ifndef SOAP_TYPE_cwmp__AddObject
#define SOAP_TYPE_cwmp__AddObject (106)
/* cwmp:AddObject */
struct cwmp__AddObject
{
	char *ObjectName;	/* optional element of type xsd:string */
	char *ParameterKey;	/* optional element of type xsd:string */
};
#endif

#ifndef SOAP_TYPE_cwmp__DeleteObject
#define SOAP_TYPE_cwmp__DeleteObject (109)
/* cwmp:DeleteObject */
struct cwmp__DeleteObject
{
	char *ObjectName;	/* optional element of type xsd:string */
	char *ParameterKey;	/* optional element of type xsd:string */
};
#endif

#ifndef SOAP_TYPE_cwmp__Download
#define SOAP_TYPE_cwmp__Download (112)
/* cwmp:Download */
struct cwmp__Download
{
	char *CommandKey;	/* optional element of type xsd:string */
	char *FileType;	/* optional element of type xsd:string */
	char *URL;	/* optional element of type xsd:string */
	char *Username;	/* optional element of type xsd:string */
	char *Password;	/* optional element of type xsd:string */
	unsigned int FileSize;	/* required element of type xsd:unsignedInt */
	char *TargetFileName;	/* optional element of type xsd:string */
	unsigned int DelaySeconds;	/* required element of type xsd:unsignedInt */
	char *SuccessURL;	/* optional element of type xsd:string */
	char *FailureURL;	/* optional element of type xsd:string */
};
#endif

#ifndef SOAP_TYPE_cwmp__Reboot
#define SOAP_TYPE_cwmp__Reboot (115)
/* cwmp:Reboot */
struct cwmp__Reboot
{
	char *CommandKey;	/* optional element of type xsd:string */
};
#endif

#ifndef SOAP_TYPE_cwmp__Upload
#define SOAP_TYPE_cwmp__Upload (118)
/* cwmp:Upload */
struct cwmp__Upload
{
	char *CommandKey;	/* optional element of type xsd:string */
	char *FileType;	/* optional element of type xsd:string */
	char *URL;	/* optional element of type xsd:string */
	char *Username;	/* optional element of type xsd:string */
	char *Password;	/* optional element of type xsd:string */
	unsigned int DelaySeconds;	/* required element of type xsd:unsignedInt */
};
#endif

#ifndef SOAP_TYPE_cwmp__FactoryReset
#define SOAP_TYPE_cwmp__FactoryReset (121)
/* cwmp:FactoryReset */
struct cwmp__FactoryReset
{
	void *a;	/* transient */
};
#endif

#ifndef SOAP_TYPE_cwmp__ScheduleInform
#define SOAP_TYPE_cwmp__ScheduleInform (124)
/* cwmp:ScheduleInform */
struct cwmp__ScheduleInform
{
	unsigned int DelaySeconds;	/* required element of type xsd:unsignedInt */
	char *CommandKey;	/* optional element of type xsd:string */
};
#endif

#ifndef SOAP_TYPE_cwmp__GetAllQueuedTransfers
#define SOAP_TYPE_cwmp__GetAllQueuedTransfers (127)
/* cwmp:GetAllQueuedTransfers */
struct cwmp__GetAllQueuedTransfers
{
	void *Req;	/* transient */
};
#endif

#ifndef SOAP_TYPE_cwmp__GetQueuedTransfers
#define SOAP_TYPE_cwmp__GetQueuedTransfers (130)
/* cwmp:GetQueuedTransfers */
struct cwmp__GetQueuedTransfers
{
	void *Req;	/* transient */
};
#endif

#ifndef SOAP_TYPE_cwmp__CancelTransfer
#define SOAP_TYPE_cwmp__CancelTransfer (133)
/* cwmp:CancelTransfer */
struct cwmp__CancelTransfer
{
	char *CommandKey;	/* optional element of type xsd:string */
};
#endif

#ifndef SOAP_TYPE_SOAP_ENV__Code
#define SOAP_TYPE_SOAP_ENV__Code (134)
/* SOAP Fault Code: */
struct SOAP_ENV__Code
{
	char *SOAP_ENV__Value;	/* optional element of type QName */
	struct SOAP_ENV__Code *SOAP_ENV__Subcode;	/* optional element of type SOAP-ENV:Code */
};
#endif

#ifndef SOAP_TYPE_SOAP_ENV__Reason
#define SOAP_TYPE_SOAP_ENV__Reason (136)
/* SOAP-ENV:Reason */
struct SOAP_ENV__Reason
{
	char *SOAP_ENV__Text;	/* optional element of type xsd:string */
};
#endif

#ifndef SOAP_TYPE_SOAP_ENV__Fault
#define SOAP_TYPE_SOAP_ENV__Fault (137)
/* SOAP Fault: */
struct SOAP_ENV__Fault
{
	char *faultcode;	/* optional element of type QName */
	char *faultstring;	/* optional element of type xsd:string */
	char *faultactor;	/* optional element of type xsd:string */
	struct SOAP_ENV__Detail *detail;	/* optional element of type SOAP-ENV:Detail */
	struct SOAP_ENV__Code *SOAP_ENV__Code;	/* optional element of type SOAP-ENV:Code */
	struct SOAP_ENV__Reason *SOAP_ENV__Reason;	/* optional element of type SOAP-ENV:Reason */
	char *SOAP_ENV__Node;	/* optional element of type xsd:string */
	char *SOAP_ENV__Role;	/* optional element of type xsd:string */
	struct SOAP_ENV__Detail *SOAP_ENV__Detail;	/* optional element of type SOAP-ENV:Detail */
};
#endif

/******************************************************************************\
 *                                                                            *
 * Types with Custom Serializers                                              *
 *                                                                            *
\******************************************************************************/


/******************************************************************************\
 *                                                                            *
 * Typedefs                                                                   *
 *                                                                            *
\******************************************************************************/

#ifndef SOAP_TYPE__XML
#define SOAP_TYPE__XML (4)
typedef char *_XML;
#endif

#ifndef SOAP_TYPE__QName
#define SOAP_TYPE__QName (5)
typedef char *_QName;
#endif

#ifndef SOAP_TYPE_xsd__boolean
#define SOAP_TYPE_xsd__boolean (7)
typedef enum mybool xsd__boolean;
#endif

#ifndef SOAP_TYPE_xsd__unsignedLong
#define SOAP_TYPE_xsd__unsignedLong (11)
typedef unsigned long xsd__unsignedLong;
#endif


/******************************************************************************\
 *                                                                            *
 * Typedef Synonyms                                                           *
 *                                                                            *
\******************************************************************************/


/******************************************************************************\
 *                                                                            *
 * Externals                                                                  *
 *                                                                            *
\******************************************************************************/


/******************************************************************************\
 *                                                                            *
 * Service Operations                                                         *
 *                                                                            *
\******************************************************************************/


SOAP_FMAC5 int SOAP_FMAC6 cwmp__GetRPCMethods(struct soap*, void *Req, struct cwmp__GetRPCMethodsResponse *Resp);

SOAP_FMAC5 int SOAP_FMAC6 cwmp__Inform(struct soap*, struct cwmp__DeviceldStruct DeviceId, struct Event Event, unsigned int MaxEnvelopes, time_t CurrentTime, unsigned int RetryCount, struct ParameterValueList ParameterList, struct X_OUI_DeviceInfoStruct *X_OUI_AssocDevice, struct cwmp__InformResponse *Resp);

SOAP_FMAC5 int SOAP_FMAC6 cwmp__TransferComplete(struct soap*, char *CommandKey, struct cwmp__FaultStruct FaultStruct, time_t StartTime, time_t CompleteTime, struct cwmp__TransferCompleteResponse *Resp);

SOAP_FMAC5 int SOAP_FMAC6 cwmp__SetParameterValues(struct soap*, struct ParameterValueList ParameterList, char *ParameterKey, struct cwmp__SetParameterValuesResponse *Resp);

SOAP_FMAC5 int SOAP_FMAC6 cwmp__GetParameterValues(struct soap*, struct ArrayOfStrings ParameterNames, struct cwmp__GetParameterValuesResponse *Resp);

SOAP_FMAC5 int SOAP_FMAC6 cwmp__GetParameterNames(struct soap*, char *ParameterPath, enum mybool NextLevel, struct cwmp__GetParameterNamesResponse *Resp);

SOAP_FMAC5 int SOAP_FMAC6 cwmp__SetParameterAttributes(struct soap*, struct ParameterAttributesList ParameterList, struct cwmp__SetParameterAttributesResponse *Resp);

SOAP_FMAC5 int SOAP_FMAC6 cwmp__GetParameterAttributes(struct soap*, struct ArrayOfStrings ParameterNames, struct cwmp__GetParameterAttributesResponse *Resp);

SOAP_FMAC5 int SOAP_FMAC6 cwmp__AddObject(struct soap*, char *ObjectName, char *ParameterKey, struct cwmp__AddObjectResponse *Resp);

SOAP_FMAC5 int SOAP_FMAC6 cwmp__DeleteObject(struct soap*, char *ObjectName, char *ParameterKey, struct cwmp__DeleteObjectResponse *Resp);

SOAP_FMAC5 int SOAP_FMAC6 cwmp__Download(struct soap*, char *CommandKey, char *FileType, char *URL, char *Username, char *Password, unsigned int FileSize, char *TargetFileName, unsigned int DelaySeconds, char *SuccessURL, char *FailureURL, struct cwmp__DownloadResponse *Resp);

SOAP_FMAC5 int SOAP_FMAC6 cwmp__Reboot(struct soap*, char *CommandKey, struct cwmp__RebootResponse *Resp);

SOAP_FMAC5 int SOAP_FMAC6 cwmp__Upload(struct soap*, char *CommandKey, char *FileType, char *URL, char *Username, char *Password, unsigned int DelaySeconds, struct cwmp__UploadResponse *Resp);

SOAP_FMAC5 int SOAP_FMAC6 cwmp__FactoryReset(struct soap*, void *a, struct cwmp__FactoryResetResponse *Resp);

SOAP_FMAC5 int SOAP_FMAC6 cwmp__ScheduleInform(struct soap*, unsigned int DelaySeconds, char *CommandKey, struct cwmp__ScheduleInformResponse *Resp);

SOAP_FMAC5 int SOAP_FMAC6 cwmp__GetAllQueuedTransfers(struct soap*, void *Req, struct cwmp__GetAllQueuedTransfersResponse *Resp);

SOAP_FMAC5 int SOAP_FMAC6 cwmp__GetQueuedTransfers(struct soap*, void *Req, struct cwmp__GetQueuedTransfersResponse *Resp);

SOAP_FMAC5 int SOAP_FMAC6 cwmp__CancelTransfer(struct soap*, char *CommandKey, struct cwmp__CancelTransferResponse *Resp);

/******************************************************************************\
 *                                                                            *
 * Stubs                                                                      *
 *                                                                            *
\******************************************************************************/


SOAP_FMAC5 int SOAP_FMAC6 soap_call_cwmp__GetRPCMethods(struct soap *soap, const char *soap_endpoint, const char *soap_action, void *Req, struct cwmp__GetRPCMethodsResponse *Resp);

SOAP_FMAC5 int SOAP_FMAC6 soap_call_cwmp__Inform(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct cwmp__DeviceldStruct DeviceId, struct Event Event, unsigned int MaxEnvelopes, time_t CurrentTime, unsigned int RetryCount, struct ParameterValueList ParameterList, struct X_OUI_DeviceInfoStruct *X_OUI_AssocDevice, struct cwmp__InformResponse *Resp);

SOAP_FMAC5 int SOAP_FMAC6 soap_call_cwmp__TransferComplete(struct soap *soap, const char *soap_endpoint, const char *soap_action, char *CommandKey, struct cwmp__FaultStruct FaultStruct, time_t StartTime, time_t CompleteTime, struct cwmp__TransferCompleteResponse *Resp);

SOAP_FMAC5 int SOAP_FMAC6 soap_call_cwmp__SetParameterValues(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct ParameterValueList ParameterList, char *ParameterKey, struct cwmp__SetParameterValuesResponse *Resp);

SOAP_FMAC5 int SOAP_FMAC6 soap_call_cwmp__GetParameterValues(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct ArrayOfStrings ParameterNames, struct cwmp__GetParameterValuesResponse *Resp);

SOAP_FMAC5 int SOAP_FMAC6 soap_call_cwmp__GetParameterNames(struct soap *soap, const char *soap_endpoint, const char *soap_action, char *ParameterPath, enum mybool NextLevel, struct cwmp__GetParameterNamesResponse *Resp);

SOAP_FMAC5 int SOAP_FMAC6 soap_call_cwmp__SetParameterAttributes(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct ParameterAttributesList ParameterList, struct cwmp__SetParameterAttributesResponse *Resp);

SOAP_FMAC5 int SOAP_FMAC6 soap_call_cwmp__GetParameterAttributes(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct ArrayOfStrings ParameterNames, struct cwmp__GetParameterAttributesResponse *Resp);

SOAP_FMAC5 int SOAP_FMAC6 soap_call_cwmp__AddObject(struct soap *soap, const char *soap_endpoint, const char *soap_action, char *ObjectName, char *ParameterKey, struct cwmp__AddObjectResponse *Resp);

SOAP_FMAC5 int SOAP_FMAC6 soap_call_cwmp__DeleteObject(struct soap *soap, const char *soap_endpoint, const char *soap_action, char *ObjectName, char *ParameterKey, struct cwmp__DeleteObjectResponse *Resp);

SOAP_FMAC5 int SOAP_FMAC6 soap_call_cwmp__Download(struct soap *soap, const char *soap_endpoint, const char *soap_action, char *CommandKey, char *FileType, char *URL, char *Username, char *Password, unsigned int FileSize, char *TargetFileName, unsigned int DelaySeconds, char *SuccessURL, char *FailureURL, struct cwmp__DownloadResponse *Resp);

SOAP_FMAC5 int SOAP_FMAC6 soap_call_cwmp__Reboot(struct soap *soap, const char *soap_endpoint, const char *soap_action, char *CommandKey, struct cwmp__RebootResponse *Resp);

SOAP_FMAC5 int SOAP_FMAC6 soap_call_cwmp__Upload(struct soap *soap, const char *soap_endpoint, const char *soap_action, char *CommandKey, char *FileType, char *URL, char *Username, char *Password, unsigned int DelaySeconds, struct cwmp__UploadResponse *Resp);

SOAP_FMAC5 int SOAP_FMAC6 soap_call_cwmp__FactoryReset(struct soap *soap, const char *soap_endpoint, const char *soap_action, void *a, struct cwmp__FactoryResetResponse *Resp);

SOAP_FMAC5 int SOAP_FMAC6 soap_call_cwmp__ScheduleInform(struct soap *soap, const char *soap_endpoint, const char *soap_action, unsigned int DelaySeconds, char *CommandKey, struct cwmp__ScheduleInformResponse *Resp);

SOAP_FMAC5 int SOAP_FMAC6 soap_call_cwmp__GetAllQueuedTransfers(struct soap *soap, const char *soap_endpoint, const char *soap_action, void *Req, struct cwmp__GetAllQueuedTransfersResponse *Resp);

SOAP_FMAC5 int SOAP_FMAC6 soap_call_cwmp__GetQueuedTransfers(struct soap *soap, const char *soap_endpoint, const char *soap_action, void *Req, struct cwmp__GetQueuedTransfersResponse *Resp);

SOAP_FMAC5 int SOAP_FMAC6 soap_call_cwmp__CancelTransfer(struct soap *soap, const char *soap_endpoint, const char *soap_action, char *CommandKey, struct cwmp__CancelTransferResponse *Resp);

/******************************************************************************\
 *                                                                            *
 * Skeletons                                                                  *
 *                                                                            *
\******************************************************************************/

SOAP_FMAC5 int SOAP_FMAC6 soap_serve(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_request(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_cwmp__GetRPCMethods(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_cwmp__Inform(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_cwmp__TransferComplete(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_cwmp__SetParameterValues(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_cwmp__GetParameterValues(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_cwmp__GetParameterNames(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_cwmp__SetParameterAttributes(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_cwmp__GetParameterAttributes(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_cwmp__AddObject(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_cwmp__DeleteObject(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_cwmp__Download(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_cwmp__Reboot(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_cwmp__Upload(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_cwmp__FactoryReset(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_cwmp__ScheduleInform(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_cwmp__GetAllQueuedTransfers(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_cwmp__GetQueuedTransfers(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_cwmp__CancelTransfer(struct soap*);

#ifdef __cplusplus
}
#endif

#endif

/* End of soapStub.h */
