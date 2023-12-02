#include "stddef.h"
#include <stdint.h>

typedef struct nfapi_p7_codec_config {

	/*! Optional call back to allow the user to define the memory allocator. 
	 *  \param size The size of the memory to allocate
	 *  \return a pointer to a valid memory block or 0 if it has failed.
	 *
	 * If not set the nfapi unpack functions will use malloc
	 */
	void* (*allocate)(size_t size);

	/*! Optional call back to allow the user to define the memory deallocator. 
	 *  \param ptr A poiner to a memory block allocated by the allocate callback
	 * 
	 *	If not set the client should use free
	 */
	void (*deallocate)(void* ptr);

	/*! Optional call back function to handle unpacking vendor extension tlv.
	 *  \param tl A pointer to a decoded tag length structure
	 *  \param ppReadPackedMsg A handle to the read buffer. 
	 *  \param end The end of the read buffer
	 *  \param ve A handle to a vendor extention structure that the call back should allocate if the structure can be decoded
	 *  \param config A pointer to the p7 codec configuration
	 *  \return return 0 if packed successfully, -1 if failed.
	 *
	 *  If not set the tlv will be skipped
	 *
	 *  Client should use the help methods in nfapi.h to decode the vendor extention.
	 * 
	 *  \todo Add code example
	 */
	int (*unpack_vendor_extension_tlv)(nfapi_tl_t* tl, uint8_t **ppReadPackedMsg, uint8_t *end, void** ve, struct nfapi_p7_codec_config* config);

	/*! Optional call back function to handle packing vendor extension tlv. 
	 *  \param ve A pointer to a vendor extention structure.
	 *  \param ppWritePackedMsg A handle to the write buffer
	 *  \param end The end of the write buffer. The callee should make sure not to write beyond the end
	 *  \param config A pointer to the p7 codec configuration
	 *  \return return 0 if packed successfully, -1 if failed.
	 * 
	 *  If not set the the tlv will be skipped
	 * 
	 *  Client should use the help methods in nfapi.h to encode the vendor extention
	 * 
	 *  \todo Add code example
	 */
	int (*pack_vendor_extension_tlv)(void* ve, uint8_t **ppWritePackedMsg, uint8_t *end, struct nfapi_p7_codec_config* config);

	/*! Optional call back function to handle unpacking vendor extension messages. 
	 *  \param header A pointer to a decode P7 message header for the vendor extention message
	 *  \param ppReadPackedMsg A handle to the encoded data buffer
	 *  \param end A pointer to the end of the encoded data buffer
	 *  \param config  A pointer to the p7 codec configuration
	 *  \return 0 if unpacked successfully, -1 if failed
	 *
	 *  If not set the message will be ignored
	 *
	 *  If the message if is unknown the function should return -1
	 */
	int (*unpack_p7_vendor_extension)(nfapi_p7_message_header_t* header, uint8_t **ppReadPackedMsg, uint8_t *end, struct nfapi_p7_codec_config* config);

	/*! Optional call back function to handle packing vendor extension messages. 
	 *  \param header A poiner to a P7 message structure for the venfor extention message
	 *  \param ppWritePackedmsg A handle to the buffer to write the encoded message into
	 *  \param end A pointer to the end of the buffer
	 *  \param cofig A pointer to the p7 codec configuration
	 *  \return 0 if packed successfully, -1 if failed
	 * 
	 * If not set the the message will be ingored
	 *	 
	 *  If the message if is unknown the function should return -1
	 */
	int (*pack_p7_vendor_extension)(nfapi_p7_message_header_t* header, uint8_t **ppWritePackedmsg, uint8_t *end, struct nfapi_p7_codec_config* config);

	/*! Optional user data that will be passed back with callbacks
	 */
	void* user_data;

} nfapi_p7_codec_config_t;


typedef struct {
	nfapi_p7_message_header_t header;
	uint16_t sfn_sf;
	nfapi_harq_indication_body_t harq_indication_body;
	nfapi_vendor_extension_tlv_t vendor_extension;
} nfapi_harq_indication_t;

typedef struct {
	nfapi_p7_message_header_t header;
	uint16_t sfn_sf;
	nfapi_crc_indication_body_t crc_indication_body;
	nfapi_vendor_extension_tlv_t vendor_extension;
} nfapi_crc_indication_t;

typedef struct {
	nfapi_p7_message_header_t header;
	uint16_t sfn_sf;
	nfapi_sr_indication_body_t sr_indication_body;
	nfapi_vendor_extension_tlv_t vendor_extension;
} nfapi_sr_indication_t;

typedef struct {
	nfapi_p7_message_header_t header;
	uint16_t sfn_sf;
	nfapi_rx_indication_body_t rx_indication_body;
	nfapi_vendor_extension_tlv_t vendor_extension;
} nfapi_rx_indication_t;

typedef struct {
	nfapi_p7_message_header_t header;
	uint16_t sfn_sf;
	nfapi_rach_indication_body_t rach_indication_body;
	nfapi_vendor_extension_tlv_t vendor_extension;
} nfapi_rach_indication_t;

typedef struct {
	nfapi_p7_message_header_t header;
	uint16_t sfn_sf;
	nfapi_srs_indication_body_t srs_indication_body;
	nfapi_vendor_extension_tlv_t vendor_extension;
} nfapi_srs_indication_t;

typedef struct {
	nfapi_p7_message_header_t header;
	uint16_t sfn_sf;
	nfapi_cqi_indication_body_t cqi_indication_body;
	nfapi_vendor_extension_tlv_t vendor_extension;
} nfapi_cqi_indication_t;

typedef struct {
	nfapi_p7_message_header_t header;
	uint16_t sfn_sf;
	nfapi_lbt_dl_indication_body_t lbt_dl_indication_body;
	nfapi_vendor_extension_tlv_t vendor_extension;
} nfapi_lbt_dl_indication_t;

typedef struct {
	nfapi_p7_message_header_t header;
	uint16_t sfn_sf;
	nfapi_nb_harq_indication_body_t nb_harq_indication_body;
	nfapi_vendor_extension_tlv_t vendor_extension;
} nfapi_nb_harq_indication_t;

typedef struct {
	nfapi_p7_message_header_t header;
	uint16_t sfn_sf;
	nfapi_nrach_indication_body_t nrach_indication_body;
	nfapi_vendor_extension_tlv_t vendor_extension;
} nfapi_nrach_indication_t;

typedef struct {
	nfapi_p7_message_header_t header;
	uint16_t sfn_sf;
	nfapi_nrach_indication_body_t nrach_indication_body;
	nfapi_vendor_extension_tlv_t vendor_extension;
} nfapi_nrach_indication_t;