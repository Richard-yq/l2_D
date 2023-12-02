#include "oran_nfapi_interface.h"

/*! The nfapi VNF P7 connection information
 */
typedef struct nfapi_vnf_p7_config nfapi_vnf_p7_config_t;
/*! The nfapi VNF P7 configuration information
 */
typedef struct nfapi_vnf_p7_config
{
	/*! A user define callback to override the default memory allocation
	 * \param size Size of the memory block to allocate
	 * \return a pointer to a memory block
	 *
	 * If not set the vnf p7 library will use malloc
	 */
	void* (*malloc)(size_t size);
	
	/*! A user define callback to override the default memory deallocation
	 * \param ptr Pointer to a memory block to deallocate
	 *
	 * If not set the vnf p7 library will use free
	 */
	void (*free)(void*);

	/*! The port the vnf p7 will receive on */
	int port;

	/*! Flag to indicate of the pnf should use the P7 checksum */
	uint8_t checksum_enabled;

	/*! The maxium size of a P7 segement. If a message is large that this it
	 * will be segemented */
	uint16_t segment_size;
	uint16_t max_num_segments;

	/*! Configuration option for the p7 pack unpack functions*/
	nfapi_p7_codec_config_t codec_config;

	/* ! Call back to indicate the sync state with the PNF PHY
	 * \param config A pointer to the vnf p7 configuration
	 * \param sync Indicating if the pnf is in sync or not
	 * \return not currently used
	 *
	 * sync = 0  : in sync
	 * sync != 0 : out of sync
	 */
	int (*sync_indication)(struct nfapi_vnf_p7_config* config, uint8_t sync);

	/*! A callback for the subframe indication
	 * \param config A pointer to the vnf p7 configuration
	 * \param phy_id The ID for the PNF PHY instance
	 * \param sfn_sf The SFN SF number formated as per the FAPI specification
	 * \return not currently used
	 *
	 * This callback is an indication for the VNF to generate the downlink subframe messages
	 * for sfn/sf. This indicatoin is called every millisecond
	 *
	 * The VNF P7 Lib will adjust the subframe timing to 'catch-up' or 'slow-down' with the PNF PHY's
	 *
	 * \todo Need some way the tell the VNF how long it has
	 */
	
	int (*subframe_indication)(struct nfapi_vnf_p7_config* config, uint16_t phy_id, uint16_t sfn_sf);
	int (*slot_indication)(struct nfapi_vnf_p7_config* config, uint16_t phy_id, uint16_t sfn, uint16_t slot);

	/*! A callback for the HARQ.indication
     *  \param config A pointer to the vnf p7 configuration
	 *  \param ind A data structure for the decoded HARQ.indication This will 
	 *              have been allocated on the stack. 
	 *  \return not currently used.
	 * 
	 *  The ind may contain pointers to dyanmically allocated sub structures  
	 *  such as the pdu. The dyanmically allocated structure will 
	 *  be deallocated on return. If the client wishes to 'keep' the structures 
	 *  then the substructure pointers should be set to 0 and then the client should
	 *  use the codec_config.deallocate function to release it at a future point
	 */	
	int (*harq_indication)(struct nfapi_vnf_p7_config* config, nfapi_harq_indication_t* ind);
	
	/*! A callback for the CRC.ind
     *  \param config A pointer to the vnf p7 configuration
	 *  \param ind A data structure for the decoded CRC.indication This will 
	 *              have been allocated on the stack. 
	 *  \return not currently used.
	 * 
	 *  The ind may contain pointers to dyanmically allocated sub structures  
	 *  such as the pdu. The dyanmically allocated structure will 
	 *  be deallocated on return. If the client wishes to 'keep' the structures 
	 *  then the substructure pointers should be set to 0 and then the client should
	 *  use the codec_config.deallocate function to release it at a future point
	 */	
	int (*crc_indication)(struct nfapi_vnf_p7_config* config, nfapi_crc_indication_t* ind);
	
	/*! A callback for the RX_ULSCH.indication
     *  \param config A pointer to the vnf p7 configuration
	 *  \param ind A data structure for the decoded RX_ULSCH.indication This will 
	 *              have been allocated on the stack. 
	 *  \return not currently used.
	 * 
	 *  The ind may contain pointers to dyanmically allocated sub structures  
	 *  such as the pdu. The dyanmically allocated structure will 
	 *  be deallocated on return. If the client wishes to 'keep' the structures 
	 *  then the substructure pointers should be set to 0 and then the client should
	 *  use the codec_config.deallocate function to release it at a future point
	 *
	 *  Note that the rx_indication may hold one or many uplink pdus in the 
	 *  ind.rx_indication_body.rx_pdu_list
	 */	
	int (*rx_indication)(struct nfapi_vnf_p7_config* config, nfapi_rx_indication_t* ind);
	
	/*! A callback for the RACH.indication
     *  \param config A pointer to the vnf p7 configuration
	 *  \param ind A data structure for the decoded RACH.indication This will 
	 *              have been allocated on the stack. 
	 *  \return not currently used.
	 * 
	 *  The ind may contain pointers to dyanmically allocated sub structures  
	 *  such as the pdu. The dyanmically allocated structure will 
	 *  be deallocated on return. If the client wishes to 'keep' the structures 
	 *  then the substructure pointers should be set to 0 and then the client should
	 *  use the codec_config.deallocate function to release it at a future point
	 */	
	int (*rach_indication)(struct nfapi_vnf_p7_config* config, nfapi_rach_indication_t* ind);
	
	/*! A callback for the SRS.indication
     *  \param config A pointer to the vnf p7 configuration
	 *  \param ind A data structure for the decoded SRS.indication This will 
	 *              have been allocated on the stack. 
	 *  \return not currently used.
	 * 
	 *  The ind may contain pointers to dyanmically allocated sub structures  
	 *  such as the pdu. The dyanmically allocated structure will 
	 *  be deallocated on return. If the client wishes to 'keep' the structures 
	 *  then the substructure pointers should be set to 0 and then the client should
	 *  use the codec_config.deallocate function to release it at a future point
	 */	
	int (*srs_indication)(struct nfapi_vnf_p7_config* config, nfapi_srs_indication_t* ind);
	
	/*! A callback for the RX_SR.indication
     *  \param config A pointer to the vnf p7 configuration
	 *  \param ind A data structure for the decoded RX_SR.indication This will 
	 *              have been allocated on the stack. 
	 *  \return not currently used.
	 * 
	 *  The ind may contain pointers to dyanmically allocated sub structures  
	 *  such as the pdu. The dyanmically allocated structure will 
	 *  be deallocated on return. If the client wishes to 'keep' the structures 
	 *  then the substructure pointers should be set to 0 and then the client should
	 *  use the codec_config.deallocate function to release it at a future point
	 */	
	int (*sr_indication)(struct nfapi_vnf_p7_config* config, nfapi_sr_indication_t* ind);
	
	/*! A callback for the RX_CQI.indication
     *  \param config A pointer to the vnf p7 configuration
	 *  \param ind A data structure for the decoded RX_CQI.indication This will 
	 *              have been allocated on the stack. 
	 *  \return not currently used.
	 * 
	 *  The ind may contain pointers to dyanmically allocated sub structures  
	 *  such as the pdu. The dyanmically allocated structure will 
	 *  be deallocated on return. If the client wishes to 'keep' the structures 
	 *  then the substructure pointers should be set to 0 and then the client should
	 *  use the codec_config.deallocate function to release it at a future point
	 */	
	int (*cqi_indication)(struct nfapi_vnf_p7_config* config, nfapi_cqi_indication_t* ind);
	
	/*! A callback for the LBT_DL.indication
     *  \param config A pointer to the vnf p7 configuration
	 *  \param ind A data structure for the decoded LBT_DL.indication This will 
	 *              have been allocated on the stack. 
	 *  \return not currently used.
	 * 
	 *  The ind may contain pointers to dyanmically allocated sub structures  
	 *  such as the pdu. The dyanmically allocated structure will 
	 *  be deallocated on return. If the client wishes to 'keep' the structures 
	 *  then the substructure pointers should be set to 0 and then the client should
	 *  use the codec_config.deallocate function to release it at a future point
	 */	
	int (*lbt_dl_indication)(struct nfapi_vnf_p7_config* config, nfapi_lbt_dl_indication_t* ind);
	
	/*! A callback for the NB_HARQ.indication
     *  \param config A pointer to the vnf p7 configuration
	 *  \param ind A data structure for the decoded LBT_DL.indication This will 
	 *              have been allocated on the stack. 
	 *  \return not currently used.
	 * 
	 *  The ind may contain pointers to dyanmically allocated sub structures  
	 *  such as the pdu. The dyanmically allocated structure will 
	 *  be deallocated on return. If the client wishes to 'keep' the structures 
	 *  then the substructure pointers should be set to 0 and then the client should
	 *  use the codec_config.deallocate function to release it at a future point
	 */	
	int (*nb_harq_indication)(struct nfapi_vnf_p7_config* config, nfapi_nb_harq_indication_t* ind);	
	
	/*! A callback for the NRACH.indication
     *  \param config A pointer to the vnf p7 configuration
	 *  \param ind A data structure for the decoded LBT_DL.indication This will 
	 *              have been allocated on the stack. 
	 *  \return not currently used.
	 * 
	 *  The ind may contain pointers to dyanmically allocated sub structures  
	 *  such as the pdu. The dyanmically allocated structure will 
	 *  be deallocated on return. If the client wishes to 'keep' the structures 
	 *  then the substructure pointers should be set to 0 and then the client should
	 *  use the codec_config.deallocate function to release it at a future point
	 */	
	int (*nrach_indication)(struct nfapi_vnf_p7_config* config, nfapi_nrach_indication_t* ind);		

	//The NR indication functions below copy uplink information received at the VNF into the UL info struct
	int (*nr_slot_indication)(nfapi_nr_slot_indication_scf_t* ind);
	int (*nr_crc_indication)(nfapi_nr_crc_indication_t* ind);
	int (*nr_rx_data_indication)(nfapi_nr_rx_data_indication_t* ind);
	int (*nr_uci_indication)(nfapi_nr_uci_indication_t* ind);
	int (*nr_rach_indication)(nfapi_nr_rach_indication_t* ind);
	int (*nr_srs_indication)(nfapi_nr_srs_indication_t* ind);

	/*! A callback for any vendor extension messages
     *  \param config A pointer to the vnf p7 configuration
	 *  \param msg A data structure for the decoded vendor extention message allocated
	 *			   using the allocate_p7_vendor_ext callback
	 *  \return not currently used.
	 */	
	int (*vendor_ext)(struct nfapi_vnf_p7_config* config, nfapi_p7_message_header_t* msg);

	/*! Optional userdata that will be passed back in the callbacks*/
	void* user_data;
	
	/*! A callback to allocate a memory for a vendor extension message
	 *  \param message_id The message is taken from the p7 message header
	 *  \param msg_size The is the vendor extention message that has been allocated. 
	 *					The callee must set this value
	 *	\return A pointer to an allocated vendor extention message
	 */
	nfapi_p7_message_header_t* (*allocate_p7_vendor_ext)(uint16_t message_id, uint16_t* msg_size);
	
	/*! A callback to deallocate a vendor extension message
	 *  \param header A pointer to an allocated vendor extention message
	 */
	void (*deallocate_p7_vendor_ext)(nfapi_p7_message_header_t* header);


} nfapi_vnf_p7_config_t;