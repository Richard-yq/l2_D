/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/home/labadmin/hlal/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./15_3_rrc/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_PUSCH_Config_H_
#define	_PUSCH_Config_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeInteger.h>
#include <NativeEnumerated.h>
#include <NULL.h>
#include <constr_CHOICE.h>
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum PUSCH_Config__txConfig {
	PUSCH_Config__txConfig_codebook	= 0,
	PUSCH_Config__txConfig_nonCodebook	= 1
} e_PUSCH_Config__txConfig;
typedef enum PUSCH_Config__dmrs_UplinkForPUSCH_MappingTypeA_PR {
	PUSCH_Config__dmrs_UplinkForPUSCH_MappingTypeA_PR_NOTHING,	/* No components present */
	PUSCH_Config__dmrs_UplinkForPUSCH_MappingTypeA_PR_release,
	PUSCH_Config__dmrs_UplinkForPUSCH_MappingTypeA_PR_setup
} PUSCH_Config__dmrs_UplinkForPUSCH_MappingTypeA_PR;
typedef enum PUSCH_Config__dmrs_UplinkForPUSCH_MappingTypeB_PR {
	PUSCH_Config__dmrs_UplinkForPUSCH_MappingTypeB_PR_NOTHING,	/* No components present */
	PUSCH_Config__dmrs_UplinkForPUSCH_MappingTypeB_PR_release,
	PUSCH_Config__dmrs_UplinkForPUSCH_MappingTypeB_PR_setup
} PUSCH_Config__dmrs_UplinkForPUSCH_MappingTypeB_PR;
typedef enum PUSCH_Config__frequencyHopping {
	PUSCH_Config__frequencyHopping_intraSlot	= 0,
	PUSCH_Config__frequencyHopping_interSlot	= 1
} e_PUSCH_Config__frequencyHopping;
typedef enum PUSCH_Config__resourceAllocation {
	PUSCH_Config__resourceAllocation_resourceAllocationType0	= 0,
	PUSCH_Config__resourceAllocation_resourceAllocationType1	= 1,
	PUSCH_Config__resourceAllocation_dynamicSwitch	= 2
} e_PUSCH_Config__resourceAllocation;
typedef enum PUSCH_Config__pusch_TimeDomainAllocationList_PR {
	PUSCH_Config__pusch_TimeDomainAllocationList_PR_NOTHING,	/* No components present */
	PUSCH_Config__pusch_TimeDomainAllocationList_PR_release,
	PUSCH_Config__pusch_TimeDomainAllocationList_PR_setup
} PUSCH_Config__pusch_TimeDomainAllocationList_PR;
typedef enum PUSCH_Config__pusch_AggregationFactor {
	PUSCH_Config__pusch_AggregationFactor_n2	= 0,
	PUSCH_Config__pusch_AggregationFactor_n4	= 1,
	PUSCH_Config__pusch_AggregationFactor_n8	= 2
} e_PUSCH_Config__pusch_AggregationFactor;
typedef enum PUSCH_Config__mcs_Table {
	PUSCH_Config__mcs_Table_qam256	= 0,
	PUSCH_Config__mcs_Table_qam64LowSE	= 1
} e_PUSCH_Config__mcs_Table;
typedef enum PUSCH_Config__mcs_TableTransformPrecoder {
	PUSCH_Config__mcs_TableTransformPrecoder_qam256	= 0,
	PUSCH_Config__mcs_TableTransformPrecoder_qam64LowSE	= 1
} e_PUSCH_Config__mcs_TableTransformPrecoder;
typedef enum PUSCH_Config__transformPrecoder {
	PUSCH_Config__transformPrecoder_enabled	= 0,
	PUSCH_Config__transformPrecoder_disabled	= 1
} e_PUSCH_Config__transformPrecoder;
typedef enum PUSCH_Config__codebookSubset {
	PUSCH_Config__codebookSubset_fullyAndPartialAndNonCoherent	= 0,
	PUSCH_Config__codebookSubset_partialAndNonCoherent	= 1,
	PUSCH_Config__codebookSubset_nonCoherent	= 2
} e_PUSCH_Config__codebookSubset;
typedef enum PUSCH_Config__rbg_Size {
	PUSCH_Config__rbg_Size_config2	= 0
} e_PUSCH_Config__rbg_Size;
typedef enum PUSCH_Config__uci_OnPUSCH_PR {
	PUSCH_Config__uci_OnPUSCH_PR_NOTHING,	/* No components present */
	PUSCH_Config__uci_OnPUSCH_PR_release,
	PUSCH_Config__uci_OnPUSCH_PR_setup
} PUSCH_Config__uci_OnPUSCH_PR;
typedef enum PUSCH_Config__tp_pi2BPSK {
	PUSCH_Config__tp_pi2BPSK_enabled	= 0
} e_PUSCH_Config__tp_pi2BPSK;

/* Forward declarations */
struct PUSCH_PowerControl;
struct DMRS_UplinkConfig;
struct PUSCH_TimeDomainResourceAllocationList;
struct UCI_OnPUSCH;

/* PUSCH-Config */
typedef struct PUSCH_Config {
	long	*dataScramblingIdentityPUSCH;	/* OPTIONAL */
	long	*txConfig;	/* OPTIONAL */
	struct PUSCH_Config__dmrs_UplinkForPUSCH_MappingTypeA {
		PUSCH_Config__dmrs_UplinkForPUSCH_MappingTypeA_PR present;
		union PUSCH_Config__dmrs_UplinkForPUSCH_MappingTypeA_u {
			NULL_t	 release;
			struct DMRS_UplinkConfig	*setup;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *dmrs_UplinkForPUSCH_MappingTypeA;
	struct PUSCH_Config__dmrs_UplinkForPUSCH_MappingTypeB {
		PUSCH_Config__dmrs_UplinkForPUSCH_MappingTypeB_PR present;
		union PUSCH_Config__dmrs_UplinkForPUSCH_MappingTypeB_u {
			NULL_t	 release;
			struct DMRS_UplinkConfig	*setup;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *dmrs_UplinkForPUSCH_MappingTypeB;
	struct PUSCH_PowerControl	*pusch_PowerControl;	/* OPTIONAL */
	long	*frequencyHopping;	/* OPTIONAL */
	struct PUSCH_Config__frequencyHoppingOffsetLists {
		A_SEQUENCE_OF(long) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *frequencyHoppingOffsetLists;
	long	 resourceAllocation;
	struct PUSCH_Config__pusch_TimeDomainAllocationList {
		PUSCH_Config__pusch_TimeDomainAllocationList_PR present;
		union PUSCH_Config__pusch_TimeDomainAllocationList_u {
			NULL_t	 release;
			struct PUSCH_TimeDomainResourceAllocationList	*setup;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *pusch_TimeDomainAllocationList;
	long	*pusch_AggregationFactor;	/* OPTIONAL */
	long	*mcs_Table;	/* OPTIONAL */
	long	*mcs_TableTransformPrecoder;	/* OPTIONAL */
	long	*transformPrecoder;	/* OPTIONAL */
	long	*codebookSubset;	/* OPTIONAL */
	long	*maxRank;	/* OPTIONAL */
	long	*rbg_Size;	/* OPTIONAL */
	struct PUSCH_Config__uci_OnPUSCH {
		PUSCH_Config__uci_OnPUSCH_PR present;
		union PUSCH_Config__uci_OnPUSCH_u {
			NULL_t	 release;
			struct UCI_OnPUSCH	*setup;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *uci_OnPUSCH;
	long	*tp_pi2BPSK;	/* OPTIONAL */
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} PUSCH_Config_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_txConfig_3;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_frequencyHopping_13;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_resourceAllocation_18;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_pusch_AggregationFactor_25;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_mcs_Table_29;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_mcs_TableTransformPrecoder_32;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_transformPrecoder_35;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_codebookSubset_38;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_rbg_Size_43;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_tp_pi2BPSK_48;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_PUSCH_Config;
extern asn_SEQUENCE_specifics_t asn_SPC_PUSCH_Config_specs_1;
extern asn_TYPE_member_t asn_MBR_PUSCH_Config_1[18];

#ifdef __cplusplus
}
#endif

#endif	/* _PUSCH_Config_H_ */
#include <asn_internal.h>
