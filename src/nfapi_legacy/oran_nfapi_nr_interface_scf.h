

typedef struct {
  nfapi_p7_message_header_t header;
	uint16_t sfn; //0->1023   
  uint16_t slot;//0->319
  
} nfapi_nr_slot_indication_scf_t;

typedef struct
{
  nfapi_p7_message_header_t header;
  uint16_t sfn;
  uint16_t slot;
  uint16_t number_crcs;
  nfapi_nr_crc_t* crc_list;

} nfapi_nr_crc_indication_t;

typedef struct
{
  nfapi_p7_message_header_t header;
  uint16_t sfn;
  uint16_t slot;
  uint16_t number_of_pdus;
  nfapi_nr_rx_data_pdu_t *pdu_list;

} nfapi_nr_rx_data_indication_t;

typedef struct
{
  nfapi_p7_message_header_t header;
  uint16_t sfn;
  uint16_t slot;
  uint16_t num_ucis;
  nfapi_nr_uci_t *uci_list;

} nfapi_nr_uci_indication_t;

typedef struct
{
  nfapi_p7_message_header_t header;
  uint16_t sfn;
  uint16_t slot;
  uint8_t number_of_pdus;
  nfapi_nr_prach_indication_pdu_t* pdu_list;

} nfapi_nr_rach_indication_t;

typedef struct
{
  nfapi_p7_message_header_t header;
  uint16_t sfn;
  uint16_t slot;
  uint8_t number_of_pdus;
  nfapi_nr_srs_indication_pdu_t* pdu_list;

} nfapi_nr_srs_indication_t;