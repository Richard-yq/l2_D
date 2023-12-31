/*******************************************************************************
################################################################################
#   Copyright (c) [2017-2019] [Radisys]                                        #
#                                                                              #
#   Licensed under the Apache License, Version 2.0 (the "License");            #
#   you may not use this file except in compliance with the License.           #
#   You may obtain a copy of the License at                                    #
#                                                                              #
#       http://www.apache.org/licenses/LICENSE-2.0                             #
#                                                                              #
#   Unless required by applicable law or agreed to in writing, software        #
#   distributed under the License is distributed on an "AS IS" BASIS,          #
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   #
#   See the License for the specific language governing permissions and        #
#   limitations under the License.                                             #
################################################################################
*******************************************************************************/

/* header include files (.h) */
#include "common_def.h"
#include "lrg.h"
//#define INTEL_FAPI
#ifdef INTEL_FAPI
#include "fapi.h"
#include "fapi_vendor_extension.h"
#endif

#include "../nfapi/nfapi_nr_interface_scf.h"
#include "../nfapi/nfapi_vnf_interface.h"
/* header/extern include files (.x) */
#include "lrg.x"
#include "du_app_mac_inf.h"
#include "mac_sch_interface.h"
#include "lwr_mac.h"
#include "lwr_mac_fsm.h"
#include "lwr_mac_phy.h"
#include "lwr_mac_upr_inf.h"
#include "mac.h"
#include "mac_utils.h"
#include "lwr_mac_utils.h"

/* Function pointer for slot indication from lower mac to mac */
SlotIndFunc sendSlotIndOpts[] =
{
   packSlotInd,  /* packing for loosely coupled */
   fapiMacSlotInd, /* packing for tightly coupled */
   packSlotInd /* packing for light weight loosly coupled */
};

/* Function pointer for rach indication from lower mac to mac */ 
RachIndFunc sendRachIndOpts[] =
{
   packRachInd,
   fapiMacRachInd,
   packRachInd
};

/* Function pointer for crc indication from lower mac to mac */
CrcIndFunc sendCrcIndOpts[] =
{
   packCrcInd,
   fapiMacCrcInd,
   packCrcInd
};

/* Function pointer for Rx Data indication from lower mac to mac */
RxDataIndFunc sendRxDataIndOpts[] =
{
   packRxDataInd,
   fapiMacRxDataInd,
   packRxDataInd
};

/* Function pointer for stop indication from lower mac to mac */ 
StopIndFunc sendStopIndOpts[] =
{
   packStopInd,
   fapiMacStopInd,
   packStopInd
};

/* Function pointer for Uci indication from lower mac to mac */
UciIndFunc sendUciIndOpts[] =
{
   packUciInd,
   FapiMacUciInd,
   packUciInd
};

/*******************************************************************
 *
 * @brief Processes Slot Indication from PHY and sends to MAC
 *
 * @details
 *
 *    Function : procSlotInd
 *
 *    Functionality:
 *     Processes Slot Indication from PHY and sends to MAC
 *
 * @params[in] fapi_slot_ind_t pointer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t ORAN_OAI_procSlotInd(NR_UL_IND_t *UL_INFO)
{
   printf("[MWNL] %s\n", __FUNCTION__);
   /* fill Pst structure to send to lwr_mac to MAC */
   Pst pst;
   uint16_t ret = 0;
   SlotTimingInfo *slotInd = {0};

   if(lwrMacCb.phyState == PHY_STATE_CONFIGURED)
   {
      DU_LOG("\nINFO  -->  LWR_MAC: PHY has moved to running state");
      lwrMacCb.phyState = PHY_STATE_RUNNING;
      lwrMacCb.cellCb[0].state = PHY_STATE_RUNNING;
   }

   MAC_ALLOC_SHRABL_BUF(UL_INFO, sizeof(NR_UL_IND_t));

   if(UL_INFO)
   {
      UL_INFO->cellId = lwrMacCb.cellCb[0].cellId; 
      FILL_PST_LWR_MAC_TO_MAC(pst, EVENT_SLOT_IND_TO_MAC);
      pst.selector = ODU_SELECTOR_LWLC;
      ret = (*sendSlotIndOpts[pst.selector])(&pst, UL_INFO);
   }
   else
   {
      DU_LOG("\nERROR  -->  LWR_MAC: Memory allocation failed in procSlotInd");
      ret = RFAILED;
   }
   return ret;
}

/*******************************************************************
 *
 * @brief Handles Rx Data indication from PHY and sends to MAC
 *
 * @details
 *
 *    Function : procRxDataInd
 *
 *    Functionality:
 *      Handles Rx Data indication from PHY and sends to MAC
 *
 * @params[in] fapi_rx_data_indication_t message pointer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t ORAN_OAI_procRxDataInd(nfapi_nr_rx_data_indication_t  *fapiRxDataInd)
{
   Pst           pst;
   uint8_t       pduIdx =0;
   RxDataInd     *rxDataInd = NULLP;
   RxDataIndPdu  *pdu = NULLP;   

   MAC_ALLOC_SHRABL_BUF(rxDataInd, sizeof(RxDataInd));
   if(!rxDataInd)
   {
      DU_LOG("\nERROR  -->  LWR_MAC : Memory Allocation failed in procRxDataInd");
      return RFAILED;
   }
   if(!fapiRxDataInd->number_of_pdus)
   {
      DU_LOG("\nDEBUG  -->  LWR_MAC : No PDU in RX_Data.indication at [%d, %d]", fapiRxDataInd->sfn, fapiRxDataInd->slot);
      return ROK;
   }

   rxDataInd->cellId = lwrMacCb.cellCb[0].cellId;
   rxDataInd->timingInfo.sfn = fapiRxDataInd->sfn; 
   rxDataInd->timingInfo.slot = fapiRxDataInd->slot;
   rxDataInd->numPdus = fapiRxDataInd->number_of_pdus;

   for(pduIdx = 0; pduIdx < rxDataInd->numPdus; pduIdx++)
   {
      pdu = &rxDataInd->pdus[pduIdx];
      pdu->handle = fapiRxDataInd->pdu_list[pduIdx].handle;
      pdu->rnti = fapiRxDataInd->pdu_list[pduIdx].rnti;
      pdu->harqId = fapiRxDataInd->pdu_list[pduIdx].harq_id;
      pdu->pduLength = fapiRxDataInd->pdu_list[pduIdx].pdu_length;
      pdu->ul_cqi = fapiRxDataInd->pdu_list[pduIdx].ul_cqi;
      pdu->timingAdvance = fapiRxDataInd->pdu_list[pduIdx].timing_advance;
      pdu->rssi = fapiRxDataInd->pdu_list[pduIdx].rssi;

      MAC_ALLOC_SHRABL_BUF(pdu->pduData, pdu->pduLength);
      memcpy(pdu->pduData, fapiRxDataInd->pdu_list[pduIdx].pdu, pdu->pduLength);
#ifdef INTEL_WLS_MEM      
      /* Free WLS memory allocated for Rx PDU */
      WLS_MEM_FREE(fapiRxDataInd->pdus[pduIdx].pduData, LWR_MAC_WLS_BUF_SIZE);
#endif
   }

   /* Fill post and sent to MAC */
   FILL_PST_LWR_MAC_TO_MAC(pst, EVENT_RX_DATA_IND_TO_MAC);
   return (*sendRxDataIndOpts[pst.selector])(&pst, rxDataInd);
}

/*******************************************************************
 *
 * @brief Handles CRC indication from PHY and sends to MAC
 *
 * @details
 *
 *    Function : procCrcInd
 *
 *    Functionality:
 *      Handles CRC indication from PHY and sends to MAC
 *
 * @params[in] fapi_crc_ind_t message pointer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t ORAN_OAI_procCrcInd(nfapi_nr_crc_indication_t  *fapiCrcInd)
{
   Pst          pst;
   uint8_t      crcInfoIdx;
   uint8_t      crcStatusIdx;
   CrcInfo      *crcIndInfo = NULLP;
   CrcInd       *crcInd = NULLP;

   MAC_ALLOC_SHRABL_BUF(crcInd, sizeof(CrcInd));
   if(!crcInd)
   {
      DU_LOG("\nERROR  -->  LWR_MAC : Memory Allocation failed in procCrcInd");
      return RFAILED;
   }
   if(!fapiCrcInd->number_crcs)
   {
      DU_LOG("\nDEBUG  --> LWR_MAC : No CRC PDUs in CRC.indication at [%d, %d]", fapiCrcInd->sfn, fapiCrcInd->slot);
      return ROK;
   }

   crcInd->cellId = lwrMacCb.cellCb[0].cellId;
   crcInd->timingInfo.sfn = fapiCrcInd->sfn;
   crcInd->timingInfo.slot = fapiCrcInd->slot;
   crcInd->numCrc = fapiCrcInd->number_crcs;

   for(crcInfoIdx = 0; crcInfoIdx < crcInd->numCrc; crcInfoIdx++)
   {
      crcIndInfo = &crcInd->crcInfo[crcInfoIdx];
      crcIndInfo->handle      = fapiCrcInd->crc_list[crcInfoIdx].handle;
      crcIndInfo->rnti        = fapiCrcInd->crc_list[crcInfoIdx].rnti;
      crcIndInfo->harqId      = fapiCrcInd->crc_list[crcInfoIdx].harq_id;
      crcIndInfo->tbCrcStatus = fapiCrcInd->crc_list[crcInfoIdx].cb_crc_status;
      crcIndInfo->numCb       = fapiCrcInd->crc_list[crcInfoIdx].num_cb;
      for(crcStatusIdx = 0; crcStatusIdx < crcIndInfo->numCb; crcStatusIdx++)
      {
	 crcIndInfo->cbCrcStatus[crcStatusIdx] = \
	    fapiCrcInd->crc_list[crcInfoIdx].cb_crc_status[crcStatusIdx];
      }
      crcIndInfo->ul_cqi  = fapiCrcInd->crc_list[crcInfoIdx].ul_cqi;
      crcIndInfo->timingAdvance = fapiCrcInd->crc_list[crcInfoIdx].timing_advance;
      crcIndInfo->rssi = fapiCrcInd->crc_list[crcInfoIdx].rssi;
   }

   /* Fill post and sent to MAC */
   FILL_PST_LWR_MAC_TO_MAC(pst, EVENT_CRC_IND_TO_MAC);
   return (*sendCrcIndOpts[pst.selector])(&pst, crcInd);
} /* handleCrcInd */
/*******************************************************************
 *
 * @brief Fills Uci Ind Pdu Info carried on Pucch Format 0/Format 1
 *
 * @details
 *
 *    Function : fillUciIndPucchF0F1
 *
 *    Functionality:
 *       Fills Uci Ind Pdu Info carried on Pucch Format 0/Format 1
 *
 *@params[in] UciPucchF0F1 *
 *            fapi_uci_o_pucch_f0f1_t *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t fillUciIndPucchF0F1(UciPucchF0F1 *pduInfo, nfapi_nr_uci_pucch_pdu_format_0_1_t *fapiPduInfo)
{

   uint8_t harqIdx;
   uint8_t ret = ROK;
   
   pduInfo->handle        = fapiPduInfo->handle;
   pduInfo->pduBitmap     = fapiPduInfo->pduBitmap;
   pduInfo->pucchFormat   = fapiPduInfo->pucch_format;
   pduInfo->ul_cqi        = fapiPduInfo->ul_cqi;
   pduInfo->crnti         = fapiPduInfo->rnti;
   pduInfo->timingAdvance = fapiPduInfo->timing_advance;
   pduInfo->rssi          = fapiPduInfo->rssi;   
   if(fapiPduInfo->sr->sr_indication)
   {
      pduInfo->srInfo.srIndPres = fapiPduInfo->sr->sr_indication;
      pduInfo->srInfo.srConfdcLevel = fapiPduInfo->sr->sr_confidence_level;
   }
   if(fapiPduInfo->harq->num_harq)
   {
      pduInfo->harqInfo.numHarq = fapiPduInfo->harq->num_harq;
      pduInfo->harqInfo.harqConfdcLevel = fapiPduInfo->harq->harq_confidence_level;
      for(harqIdx = 0; harqIdx < pduInfo->harqInfo.numHarq; harqIdx++)
      {
         pduInfo->harqInfo.harqValue[harqIdx] = fapiPduInfo->harq->harq_list[harqIdx].harq_value;
      }
   }
   return ret;
}

/*******************************************************************
 *
 * @brief Handles Uci indication from PHY and sends to MAC
 *
 * @details
 *
 *    Function : procUciInd
 *
 *    Functionality:
 *      Handles Uci indication from PHY and sends to MAC
 *
 * @params[in] fapi_uci_indication_t message pointer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t ORAN_OAI_procUciInd(nfapi_nr_uci_indication_t  *fapiUciInd)
{
   uint8_t pduIdx;
   uint8_t ret = ROK;
   Pst     pst;
   UciInd  *macUciInd = NULLP;

   MAC_ALLOC_SHRABL_BUF(macUciInd, sizeof(UciInd));
   if(!macUciInd)
   {
      DU_LOG("\nERROR  -->  LWR_MAC: Memory Allocation failed in procUciInd");
      return RFAILED;
   }

   DU_LOG("\nDEBUG  -->  LWR_MAC: Processing UCI Indication");
   memset(macUciInd, 0, sizeof(UciInd));
   macUciInd->cellId = lwrMacCb.cellCb[0].cellId;
   macUciInd->slotInd.sfn = fapiUciInd->sfn; 
   macUciInd->slotInd.slot = fapiUciInd->slot;
   macUciInd->numUcis = fapiUciInd->num_ucis;

   for(pduIdx = 0; pduIdx < macUciInd->numUcis; pduIdx++)
   {
      macUciInd->pdus[pduIdx].pduType = fapiUciInd->uci_list[pduIdx].pdu_type;
      switch(macUciInd->pdus[pduIdx].pduType)
      {
         case UCI_IND_PUSCH:
         break;
         case UCI_IND_PUCCH_F0F1:
         {
            UciPucchF0F1 *pduInfo = NULLP;
            macUciInd->pdus[pduIdx].pduSize = fapiUciInd->uci_list[pduIdx].pdu_size;
            pduInfo = &macUciInd->pdus[pduIdx].uci.uciPucchF0F1;
            ret = fillUciIndPucchF0F1(pduInfo, &fapiUciInd->uci_list[pduIdx].pucch_pdu_format_0_1);
         }
         break;
         case UCI_IND_PUCCH_F2F3F4:
            break;
         default:
            DU_LOG("\nERROR  -->  LWR_MAC: Invalid Pdu Type %d at procmacUciInd()", macUciInd->pdus[pduIdx].pduType);
	    ret = RFAILED;
            break;
      }
   }
   if(!ret)
   {
      FILL_PST_LWR_MAC_TO_MAC(pst, EVENT_UCI_IND_TO_MAC);
      ret = (*sendUciIndOpts[pst.selector])(&pst, macUciInd);
   }
   else
   {
      DU_LOG("\nERROR  -->  LWR_MAC: Failed sending UCI Ind to MAC");
   }
   return ret;
}

/*******************************************************************
 *
 * @brief Processes Rach Indication from PHY and sends to MAC
 *
 * @details
 *
 *    Function : procRachInd
 *
 *    Functionality:
 *         Processes Rach Indication from PHY and sends to MAC
 *
 * @params[in] fapi_rach_indication_t pointer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t ORAN_OAI_procRachInd(nfapi_nr_rach_indication_t  *fapiRachInd)
{
   Pst          pst;
   uint8_t      pduIdx;
   uint8_t      prmbleIdx;
   RachPduInfo  *rachPdu = NULLP;
   RachInd      *rachInd = NULLP;

   MAC_ALLOC_SHRABL_BUF(rachInd, sizeof(RachInd));
   if(!rachInd)
   {
      DU_LOG("\nERROR  -->  LWR_MAC : Memory Allocation failed in procRachInd");
      return RFAILED;
   }
   if(!fapiRachInd->number_of_pdus)
   {
      DU_LOG("\nDEBUG  -->  LWR_MAC : No PDU in RACH.indication at [%d, %d]", fapiRachInd->sfn, fapiRachInd->slot);
      return ROK;
   }

   rachInd->cellId = lwrMacCb.cellCb[0].cellId;
   rachInd->timingInfo.sfn = fapiRachInd->sfn;
   rachInd->timingInfo.slot = fapiRachInd->slot;
   rachInd->numPdu = fapiRachInd->number_of_pdus;
   for(pduIdx=0; pduIdx < rachInd->numPdu; pduIdx++)
   {
      rachPdu = &rachInd->rachPdu[pduIdx];
      rachPdu->pci = fapiRachInd->pdu_list[pduIdx].phy_cell_id;
      rachPdu->symbolIdx = fapiRachInd->pdu_list[pduIdx].symbol_index;
      rachPdu->slotIdx = fapiRachInd->pdu_list[pduIdx].slot_index;
      rachPdu->freqIdx = fapiRachInd->pdu_list[pduIdx].freq_index;
      rachPdu->numPream = fapiRachInd->pdu_list[pduIdx].num_preamble; 
      for(prmbleIdx=0; prmbleIdx<rachPdu->numPream; prmbleIdx++)
      {
	 rachPdu->preamInfo[prmbleIdx].preamIdx = \
	    fapiRachInd->pdu_list[pduIdx].preamble_list[prmbleIdx].preamble_index;
	 rachPdu->preamInfo[prmbleIdx].timingAdv = \
	    fapiRachInd->pdu_list[pduIdx].preamble_list[prmbleIdx].timing_advance;
      }
   }

   /* Fill post and sent to MAC */
   FILL_PST_LWR_MAC_TO_MAC(pst, EVENT_RACH_IND_TO_MAC);
   return (*sendRachIndOpts[pst.selector])(&pst, rachInd);
}/* handleRachInd */


/*******************************************************************
 *
 * @brief Processes Slot Indication from PHY and sends to MAC
 *
 * @details
 *
 *    Function : procSlotInd
 *
 *    Functionality:
 *     Processes Slot Indication from PHY and sends to MAC
 *
 * @params[in] fapi_slot_ind_t pointer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
#ifdef INTEL_FAPI
uint8_t procSlotInd(fapi_slot_ind_t *fapiSlotInd)
{
   /* fill Pst structure to send to lwr_mac to MAC */
   Pst pst;
   uint16_t ret = 0;
   SlotTimingInfo *slotInd = {0};

   MAC_ALLOC_SHRABL_BUF(slotInd, sizeof(SlotTimingInfo));
   if(slotInd)
   {
      slotInd->cellId = lwrMacCb.cellCb[0].cellId; 
      slotInd->sfn = fapiSlotInd->sfn;
      slotInd->slot = fapiSlotInd->slot;

      FILL_PST_LWR_MAC_TO_MAC(pst, EVENT_SLOT_IND_TO_MAC);
      pst.selector = ODU_SELECTOR_LWLC;
      ret = (*sendSlotIndOpts[pst.selector])(&pst, slotInd);
   }
   else
   {
      DU_LOG("\nERROR  -->  LWR_MAC: Memory allocation failed in procSlotInd");
      ret = RFAILED;
   }
   return ret;
}
#endif
/*******************************************************************
 *
 * @brief Handles stop indication recived from PHY
 *
 * @details
 *
 *    Function : procStopInd
 *
 *    Functionality:
 *         Handles Stop Indication received from PHY
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
#ifdef INTEL_FAPI
uint8_t procStopInd()
{
   Pst pst;
   uint16_t *cellId = NULLP;

   MAC_ALLOC_SHRABL_BUF(cellId, sizeof(uint16_t));
   if(!cellId)
   {
      DU_LOG("\nERROR  -->  LWR_MAC: Memory Allocation failed in procStopInd");
      return RFAILED;
   }

   *cellId = lwrMacCb.cellCb[0].cellId;
   lwrMacCb.phyState = PHY_STATE_CONFIGURED;
   lwrMacCb.cellCb[0].state = PHY_STATE_CONFIGURED;
   DU_LOG("\nINFO  -->  LWR_MAC: PHY has moved to configured state");

   FILL_PST_LWR_MAC_TO_MAC(pst, EVENT_STOP_IND_TO_MAC);
   pst.selector = ODU_SELECTOR_LWLC; 
   return (*sendStopIndOpts[pst.selector])(&pst, cellId);
}
#endif
/*******************************************************************
 *
 * @brief Processes Rach Indication from PHY and sends to MAC
 *
 * @details
 *
 *    Function : procRachInd
 *
 *    Functionality:
 *         Processes Rach Indication from PHY and sends to MAC
 *
 * @params[in] fapi_rach_indication_t pointer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
#ifdef INTEL_FAPI
uint8_t procRachInd(fapi_rach_indication_t  *fapiRachInd)
{
   Pst          pst;
   uint8_t      pduIdx;
   uint8_t      prmbleIdx;
   RachPduInfo  *rachPdu = NULLP;
   RachInd      *rachInd = NULLP;

   MAC_ALLOC_SHRABL_BUF(rachInd, sizeof(RachInd));
   if(!rachInd)
   {
      DU_LOG("\nERROR  -->  LWR_MAC : Memory Allocation failed in procRachInd");
      return RFAILED;
   }
   if(!fapiRachInd->numPdus)
   {
      DU_LOG("\nDEBUG  -->  LWR_MAC : No PDU in RACH.indication at [%d, %d]", fapiRachInd->sfn, fapiRachInd->slot);
      return ROK;
   }

   rachInd->cellId = lwrMacCb.cellCb[0].cellId;
   rachInd->timingInfo.sfn = fapiRachInd->sfn;
   rachInd->timingInfo.slot = fapiRachInd->slot;
   rachInd->numPdu = fapiRachInd->numPdus;
   for(pduIdx=0; pduIdx < rachInd->numPdu; pduIdx++)
   {
      rachPdu = &rachInd->rachPdu[pduIdx];
      rachPdu->pci = fapiRachInd->rachPdu[pduIdx].phyCellId;
      rachPdu->symbolIdx = fapiRachInd->rachPdu[pduIdx].symbolIndex;
      rachPdu->slotIdx = fapiRachInd->rachPdu[pduIdx].slotIndex;
      rachPdu->freqIdx = fapiRachInd->rachPdu[pduIdx].freqIndex;
      rachPdu->numPream = fapiRachInd->rachPdu[pduIdx].numPreamble; 
      for(prmbleIdx=0; prmbleIdx<rachPdu->numPream; prmbleIdx++)
      {
	 rachPdu->preamInfo[prmbleIdx].preamIdx = \
	    fapiRachInd->rachPdu[pduIdx].preambleInfo[prmbleIdx].preambleIndex;
	 rachPdu->preamInfo[prmbleIdx].timingAdv = \
	    fapiRachInd->rachPdu[pduIdx].preambleInfo[prmbleIdx].timingAdvance;
      }
   }

   /* Fill post and sent to MAC */
   FILL_PST_LWR_MAC_TO_MAC(pst, EVENT_RACH_IND_TO_MAC);
   return (*sendRachIndOpts[pst.selector])(&pst, rachInd);
}/* handleRachInd */
#endif
/*******************************************************************
 *
 * @brief Handles CRC indication from PHY and sends to MAC
 *
 * @details
 *
 *    Function : procCrcInd
 *
 *    Functionality:
 *      Handles CRC indication from PHY and sends to MAC
 *
 * @params[in] fapi_crc_ind_t message pointer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
#ifdef INTEL_FAPI
uint8_t procCrcInd(fapi_crc_ind_t  *fapiCrcInd)
{
   Pst          pst;
   uint8_t      crcInfoIdx;
   uint8_t      crcStatusIdx;
   CrcInfo      *crcIndInfo = NULLP;
   CrcInd       *crcInd = NULLP;

   MAC_ALLOC_SHRABL_BUF(crcInd, sizeof(CrcInd));
   if(!crcInd)
   {
      DU_LOG("\nERROR  -->  LWR_MAC : Memory Allocation failed in procCrcInd");
      return RFAILED;
   }
   if(!fapiCrcInd->numCrcs)
   {
      DU_LOG("\nDEBUG  --> LWR_MAC : No CRC PDUs in CRC.indication at [%d, %d]", fapiCrcInd->sfn, fapiCrcInd->slot);
      return ROK;
   }

   crcInd->cellId = lwrMacCb.cellCb[0].cellId;
   crcInd->timingInfo.sfn = fapiCrcInd->sfn;
   crcInd->timingInfo.slot = fapiCrcInd->slot;
   crcInd->numCrc = fapiCrcInd->numCrcs;

   for(crcInfoIdx = 0; crcInfoIdx < crcInd->numCrc; crcInfoIdx++)
   {
      crcIndInfo = &crcInd->crcInfo[crcInfoIdx];
      crcIndInfo->handle      = fapiCrcInd->crc[crcInfoIdx].handle;
      crcIndInfo->rnti        = fapiCrcInd->crc[crcInfoIdx].rnti;
      crcIndInfo->harqId      = fapiCrcInd->crc[crcInfoIdx].harqId;
      crcIndInfo->tbCrcStatus = fapiCrcInd->crc[crcInfoIdx].tbCrcStatus;
      crcIndInfo->numCb       = fapiCrcInd->crc[crcInfoIdx].numCb;
      for(crcStatusIdx = 0; crcStatusIdx < crcIndInfo->numCb; crcStatusIdx++)
      {
	 crcIndInfo->cbCrcStatus[crcStatusIdx] = \
	    fapiCrcInd->crc[crcInfoIdx].cbCrcStatus[crcStatusIdx];
      }
      crcIndInfo->ul_cqi  = fapiCrcInd->crc[crcInfoIdx].ul_cqi;
      crcIndInfo->timingAdvance = fapiCrcInd->crc[crcInfoIdx].timingAdvance;
      crcIndInfo->rssi = fapiCrcInd->crc[crcInfoIdx].rssi;
   }

   /* Fill post and sent to MAC */
   FILL_PST_LWR_MAC_TO_MAC(pst, EVENT_CRC_IND_TO_MAC);
   return (*sendCrcIndOpts[pst.selector])(&pst, crcInd);
} /* handleCrcInd */
#endif
/*******************************************************************
 *
 * @brief Handles Rx Data indication from PHY and sends to MAC
 *
 * @details
 *
 *    Function : procRxDataInd
 *
 *    Functionality:
 *      Handles Rx Data indication from PHY and sends to MAC
 *
 * @params[in] fapi_rx_data_indication_t message pointer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
#ifdef INTEL_FAPI
uint8_t procRxDataInd(fapi_rx_data_indication_t  *fapiRxDataInd)
{
   Pst           pst;
   uint8_t       pduIdx =0;
   RxDataInd     *rxDataInd = NULLP;
   RxDataIndPdu  *pdu = NULLP;   

   MAC_ALLOC_SHRABL_BUF(rxDataInd, sizeof(RxDataInd));
   if(!rxDataInd)
   {
      DU_LOG("\nERROR  -->  LWR_MAC : Memory Allocation failed in procRxDataInd");
      return RFAILED;
   }
   if(!fapiRxDataInd->numPdus)
   {
      DU_LOG("\nDEBUG  -->  LWR_MAC : No PDU in RX_Data.indication at [%d, %d]", fapiRxDataInd->sfn, fapiRxDataInd->slot);
      return ROK;
   }

   rxDataInd->cellId = lwrMacCb.cellCb[0].cellId;
   rxDataInd->timingInfo.sfn = fapiRxDataInd->sfn; 
   rxDataInd->timingInfo.slot = fapiRxDataInd->slot;
   rxDataInd->numPdus = fapiRxDataInd->numPdus;

   for(pduIdx = 0; pduIdx < rxDataInd->numPdus; pduIdx++)
   {
      pdu = &rxDataInd->pdus[pduIdx];
      pdu->handle = fapiRxDataInd->pdus[pduIdx].handle;
      pdu->rnti = fapiRxDataInd->pdus[pduIdx].rnti;
      pdu->harqId = fapiRxDataInd->pdus[pduIdx].harqId;
      pdu->pduLength = fapiRxDataInd->pdus[pduIdx].pdu_length;
      pdu->ul_cqi = fapiRxDataInd->pdus[pduIdx].ul_cqi;
      pdu->timingAdvance = fapiRxDataInd->pdus[pduIdx].timingAdvance;
      pdu->rssi = fapiRxDataInd->pdus[pduIdx].rssi;

      MAC_ALLOC_SHRABL_BUF(pdu->pduData, pdu->pduLength);
      memcpy(pdu->pduData, fapiRxDataInd->pdus[pduIdx].pduData, pdu->pduLength);
#ifdef INTEL_WLS_MEM      
      /* Free WLS memory allocated for Rx PDU */
      WLS_MEM_FREE(fapiRxDataInd->pdus[pduIdx].pduData, LWR_MAC_WLS_BUF_SIZE);
#endif
   }

   /* Fill post and sent to MAC */
   FILL_PST_LWR_MAC_TO_MAC(pst, EVENT_RX_DATA_IND_TO_MAC);
   return (*sendRxDataIndOpts[pst.selector])(&pst, rxDataInd);
}
#endif

/*******************************************************************
 *
 * @brief Handles Uci indication from PHY and sends to MAC
 *
 * @details
 *
 *    Function : procUciInd
 *
 *    Functionality:
 *      Handles Uci indication from PHY and sends to MAC
 *
 * @params[in] fapi_uci_indication_t message pointer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
#ifdef INTEL_FAPI

uint8_t procUciInd(fapi_uci_indication_t  *fapiUciInd)
{
   uint8_t pduIdx;
   uint8_t ret = ROK;
   Pst     pst;
   UciInd  *macUciInd = NULLP;

   MAC_ALLOC_SHRABL_BUF(macUciInd, sizeof(UciInd));
   if(!macUciInd)
   {
      DU_LOG("\nERROR  -->  LWR_MAC: Memory Allocation failed in procUciInd");
      return RFAILED;
   }

   DU_LOG("\nDEBUG  -->  LWR_MAC: Processing UCI Indication");
   memset(macUciInd, 0, sizeof(UciInd));
   macUciInd->cellId = lwrMacCb.cellCb[0].cellId;
   macUciInd->slotInd.sfn = fapiUciInd->sfn; 
   macUciInd->slotInd.slot = fapiUciInd->slot;
   macUciInd->numUcis = fapiUciInd->numUcis;

   for(pduIdx = 0; pduIdx < macUciInd->numUcis; pduIdx++)
   {
      macUciInd->pdus[pduIdx].pduType = fapiUciInd->uciPdu[pduIdx].pduType;
      switch(macUciInd->pdus[pduIdx].pduType)
      {
         case UCI_IND_PUSCH:
         break;
         case UCI_IND_PUCCH_F0F1:
         {
            UciPucchF0F1 *pduInfo = NULLP;
            macUciInd->pdus[pduIdx].pduSize = fapiUciInd->uciPdu[pduIdx].pduSize;
            pduInfo = &macUciInd->pdus[pduIdx].uci.uciPucchF0F1;
            ret = fillUciIndPucchF0F1(pduInfo, &fapiUciInd->uciPdu[pduIdx].uci.uciPucchF0F1);
         }
         break;
         case UCI_IND_PUCCH_F2F3F4:
            break;
         default:
            DU_LOG("\nERROR  -->  LWR_MAC: Invalid Pdu Type %d at procmacUciInd()", macUciInd->pdus[pduIdx].pduType);
	    ret = RFAILED;
            break;
      }
   }
   if(!ret)
   {
      FILL_PST_LWR_MAC_TO_MAC(pst, EVENT_UCI_IND_TO_MAC);
      ret = (*sendUciIndOpts[pst.selector])(&pst, macUciInd);
   }
   else
   {
      DU_LOG("\nERROR  -->  LWR_MAC: Failed sending UCI Ind to MAC");
   }
   return ret;
}
#endif /* FAPI */

/*******************************************************************
 * @brief Function prints src, dest, msg infor about all the msgs that
 received.
 *
 * @details
 *
 *     Function :callFlowFromPhyToLwrMac 
 *
 *     Function call Flow From Phy To LwrMac
 *
 *  @param[in]  Pst     *pst, Post structure of the primitive.
 *
 *  @return  void
 * ****************************************************************/
#ifdef INTEL_FAPI
void callFlowFromPhyToLwrMac(uint16_t msgId)
{
   char message[100];
   switch(msgId)
   {
#ifdef INTEL_TIMER_MODE
      case FAPI_VENDOR_EXT_UL_IQ_SAMPLES:
         strcpy(message,"FAPI_VENDOR_EXT_UL_IQ_SAMPLES");
         break;
#endif
      case FAPI_PARAM_RESPONSE:
         strcpy(message,"FAPI_PARAM_RESPONSE");
         break;
      case FAPI_CONFIG_RESPONSE:
         strcpy(message,"FAPI_CONFIG_RESPONSE");
         break;
      case FAPI_SLOT_INDICATION:
         strcpy(message,"FAPI_SLOT_INDICATION");
         break;
      case FAPI_ERROR_INDICATION:
         strcpy(message,"FAPI_ERROR_INDICATION");
         break;
      case FAPI_RX_DATA_INDICATION:
         strcpy(message,"FAPI_RX_DATA_INDICATION");
         break;
      case FAPI_CRC_INDICATION:
         strcpy(message,"FAPI_CRC_INDICATION");
         break;
      case FAPI_UCI_INDICATION:
         strcpy(message,"FAPI_UCI_INDICATION");
         break;
      case FAPI_SRS_INDICATION:
         strcpy(message,"FAPI_SRS_INDICATION");
         break;
      case FAPI_RACH_INDICATION:
         strcpy(message,"FAPI_RACH_INDICATION");
         break;
      case FAPI_STOP_INDICATION:
         strcpy(message,"FAPI_STOP_INDICATION");
         break;
   }
   DU_LOG("\nCall Flow: PHY -> ENTLWRMAC : %s\n",message);
}
#endif
/*******************************************************************
 *
 * @brief O-RAN @ OAI Processes P7 message from PHY
 *
 * @details
 *
 *    Function : procPhyMessages
 *
 *    Functionality: Processes message from PHY
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
/*
void ORAN_OAI_procP7_message(nfapi_p7_message_header_t header, void *pRecvMsg, int recvMsgLen, vnf_p7_t* vnf_p7)
{
   switch (header.message_id)
   {
      case NFAPI_NR_PHY_MSG_TYPE_SLOT_INDICATION:
	 {
	    if(lwrMacCb.phyState == PHY_STATE_CONFIGURED)
	    {
	       DU_LOG("\nINFO  -->  LWR_MAC: PHY has moved to running state");
	       lwrMacCb.phyState = PHY_STATE_RUNNING;
	       lwrMacCb.cellCb[0].state = PHY_STATE_RUNNING;
	    }
       if (pRecvMsg == NULL || vnf_p7 == NULL)
       {
          DU_LOG("%s: ORAN_OAI_procP7_message NULL parameters\n");
       }
       else
	    oran_oai_nfapi_slot_ind_t *slotInd = NULLP;
	    slotInd  = (oran_oai_nfapi_slot_ind_t *)pRecvMsg;
	    procSlotInd(slotInd);
	    break;
    }
      case FAPI_ERROR_INDICATION:
	 {
	    break;
	 }
      case FAPI_RX_DATA_INDICATION:
	 {
	    oran_oai_nfapi_rx_data_indication_t *rxDataInd;
	    rxDataInd = (oran_oai_nfapi_rx_data_indication_t *)pRecvMsg;
	    procRxDataInd(rxDataInd);
	    break;
	 }  
      case FAPI_CRC_INDICATION:
	 {
	    oran_oai_nfapi_crc_ind_t  *crcInd;
	    crcInd = (oran_oai_nfapi_crc_ind_t *)pRecvMsg;
	    procCrcInd(crcInd);
	    break;
	 }  
      case FAPI_UCI_INDICATION:
	 {
	    oran_oai_nfapi_uci_indication_t *phyUciInd = NULLP;
	    phyUciInd = (oran_oai_nfapi_uci_indication_t*)pRecvMsg;
	    procUciInd(phyUciInd);
	    break;
	 }
      case FAPI_SRS_INDICATION:
	 {
	    break;
	 }  
      case FAPI_RACH_INDICATION:
	 {
	    oran_oai_nfapi_rach_indication_t  *rachInd;
	    rachInd = (oran_oai_nfapi_rach_indication_t *)pRecvMsg;
	    procRachInd(rachInd);
	    break;
	 }
      case FAPI_STOP_INDICATION:
	 {
	    DU_LOG("\nINFO  -->  LWR_MAC: Handling Stop Indication");
	    procStopInd();
	    break;       
	 }      
   }
}
*/
/*******************************************************************
 *
 * @brief Processes message from PHY
 *
 * @details
 *
 *    Function : procPhyMessages
 *
 *    Functionality: Processes message from PHY
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
#ifdef INTEL_FAPI
void procPhyMessages(uint16_t msgType, uint32_t msgSize, void *msg)
{

   /* extract the header */
   fapi_msg_t *header = NULLP;
   header = (fapi_msg_t *)msg;

#ifdef CALL_FLOW_DEBUG_LOG 
   callFlowFromPhyToLwrMac(header->msg_id);
#endif
   switch(header->msg_id)
   {
#ifdef INTEL_TIMER_MODE
      case FAPI_VENDOR_EXT_UL_IQ_SAMPLES:
         {
            DU_LOG("\nDEBUG  -->  LWR_MAC: Received FAPI_VENDOR_EXT_UL_IQ_SAMPLES");
            //send config req
            uint16_t cellId = 1;
            sendToLowerMac(CONFIG_REQUEST, 0, (void *)&cellId);
            break;
         } 
#endif
      case FAPI_PARAM_RESPONSE:
	 {
            sendToLowerMac(PARAM_RESPONSE, msgSize, msg);
	    break;
	 }
      case FAPI_CONFIG_RESPONSE:
	 {
	    sendToLowerMac(CONFIG_RESPONSE, msgSize, msg);
	    break;
	 }
      case FAPI_SLOT_INDICATION:
	 {
	    if(lwrMacCb.phyState == PHY_STATE_CONFIGURED)
	    {
	       DU_LOG("\nINFO  -->  LWR_MAC: PHY has moved to running state");
	       lwrMacCb.phyState = PHY_STATE_RUNNING;
	       lwrMacCb.cellCb[0].state = PHY_STATE_RUNNING;
	    }

	    fapi_slot_ind_t *slotInd = NULLP;
	    slotInd  = (fapi_slot_ind_t *)msg;
	    procSlotInd(slotInd);
	    break;
	 }
      case FAPI_ERROR_INDICATION:
	 {
	    break;
	 }
      case FAPI_RX_DATA_INDICATION:
	 {
	    fapi_rx_data_indication_t *rxDataInd;
	    rxDataInd = (fapi_rx_data_indication_t *)msg;
	    procRxDataInd(rxDataInd);
	    break;
	 }  
      case FAPI_CRC_INDICATION:
	 {
	    fapi_crc_ind_t  *crcInd;
	    crcInd = (fapi_crc_ind_t *)msg;
	    procCrcInd(crcInd);
	    break;
	 }  
      case FAPI_UCI_INDICATION:
	 {
	    fapi_uci_indication_t *phyUciInd = NULLP;
	    phyUciInd = (fapi_uci_indication_t*)msg;
	    procUciInd(phyUciInd);
	    break;
	 }
      case FAPI_SRS_INDICATION:
	 {
	    break;
	 }  
      case FAPI_RACH_INDICATION:
	 {
	    fapi_rach_indication_t  *rachInd;
	    rachInd = (fapi_rach_indication_t *)msg;
	    procRachInd(rachInd);
	    break;
	 }
      case FAPI_STOP_INDICATION:
	 {
	    DU_LOG("\nINFO  -->  LWR_MAC: Handling Stop Indication");
	    procStopInd();
	    break;
	 }  
   }
#endif
}

/**********************************************************************
  End of file
 **********************************************************************/
