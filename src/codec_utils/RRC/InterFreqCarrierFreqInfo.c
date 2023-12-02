/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "/home/labadmin/hlal/rrc_15.3_asn.asn1"
 * 	`asn1c -D ./15_3_rrc/ -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-example`
 */

#include "InterFreqCarrierFreqInfo.h"

#include "MultiFrequencyBandListNR-SIB.h"
#include "ThresholdNR.h"
#include "SSB-MTC.h"
#include "SSB-ToMeasure.h"
#include "SS-RSSI-Measurement.h"
#include "SpeedStateScaleFactors.h"
#include "InterFreqNeighCellList.h"
#include "InterFreqBlackCellList.h"
static int
memb_nrofSS_BlocksToAverage_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 2 && value <= 16)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static asn_oer_constraints_t asn_OER_memb_nrofSS_BlocksToAverage_constr_5 CC_NOTUSED = {
	{ 1, 1 }	/* (2..16) */,
	-1};
static asn_per_constraints_t asn_PER_memb_nrofSS_BlocksToAverage_constr_5 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 4,  4,  2,  16 }	/* (2..16) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_TYPE_member_t asn_MBR_threshX_Q_20[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct InterFreqCarrierFreqInfo__threshX_Q, threshX_HighQ),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ReselectionThresholdQ,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"threshX-HighQ"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct InterFreqCarrierFreqInfo__threshX_Q, threshX_LowQ),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ReselectionThresholdQ,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"threshX-LowQ"
		},
};
static const ber_tlv_tag_t asn_DEF_threshX_Q_tags_20[] = {
	(ASN_TAG_CLASS_CONTEXT | (18 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_threshX_Q_tag2el_20[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* threshX-HighQ */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* threshX-LowQ */
};
static asn_SEQUENCE_specifics_t asn_SPC_threshX_Q_specs_20 = {
	sizeof(struct InterFreqCarrierFreqInfo__threshX_Q),
	offsetof(struct InterFreqCarrierFreqInfo__threshX_Q, _asn_ctx),
	asn_MAP_threshX_Q_tag2el_20,
	2,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	-1,	/* First extension addition */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_threshX_Q_20 = {
	"threshX-Q",
	"threshX-Q",
	&asn_OP_SEQUENCE,
	asn_DEF_threshX_Q_tags_20,
	sizeof(asn_DEF_threshX_Q_tags_20)
		/sizeof(asn_DEF_threshX_Q_tags_20[0]) - 1, /* 1 */
	asn_DEF_threshX_Q_tags_20,	/* Same as above */
	sizeof(asn_DEF_threshX_Q_tags_20)
		/sizeof(asn_DEF_threshX_Q_tags_20[0]), /* 2 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_threshX_Q_20,
	2,	/* Elements count */
	&asn_SPC_threshX_Q_specs_20	/* Additional specs */
};

static int asn_DFL_25_cmp_15(const void *sptr) {
	const Q_OffsetRange_t *st = sptr;
	
	if(!st) {
		return -1; /* No value is not a default value */
	}
	
	/* Test default value 15 */
	return (*st != 15);
}
static int asn_DFL_25_set_15(void **sptr) {
	Q_OffsetRange_t *st = *sptr;
	
	if(!st) {
		st = (*sptr = CALLOC(1, sizeof(*st)));
		if(!st) return -1;
	}
	
	/* Install default value 15 */
	*st = 15;
	return 0;
}
asn_TYPE_member_t asn_MBR_InterFreqCarrierFreqInfo_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct InterFreqCarrierFreqInfo, dl_CarrierFreq),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ARFCN_ValueNR,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"dl-CarrierFreq"
		},
	{ ATF_POINTER, 5, offsetof(struct InterFreqCarrierFreqInfo, frequencyBandList),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_MultiFrequencyBandListNR_SIB,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"frequencyBandList"
		},
	{ ATF_POINTER, 4, offsetof(struct InterFreqCarrierFreqInfo, frequencyBandListSUL),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_MultiFrequencyBandListNR_SIB,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"frequencyBandListSUL"
		},
	{ ATF_POINTER, 3, offsetof(struct InterFreqCarrierFreqInfo, nrofSS_BlocksToAverage),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		0,
		{ &asn_OER_memb_nrofSS_BlocksToAverage_constr_5, &asn_PER_memb_nrofSS_BlocksToAverage_constr_5,  memb_nrofSS_BlocksToAverage_constraint_1 },
		0, 0, /* No default value */
		"nrofSS-BlocksToAverage"
		},
	{ ATF_POINTER, 2, offsetof(struct InterFreqCarrierFreqInfo, absThreshSS_BlocksConsolidation),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ThresholdNR,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"absThreshSS-BlocksConsolidation"
		},
	{ ATF_POINTER, 1, offsetof(struct InterFreqCarrierFreqInfo, smtc),
		(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_SSB_MTC,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"smtc"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct InterFreqCarrierFreqInfo, ssbSubcarrierSpacing),
		(ASN_TAG_CLASS_CONTEXT | (6 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_SubcarrierSpacing,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"ssbSubcarrierSpacing"
		},
	{ ATF_POINTER, 1, offsetof(struct InterFreqCarrierFreqInfo, ssb_ToMeasure),
		(ASN_TAG_CLASS_CONTEXT | (7 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_SSB_ToMeasure,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"ssb-ToMeasure"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct InterFreqCarrierFreqInfo, deriveSSB_IndexFromCell),
		(ASN_TAG_CLASS_CONTEXT | (8 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_BOOLEAN,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"deriveSSB-IndexFromCell"
		},
	{ ATF_POINTER, 1, offsetof(struct InterFreqCarrierFreqInfo, ss_RSSI_Measurement),
		(ASN_TAG_CLASS_CONTEXT | (9 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_SS_RSSI_Measurement,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"ss-RSSI-Measurement"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct InterFreqCarrierFreqInfo, q_RxLevMin),
		(ASN_TAG_CLASS_CONTEXT | (10 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Q_RxLevMin,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"q-RxLevMin"
		},
	{ ATF_POINTER, 3, offsetof(struct InterFreqCarrierFreqInfo, q_RxLevMinSUL),
		(ASN_TAG_CLASS_CONTEXT | (11 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Q_RxLevMin,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"q-RxLevMinSUL"
		},
	{ ATF_POINTER, 2, offsetof(struct InterFreqCarrierFreqInfo, q_QualMin),
		(ASN_TAG_CLASS_CONTEXT | (12 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Q_QualMin,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"q-QualMin"
		},
	{ ATF_POINTER, 1, offsetof(struct InterFreqCarrierFreqInfo, p_Max),
		(ASN_TAG_CLASS_CONTEXT | (13 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_P_Max,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"p-Max"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct InterFreqCarrierFreqInfo, t_ReselectionNR),
		(ASN_TAG_CLASS_CONTEXT | (14 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_T_Reselection,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"t-ReselectionNR"
		},
	{ ATF_POINTER, 1, offsetof(struct InterFreqCarrierFreqInfo, t_ReselectionNR_SF),
		(ASN_TAG_CLASS_CONTEXT | (15 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_SpeedStateScaleFactors,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"t-ReselectionNR-SF"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct InterFreqCarrierFreqInfo, threshX_HighP),
		(ASN_TAG_CLASS_CONTEXT | (16 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ReselectionThreshold,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"threshX-HighP"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct InterFreqCarrierFreqInfo, threshX_LowP),
		(ASN_TAG_CLASS_CONTEXT | (17 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ReselectionThreshold,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"threshX-LowP"
		},
	{ ATF_POINTER, 6, offsetof(struct InterFreqCarrierFreqInfo, threshX_Q),
		(ASN_TAG_CLASS_CONTEXT | (18 << 2)),
		0,
		&asn_DEF_threshX_Q_20,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"threshX-Q"
		},
	{ ATF_POINTER, 5, offsetof(struct InterFreqCarrierFreqInfo, cellReselectionPriority),
		(ASN_TAG_CLASS_CONTEXT | (19 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_CellReselectionPriority,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"cellReselectionPriority"
		},
	{ ATF_POINTER, 4, offsetof(struct InterFreqCarrierFreqInfo, cellReselectionSubPriority),
		(ASN_TAG_CLASS_CONTEXT | (20 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_CellReselectionSubPriority,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"cellReselectionSubPriority"
		},
	{ ATF_POINTER, 3, offsetof(struct InterFreqCarrierFreqInfo, q_OffsetFreq),
		(ASN_TAG_CLASS_CONTEXT | (21 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Q_OffsetRange,
		0,
		{ 0, 0, 0 },
		&asn_DFL_25_cmp_15,	/* Compare DEFAULT 15 */
		&asn_DFL_25_set_15,	/* Set DEFAULT 15 */
		"q-OffsetFreq"
		},
	{ ATF_POINTER, 2, offsetof(struct InterFreqCarrierFreqInfo, interFreqNeighCellList),
		(ASN_TAG_CLASS_CONTEXT | (22 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_InterFreqNeighCellList,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"interFreqNeighCellList"
		},
	{ ATF_POINTER, 1, offsetof(struct InterFreqCarrierFreqInfo, interFreqBlackCellList),
		(ASN_TAG_CLASS_CONTEXT | (23 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_InterFreqBlackCellList,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"interFreqBlackCellList"
		},
};
static const int asn_MAP_InterFreqCarrierFreqInfo_oms_1[] = { 1, 2, 3, 4, 5, 7, 9, 11, 12, 13, 15, 18, 19, 20, 21, 22, 23 };
static const ber_tlv_tag_t asn_DEF_InterFreqCarrierFreqInfo_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_InterFreqCarrierFreqInfo_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* dl-CarrierFreq */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* frequencyBandList */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* frequencyBandListSUL */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* nrofSS-BlocksToAverage */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 }, /* absThreshSS-BlocksConsolidation */
    { (ASN_TAG_CLASS_CONTEXT | (5 << 2)), 5, 0, 0 }, /* smtc */
    { (ASN_TAG_CLASS_CONTEXT | (6 << 2)), 6, 0, 0 }, /* ssbSubcarrierSpacing */
    { (ASN_TAG_CLASS_CONTEXT | (7 << 2)), 7, 0, 0 }, /* ssb-ToMeasure */
    { (ASN_TAG_CLASS_CONTEXT | (8 << 2)), 8, 0, 0 }, /* deriveSSB-IndexFromCell */
    { (ASN_TAG_CLASS_CONTEXT | (9 << 2)), 9, 0, 0 }, /* ss-RSSI-Measurement */
    { (ASN_TAG_CLASS_CONTEXT | (10 << 2)), 10, 0, 0 }, /* q-RxLevMin */
    { (ASN_TAG_CLASS_CONTEXT | (11 << 2)), 11, 0, 0 }, /* q-RxLevMinSUL */
    { (ASN_TAG_CLASS_CONTEXT | (12 << 2)), 12, 0, 0 }, /* q-QualMin */
    { (ASN_TAG_CLASS_CONTEXT | (13 << 2)), 13, 0, 0 }, /* p-Max */
    { (ASN_TAG_CLASS_CONTEXT | (14 << 2)), 14, 0, 0 }, /* t-ReselectionNR */
    { (ASN_TAG_CLASS_CONTEXT | (15 << 2)), 15, 0, 0 }, /* t-ReselectionNR-SF */
    { (ASN_TAG_CLASS_CONTEXT | (16 << 2)), 16, 0, 0 }, /* threshX-HighP */
    { (ASN_TAG_CLASS_CONTEXT | (17 << 2)), 17, 0, 0 }, /* threshX-LowP */
    { (ASN_TAG_CLASS_CONTEXT | (18 << 2)), 18, 0, 0 }, /* threshX-Q */
    { (ASN_TAG_CLASS_CONTEXT | (19 << 2)), 19, 0, 0 }, /* cellReselectionPriority */
    { (ASN_TAG_CLASS_CONTEXT | (20 << 2)), 20, 0, 0 }, /* cellReselectionSubPriority */
    { (ASN_TAG_CLASS_CONTEXT | (21 << 2)), 21, 0, 0 }, /* q-OffsetFreq */
    { (ASN_TAG_CLASS_CONTEXT | (22 << 2)), 22, 0, 0 }, /* interFreqNeighCellList */
    { (ASN_TAG_CLASS_CONTEXT | (23 << 2)), 23, 0, 0 } /* interFreqBlackCellList */
};
asn_SEQUENCE_specifics_t asn_SPC_InterFreqCarrierFreqInfo_specs_1 = {
	sizeof(struct InterFreqCarrierFreqInfo),
	offsetof(struct InterFreqCarrierFreqInfo, _asn_ctx),
	asn_MAP_InterFreqCarrierFreqInfo_tag2el_1,
	24,	/* Count of tags in the map */
	asn_MAP_InterFreqCarrierFreqInfo_oms_1,	/* Optional members */
	17, 0,	/* Root/Additions */
	24,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_InterFreqCarrierFreqInfo = {
	"InterFreqCarrierFreqInfo",
	"InterFreqCarrierFreqInfo",
	&asn_OP_SEQUENCE,
	asn_DEF_InterFreqCarrierFreqInfo_tags_1,
	sizeof(asn_DEF_InterFreqCarrierFreqInfo_tags_1)
		/sizeof(asn_DEF_InterFreqCarrierFreqInfo_tags_1[0]), /* 1 */
	asn_DEF_InterFreqCarrierFreqInfo_tags_1,	/* Same as above */
	sizeof(asn_DEF_InterFreqCarrierFreqInfo_tags_1)
		/sizeof(asn_DEF_InterFreqCarrierFreqInfo_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_InterFreqCarrierFreqInfo_1,
	24,	/* Elements count */
	&asn_SPC_InterFreqCarrierFreqInfo_specs_1	/* Additional specs */
};

