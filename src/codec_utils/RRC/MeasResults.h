/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/home/labadmin/hlal/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./15_3_rrc/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_MeasResults_H_
#define	_MeasResults_H_


#include <asn_application.h>

/* Including external dependencies */
#include "MeasId.h"
#include "MeasResultServMOList.h"
#include <constr_CHOICE.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum MeasResults__measResultNeighCells_PR {
	MeasResults__measResultNeighCells_PR_NOTHING,	/* No components present */
	MeasResults__measResultNeighCells_PR_measResultListNR,
	/* Extensions may appear below */
	MeasResults__measResultNeighCells_PR_measResultListEUTRA
} MeasResults__measResultNeighCells_PR;

/* Forward declarations */
struct MeasResultListNR;
struct MeasResultListEUTRA;

/* MeasResults */
typedef struct MeasResults {
	MeasId_t	 measId;
	MeasResultServMOList_t	 measResultServingMOList;
	struct MeasResults__measResultNeighCells {
		MeasResults__measResultNeighCells_PR present;
		union MeasResults__measResultNeighCells_u {
			struct MeasResultListNR	*measResultListNR;
			/*
			 * This type is extensible,
			 * possible extensions are below.
			 */
			struct MeasResultListEUTRA	*measResultListEUTRA;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *measResultNeighCells;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} MeasResults_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_MeasResults;
extern asn_SEQUENCE_specifics_t asn_SPC_MeasResults_specs_1;
extern asn_TYPE_member_t asn_MBR_MeasResults_1[3];

#ifdef __cplusplus
}
#endif

#endif	/* _MeasResults_H_ */
#include <asn_internal.h>
