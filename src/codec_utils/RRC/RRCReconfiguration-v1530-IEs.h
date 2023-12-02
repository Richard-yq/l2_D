/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/home/labadmin/hlal/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./15_3_rrc/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_RRCReconfiguration_v1530_IEs_H_
#define	_RRCReconfiguration_v1530_IEs_H_


#include <asn_application.h>

/* Including external dependencies */
#include <OCTET_STRING.h>
#include <NativeEnumerated.h>
#include "DedicatedNAS-Message.h"
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum RRCReconfiguration_v1530_IEs__fullConfig {
	RRCReconfiguration_v1530_IEs__fullConfig_true	= 0
} e_RRCReconfiguration_v1530_IEs__fullConfig;

/* Forward declarations */
struct MasterKeyUpdate;
struct OtherConfig;

/* RRCReconfiguration-v1530-IEs */
typedef struct RRCReconfiguration_v1530_IEs {
	OCTET_STRING_t	*masterCellGroup;	/* OPTIONAL */
	long	*fullConfig;	/* OPTIONAL */
	struct RRCReconfiguration_v1530_IEs__dedicatedNAS_MessageList {
		A_SEQUENCE_OF(DedicatedNAS_Message_t) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *dedicatedNAS_MessageList;
	struct MasterKeyUpdate	*masterKeyUpdate;	/* OPTIONAL */
	OCTET_STRING_t	*dedicatedSIB1_Delivery;	/* OPTIONAL */
	OCTET_STRING_t	*dedicatedSystemInformationDelivery;	/* OPTIONAL */
	struct OtherConfig	*otherConfig;	/* OPTIONAL */
	struct RRCReconfiguration_v1530_IEs__nonCriticalExtension {
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *nonCriticalExtension;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} RRCReconfiguration_v1530_IEs_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_fullConfig_3;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_RRCReconfiguration_v1530_IEs;
extern asn_SEQUENCE_specifics_t asn_SPC_RRCReconfiguration_v1530_IEs_specs_1;
extern asn_TYPE_member_t asn_MBR_RRCReconfiguration_v1530_IEs_1[8];

#ifdef __cplusplus
}
#endif

#endif	/* _RRCReconfiguration_v1530_IEs_H_ */
#include <asn_internal.h>
