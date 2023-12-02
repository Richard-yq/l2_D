/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/home/labadmin/hlal/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./15_3_rrc/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#ifndef	_FilterCoefficient_H_
#define	_FilterCoefficient_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum FilterCoefficient {
	FilterCoefficient_fc0	= 0,
	FilterCoefficient_fc1	= 1,
	FilterCoefficient_fc2	= 2,
	FilterCoefficient_fc3	= 3,
	FilterCoefficient_fc4	= 4,
	FilterCoefficient_fc5	= 5,
	FilterCoefficient_fc6	= 6,
	FilterCoefficient_fc7	= 7,
	FilterCoefficient_fc8	= 8,
	FilterCoefficient_fc9	= 9,
	FilterCoefficient_fc11	= 10,
	FilterCoefficient_fc13	= 11,
	FilterCoefficient_fc15	= 12,
	FilterCoefficient_fc17	= 13,
	FilterCoefficient_fc19	= 14,
	FilterCoefficient_spare1	= 15
	/*
	 * Enumeration is extensible
	 */
} e_FilterCoefficient;

/* FilterCoefficient */
typedef long	 FilterCoefficient_t;

/* Implementation */
extern asn_per_constraints_t asn_PER_type_FilterCoefficient_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_FilterCoefficient;
extern const asn_INTEGER_specifics_t asn_SPC_FilterCoefficient_specs_1;
asn_struct_free_f FilterCoefficient_free;
asn_struct_print_f FilterCoefficient_print;
asn_constr_check_f FilterCoefficient_constraint;
ber_type_decoder_f FilterCoefficient_decode_ber;
der_type_encoder_f FilterCoefficient_encode_der;
xer_type_decoder_f FilterCoefficient_decode_xer;
xer_type_encoder_f FilterCoefficient_encode_xer;
oer_type_decoder_f FilterCoefficient_decode_oer;
oer_type_encoder_f FilterCoefficient_encode_oer;
per_type_decoder_f FilterCoefficient_decode_uper;
per_type_encoder_f FilterCoefficient_encode_uper;
per_type_decoder_f FilterCoefficient_decode_aper;
per_type_encoder_f FilterCoefficient_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	/* _FilterCoefficient_H_ */
#include <asn_internal.h>
