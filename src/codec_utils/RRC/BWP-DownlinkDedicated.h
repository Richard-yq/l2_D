/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/home/labadmin/hlal/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./15_3_rrc/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_BWP_DownlinkDedicated_H_
#define	_BWP_DownlinkDedicated_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NULL.h>
#include <constr_CHOICE.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum BWP_DownlinkDedicated__pdcch_Config_PR {
	BWP_DownlinkDedicated__pdcch_Config_PR_NOTHING,	/* No components present */
	BWP_DownlinkDedicated__pdcch_Config_PR_release,
	BWP_DownlinkDedicated__pdcch_Config_PR_setup
} BWP_DownlinkDedicated__pdcch_Config_PR;
typedef enum BWP_DownlinkDedicated__pdsch_Config_PR {
	BWP_DownlinkDedicated__pdsch_Config_PR_NOTHING,	/* No components present */
	BWP_DownlinkDedicated__pdsch_Config_PR_release,
	BWP_DownlinkDedicated__pdsch_Config_PR_setup
} BWP_DownlinkDedicated__pdsch_Config_PR;
typedef enum BWP_DownlinkDedicated__sps_Config_PR {
	BWP_DownlinkDedicated__sps_Config_PR_NOTHING,	/* No components present */
	BWP_DownlinkDedicated__sps_Config_PR_release,
	BWP_DownlinkDedicated__sps_Config_PR_setup
} BWP_DownlinkDedicated__sps_Config_PR;
typedef enum BWP_DownlinkDedicated__radioLinkMonitoringConfig_PR {
	BWP_DownlinkDedicated__radioLinkMonitoringConfig_PR_NOTHING,	/* No components present */
	BWP_DownlinkDedicated__radioLinkMonitoringConfig_PR_release,
	BWP_DownlinkDedicated__radioLinkMonitoringConfig_PR_setup
} BWP_DownlinkDedicated__radioLinkMonitoringConfig_PR;

/* Forward declarations */
struct PDCCH_Config;
struct PDSCH_Config;
struct SPS_Config;
struct RadioLinkMonitoringConfig;

/* BWP-DownlinkDedicated */
typedef struct BWP_DownlinkDedicated {
	struct BWP_DownlinkDedicated__pdcch_Config {
		BWP_DownlinkDedicated__pdcch_Config_PR present;
		union BWP_DownlinkDedicated__pdcch_Config_u {
			NULL_t	 release;
			struct PDCCH_Config	*setup;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *pdcch_Config;
	struct BWP_DownlinkDedicated__pdsch_Config {
		BWP_DownlinkDedicated__pdsch_Config_PR present;
		union BWP_DownlinkDedicated__pdsch_Config_u {
			NULL_t	 release;
			struct PDSCH_Config	*setup;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *pdsch_Config;
	struct BWP_DownlinkDedicated__sps_Config {
		BWP_DownlinkDedicated__sps_Config_PR present;
		union BWP_DownlinkDedicated__sps_Config_u {
			NULL_t	 release;
			struct SPS_Config	*setup;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *sps_Config;
	struct BWP_DownlinkDedicated__radioLinkMonitoringConfig {
		BWP_DownlinkDedicated__radioLinkMonitoringConfig_PR present;
		union BWP_DownlinkDedicated__radioLinkMonitoringConfig_u {
			NULL_t	 release;
			struct RadioLinkMonitoringConfig	*setup;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *radioLinkMonitoringConfig;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} BWP_DownlinkDedicated_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_BWP_DownlinkDedicated;
extern asn_SEQUENCE_specifics_t asn_SPC_BWP_DownlinkDedicated_specs_1;
extern asn_TYPE_member_t asn_MBR_BWP_DownlinkDedicated_1[4];

#ifdef __cplusplus
}
#endif

#endif	/* _BWP_DownlinkDedicated_H_ */
#include <asn_internal.h>
