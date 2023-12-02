/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/home/labadmin/hlal/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./15_3_rrc/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_Phy_ParametersFR2_H_
#define	_Phy_ParametersFR2_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum Phy_ParametersFR2__calibrationGapPA {
	Phy_ParametersFR2__calibrationGapPA_supported	= 0
} e_Phy_ParametersFR2__calibrationGapPA;
typedef enum Phy_ParametersFR2__pdsch_RE_MappingFR2 {
	Phy_ParametersFR2__pdsch_RE_MappingFR2_n6	= 0,
	Phy_ParametersFR2__pdsch_RE_MappingFR2_n20	= 1
} e_Phy_ParametersFR2__pdsch_RE_MappingFR2;

/* Phy-ParametersFR2 */
typedef struct Phy_ParametersFR2 {
	long	*calibrationGapPA;	/* OPTIONAL */
	long	*pdsch_RE_MappingFR2;	/* OPTIONAL */
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} Phy_ParametersFR2_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_calibrationGapPA_2;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_pdsch_RE_MappingFR2_4;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_Phy_ParametersFR2;
extern asn_SEQUENCE_specifics_t asn_SPC_Phy_ParametersFR2_specs_1;
extern asn_TYPE_member_t asn_MBR_Phy_ParametersFR2_1[2];

#ifdef __cplusplus
}
#endif

#endif	/* _Phy_ParametersFR2_H_ */
#include <asn_internal.h>
