/* header auto-generated by pidl */

#include "librpc/ndr/libndr.h"
#include "librpc/gen_ndr/frstrans.h"

#ifndef _HEADER_NDR_frstrans
#define _HEADER_NDR_frstrans

#define NDR_FRSTRANS_UUID "897e2e5f-93f3-4376-9c9c-fd2277495c27"
#define NDR_FRSTRANS_VERSION 1.0
#define NDR_FRSTRANS_NAME "frstrans"
#define NDR_FRSTRANS_HELPSTRING "File Replication Service DFS-R"
extern const struct ndr_interface_table ndr_table_frstrans;
#define NDR_FRSTRANS_CHECKCONNECTIVITY (0x00)

#define NDR_FRSTRANS_ESTABLISHCONNECTION (0x01)

#define NDR_FRSTRANS_ESTABLISHSESSION (0x02)

#define NDR_FRSTRANS_REQUESTUPDATES (0x03)

#define NDR_FRSTRANS_REQUESTVERSIONVECTOR (0x04)

#define NDR_FRSTRANS_ASYNCPOLL (0x05)

#define NDR_FRSTRANS_REQUEST_RECORDS (0x06)

#define NDR_FRSTRANS_UPDATE_CANCEL (0x07)

#define NDR_FRSTRANS_RAW_GET_FILE_DATA (0x08)

#define NDR_FRSTRANS_RDC_GET_SIGNATURES (0x09)

#define NDR_FRSTRANS_RDC_PUSH_SOURCE_NEEDS (0x0a)

#define NDR_FRSTRANS_RDC_GET_FILE_DATA (0x0b)

#define NDR_FRSTRANS_RDC_CLOSE (0x0c)

#define NDR_FRSTRANS_INITIALIZEFILETRANSFERASYNC (0x0d)

#define NDR_FRSTRANS_OPNUM_0E_NOT_USED_ON_THE_WIRE (0x0e)

#define NDR_FRSTRANS_RAWGETFILEDATAASYNC (0x0f)

#define NDR_FRSTRANS_RDCGETFILEDATAASYNC (0x10)

#define NDR_FRSTRANS_CALL_COUNT (17)
void ndr_print_frstrans_ProtocolVersion(struct ndr_print *ndr, const char *name, enum frstrans_ProtocolVersion r);
void ndr_print_frstrans_TransportFlags(struct ndr_print *ndr, const char *name, uint32_t r);
void ndr_print_frstrans_UpdateRequestType(struct ndr_print *ndr, const char *name, enum frstrans_UpdateRequestType r);
void ndr_print_frstrans_UpdateStatus(struct ndr_print *ndr, const char *name, enum frstrans_UpdateStatus r);
void ndr_print_frstrans_VersionVector(struct ndr_print *ndr, const char *name, const struct frstrans_VersionVector *r);
void ndr_print_frstrans_Update(struct ndr_print *ndr, const char *name, const struct frstrans_Update *r);
void ndr_print_frstrans_VersionRequestType(struct ndr_print *ndr, const char *name, enum frstrans_VersionRequestType r);
void ndr_print_frstrans_VersionChangeType(struct ndr_print *ndr, const char *name, enum frstrans_VersionChangeType r);
void ndr_print_frstrans_EpoqueVector(struct ndr_print *ndr, const char *name, const struct frstrans_EpoqueVector *r);
void ndr_print_frstrans_AsyncVersionVectorResponse(struct ndr_print *ndr, const char *name, const struct frstrans_AsyncVersionVectorResponse *r);
void ndr_print_frstrans_AsyncResponseContext(struct ndr_print *ndr, const char *name, const struct frstrans_AsyncResponseContext *r);
void ndr_print_frstrans_RequestedStagingPolicy(struct ndr_print *ndr, const char *name, enum frstrans_RequestedStagingPolicy r);
void ndr_print_frstrans_RdcChunckerAlgorithm(struct ndr_print *ndr, const char *name, enum frstrans_RdcChunckerAlgorithm r);
void ndr_print_frstrans_RdcParameterGeneric(struct ndr_print *ndr, const char *name, const struct frstrans_RdcParameterGeneric *r);
void ndr_print_frstrans_RdcParameterFilterMax(struct ndr_print *ndr, const char *name, const struct frstrans_RdcParameterFilterMax *r);
void ndr_print_frstrans_RdcParameterFilterPoint(struct ndr_print *ndr, const char *name, const struct frstrans_RdcParameterFilterPoint *r);
void ndr_print_frstrans_RdcParameterUnion(struct ndr_print *ndr, const char *name, const union frstrans_RdcParameterUnion *r);
void ndr_print_frstrans_RdcParameters(struct ndr_print *ndr, const char *name, const struct frstrans_RdcParameters *r);
void ndr_print_frstrans_RdcVersion(struct ndr_print *ndr, const char *name, enum frstrans_RdcVersion r);
void ndr_print_frstrans_RdcVersionCompatible(struct ndr_print *ndr, const char *name, enum frstrans_RdcVersionCompatible r);
void ndr_print_frstrans_RdcCompressionAlgorithm(struct ndr_print *ndr, const char *name, enum frstrans_RdcCompressionAlgorithm r);
void ndr_print_frstrans_RdcFileInfo(struct ndr_print *ndr, const char *name, const struct frstrans_RdcFileInfo *r);
void ndr_print_frstrans_BytePipe_chunk(struct ndr_print *ndr, const char *name, const struct frstrans_BytePipe_chunk *r);
void ndr_print_frstrans_CheckConnectivity(struct ndr_print *ndr, const char *name, int flags, const struct frstrans_CheckConnectivity *r);
void ndr_print_frstrans_EstablishConnection(struct ndr_print *ndr, const char *name, int flags, const struct frstrans_EstablishConnection *r);
void ndr_print_frstrans_EstablishSession(struct ndr_print *ndr, const char *name, int flags, const struct frstrans_EstablishSession *r);
void ndr_print_frstrans_RequestUpdates(struct ndr_print *ndr, const char *name, int flags, const struct frstrans_RequestUpdates *r);
void ndr_print_frstrans_RequestVersionVector(struct ndr_print *ndr, const char *name, int flags, const struct frstrans_RequestVersionVector *r);
void ndr_print_frstrans_AsyncPoll(struct ndr_print *ndr, const char *name, int flags, const struct frstrans_AsyncPoll *r);
void ndr_print_FRSTRANS_REQUEST_RECORDS(struct ndr_print *ndr, const char *name, int flags, const struct FRSTRANS_REQUEST_RECORDS *r);
void ndr_print_FRSTRANS_UPDATE_CANCEL(struct ndr_print *ndr, const char *name, int flags, const struct FRSTRANS_UPDATE_CANCEL *r);
void ndr_print_FRSTRANS_RAW_GET_FILE_DATA(struct ndr_print *ndr, const char *name, int flags, const struct FRSTRANS_RAW_GET_FILE_DATA *r);
void ndr_print_FRSTRANS_RDC_GET_SIGNATURES(struct ndr_print *ndr, const char *name, int flags, const struct FRSTRANS_RDC_GET_SIGNATURES *r);
void ndr_print_FRSTRANS_RDC_PUSH_SOURCE_NEEDS(struct ndr_print *ndr, const char *name, int flags, const struct FRSTRANS_RDC_PUSH_SOURCE_NEEDS *r);
void ndr_print_FRSTRANS_RDC_GET_FILE_DATA(struct ndr_print *ndr, const char *name, int flags, const struct FRSTRANS_RDC_GET_FILE_DATA *r);
void ndr_print_FRSTRANS_RDC_CLOSE(struct ndr_print *ndr, const char *name, int flags, const struct FRSTRANS_RDC_CLOSE *r);
void ndr_print_frstrans_InitializeFileTransferAsync(struct ndr_print *ndr, const char *name, int flags, const struct frstrans_InitializeFileTransferAsync *r);
void ndr_print_FRSTRANS_OPNUM_0E_NOT_USED_ON_THE_WIRE(struct ndr_print *ndr, const char *name, int flags, const struct FRSTRANS_OPNUM_0E_NOT_USED_ON_THE_WIRE *r);
void ndr_print_frstrans_RawGetFileDataAsync(struct ndr_print *ndr, const char *name, int flags, const struct frstrans_RawGetFileDataAsync *r);
void ndr_print_frstrans_RdcGetFileDataAsync(struct ndr_print *ndr, const char *name, int flags, const struct frstrans_RdcGetFileDataAsync *r);
#endif /* _HEADER_NDR_frstrans */
