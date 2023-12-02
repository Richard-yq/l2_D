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
#include "lrg.x"
#include "du_app_mac_inf.h"
#include "mac_sch_interface.h"
#include "lwr_mac_upr_inf.h"
#include "rlc_mac_inf.h"
#include "mac.h"
#include "mac_upr_inf_api.h"
#include "lwr_mac.h"
#include "lwr_mac_fsm.h"
#include "mac_utils.h"
#include "../nfapi/queue_t.h"
#include "../nfapi/assertions.h"
#include "../nfapi/nfapi_nr_interface_scf.h"

/* function pointers for packing slot ind from mac to sch */
MacSchSlotIndFunc macSchSlotIndOpts[] =
{
   packMacSchSlotInd,
   MacSchSlotInd,
   packMacSchSlotInd
};

/**
 * @brief process DL allocation from scheduler
 *
 * @details
 *
 *     Function : MacProcDlAlloc 
 *      
 *      This function copied dl sch info in the mac slot info
 *           
 *  @param[in]  Pst            *pst
 *  @param[in]  DL allocation from scheduler
 *  @return
 *      -# ROK 
 *      -# RFAILED 
 **/
uint8_t MacProcDlAlloc(Pst *pst, DlSchedInfo *dlSchedInfo)
{
   uint8_t   schInfoIdx = 0;
   uint8_t   ueId = 0, ueIdx = 0;
   uint16_t  cellIdx = 0;
   MacDlSlot *currDlSlot = NULLP;

#ifdef CALL_FLOW_DEBUG_LOG
   DU_LOG("\nCall Flow: ENTSCH -> ENTMAC : EVENT_DL_SCH_INFO\n");
#endif
   if(dlSchedInfo != NULLP)
   {
      GET_CELL_IDX(dlSchedInfo->cellId, cellIdx);
      if(dlSchedInfo->isBroadcastPres)
      {
         currDlSlot = &macCb.macCell[cellIdx]->\
                      dlSlot[dlSchedInfo->schSlotValue.broadcastTime.slot];
         currDlSlot->dlInfo.isBroadcastPres = true;
         memcpy(&currDlSlot->dlInfo.brdcstAlloc, &dlSchedInfo->brdcstAlloc, sizeof(DlBrdcstAlloc));
         currDlSlot->dlInfo.brdcstAlloc.sib1Alloc.sib1PdcchCfg.dci.pdschCfg = \
                                            &currDlSlot->dlInfo.brdcstAlloc.sib1Alloc.sib1PdschCfg;
      }

      for(ueIdx=0; ueIdx < MAX_NUM_UE; ueIdx++)
      {
         if(dlSchedInfo->rarAlloc[ueIdx] != NULLP)
         {
            currDlSlot = &macCb.macCell[cellIdx]->dlSlot[dlSchedInfo->schSlotValue.rarTime.slot];
            currDlSlot->dlInfo.rarAlloc[ueIdx] = dlSchedInfo->rarAlloc[ueIdx];

            /* MUXing of RAR */
            fillRarPdu(&currDlSlot->dlInfo.rarAlloc[ueIdx]->rarInfo);
         }

         if(dlSchedInfo->dlMsgAlloc[ueIdx] != NULLP)
         {
            currDlSlot = &macCb.macCell[cellIdx]->\
                         dlSlot[dlSchedInfo->schSlotValue.dlMsgTime.slot];
            currDlSlot->dlInfo.dlMsgAlloc[ueIdx] = dlSchedInfo->dlMsgAlloc[ueIdx]; /* copy msg4 alloc pointer in MAC slot info */
            currDlSlot->dlInfo.cellId = dlSchedInfo->cellId;

            /* Check if the downlink pdu is msg4 */
            for(schInfoIdx=0; schInfoIdx < dlSchedInfo->dlMsgAlloc[ueIdx]->numSchedInfo; schInfoIdx++)
            {
               if(dlSchedInfo->dlMsgAlloc[ueIdx]->dlMsgSchedInfo[schInfoIdx].dlMsgInfo.isMsg4Pdu)
               {
                  GET_UE_ID(dlSchedInfo->dlMsgAlloc[ueIdx]->dlMsgSchedInfo[schInfoIdx].dlMsgInfo.crnti, ueId);
                  ueIdx = ueId -1;
                  macCb.macCell[cellIdx]->macRaCb[ueIdx].msg4TbSize = \
                     dlSchedInfo->dlMsgAlloc[ueIdx]->dlMsgSchedInfo[schInfoIdx].dlMsgPdschCfg.codeword[0].tbSize;
               }
               else
               {
                  memcpy(&currDlSlot->dlInfo.schSlotValue, &dlSchedInfo->schSlotValue, sizeof(SchSlotValue));
                  /* Send LC schedule result to RLC */
                  if((dlSchedInfo->dlMsgAlloc[ueIdx]->dlMsgSchedInfo[schInfoIdx].pduPres == PDSCH_PDU) ||
                        (dlSchedInfo->dlMsgAlloc[ueIdx]->dlMsgSchedInfo[schInfoIdx].pduPres == BOTH))
                     sendSchedRptToRlc(currDlSlot->dlInfo, dlSchedInfo->schSlotValue.dlMsgTime, ueIdx, schInfoIdx);
               }
            }
         }
      }

      if(dlSchedInfo->ulGrant != NULLP)
      {
         currDlSlot = &macCb.macCell[cellIdx]->\
                      dlSlot[dlSchedInfo->schSlotValue.ulDciTime.slot];
         currDlSlot->dlInfo.ulGrant = dlSchedInfo->ulGrant;
      }
   }
   return ROK;
}

/**
 * @brief Forming anf filling the MUX Pdu
 * @details
 *
 *     Function : fillMsg4Pdu
 * 
 *      Forming and filling of Msg4Pdu
 *           
 *  @param[in]  DlMsgAlloc  *msg4Alloc
 *  @return  void
 **/
void fillMsg4Pdu(uint16_t cellId, DlMsgSchInfo *msg4SchInfo)
{
   uint8_t   ueId = 0, ueIdx = 0;
   uint16_t  cellIdx;
   uint16_t  msg4TxPduLen;
   MacDlData msg4DlData;
   MacCeInfo  macCeData;

   GET_CELL_IDX(cellId, cellIdx);

   memset(&msg4DlData, 0, sizeof(MacDlData));
   memset(&macCeData, 0, sizeof(MacCeInfo));

   GET_UE_ID(msg4SchInfo->dlMsgInfo.crnti, ueId);
   ueIdx = ueId -1;

   if(macCb.macCell[cellIdx] == NULLP)
   {
      DU_LOG("\nERROR -->  MAC: Cell Id[%d] not found", cellId);
      return;
   }

   if(macCb.macCell[cellIdx]->macRaCb[ueIdx].msg4Pdu != NULLP)
   {
      MAC_ALLOC(msg4DlData.pduInfo[msg4DlData.numPdu].dlPdu, macCb.macCell[cellIdx]->macRaCb[ueIdx].msg4PduLen);
      if(msg4DlData.pduInfo[msg4DlData.numPdu].dlPdu != NULLP)
      {
         msg4TxPduLen = macCb.macCell[cellIdx]->macRaCb[ueIdx].msg4TbSize - TX_PAYLOAD_HDR_LEN;

         fillMsg4DlData(&msg4DlData, macCb.macCell[cellIdx]->macRaCb[ueIdx].msg4PduLen, \
            macCb.macCell[cellIdx]->macRaCb[ueIdx].msg4Pdu);
         fillMacCe(&macCeData, macCb.macCell[cellIdx]->macRaCb[ueIdx].msg3Pdu);
         /* Forming Mux Pdu */
         macCb.macCell[cellIdx]->macRaCb[ueIdx].msg4TxPdu = NULLP;
         MAC_ALLOC(macCb.macCell[cellIdx]->macRaCb[ueIdx].msg4TxPdu, msg4TxPduLen);
         if(macCb.macCell[cellIdx]->macRaCb[ueIdx].msg4TxPdu != NULLP)
         {
            memset(macCb.macCell[cellIdx]->macRaCb[ueIdx].msg4TxPdu, 0, msg4TxPduLen);
            macMuxPdu(&msg4DlData, &macCeData, macCb.macCell[cellIdx]->macRaCb[ueIdx].msg4TxPdu, msg4TxPduLen);
         }
         else
         {
            DU_LOG("\nERROR  -->  MAC: Failed allocating memory for msg4TxPdu");
         }
         /* Free memory allocated */
         MAC_FREE(msg4DlData.pduInfo[msg4DlData.numPdu-1].dlPdu, macCb.macCell[cellIdx]->macRaCb[ueIdx].msg4PduLen);
         msg4DlData.numPdu--;
      }
   }

   /* storing msg4 Pdu in macDlSlot */
   if(macCb.macCell[cellIdx]->macRaCb[ueIdx].msg4TxPdu)
   {
      msg4SchInfo->dlMsgInfo.dlMsgPduLen = msg4TxPduLen;
      MAC_ALLOC(msg4SchInfo->dlMsgInfo.dlMsgPdu, msg4SchInfo->dlMsgInfo.dlMsgPduLen);
      if(msg4SchInfo->dlMsgInfo.dlMsgPdu != NULLP)
      {
         memcpy(msg4SchInfo->dlMsgInfo.dlMsgPdu, macCb.macCell[cellIdx]->macRaCb[ueIdx].msg4TxPdu, \
               msg4SchInfo->dlMsgInfo.dlMsgPduLen);
      }
   }
   else
   {
      DU_LOG("\nERROR  -->  MAC: Failed at fillMsg4Pdu()");
   }
}

/**
 * @brief Builds and Send the Muxed Pdu to Lower MAC
 *
 * @details
 *
 *     Function : buildAndSendMuxPdu
 * 
 *      Build and Sends the Muxed Pdu to Lower MAC.
 *           
 *  @param[in]  SlotTimingInfo    *slotInd
 *  @return  void
 **/

void buildAndSendMuxPdu(SlotTimingInfo currTimingInfo)
{
   uint8_t   ueIdx;
   uint8_t   schInfoIdx;
   uint16_t  cellIdx;
   MacDlSlot *currDlSlot = NULLP;
   SlotTimingInfo muxTimingInfo;
   memset(&muxTimingInfo, 0, sizeof(SlotTimingInfo));

   GET_CELL_IDX(currTimingInfo.cellId, cellIdx);

   ADD_DELTA_TO_TIME(currTimingInfo, muxTimingInfo, PHY_DELTA_DL);
   currDlSlot = &macCb.macCell[cellIdx]->dlSlot[muxTimingInfo.slot];

   for(ueIdx=0; ueIdx<MAX_NUM_UE; ueIdx++)
   {
      if(currDlSlot->dlInfo.dlMsgAlloc[ueIdx])
      {
         for(schInfoIdx=0; schInfoIdx<currDlSlot->dlInfo.dlMsgAlloc[ueIdx]->numSchedInfo; schInfoIdx++)
         {
            if((currDlSlot->dlInfo.dlMsgAlloc[ueIdx]->dlMsgSchedInfo[schInfoIdx].dlMsgInfo.isMsg4Pdu) &&
                  ((currDlSlot->dlInfo.dlMsgAlloc[ueIdx]->dlMsgSchedInfo[schInfoIdx].pduPres == PDSCH_PDU) ||
                   (currDlSlot->dlInfo.dlMsgAlloc[ueIdx]->dlMsgSchedInfo[schInfoIdx].pduPres == BOTH)))
            {
               fillMsg4Pdu(currTimingInfo.cellId, &currDlSlot->dlInfo.dlMsgAlloc[ueIdx]->dlMsgSchedInfo[schInfoIdx]);
            }
         }
      }
   }
}

/**
 * @brief Transmission time interval indication from PHY.
 *
 * @details
 *
 *     Function : sendSlotIndMacToSch
 * 
 *      This API is invoked by MAC to send slot ind to scheduler.
 *           
 *  @param[in]  SlotTimingInfo    *slotInd
 *  @return  
 *      -# ROK 
 *      -# RFAILED 
 **/
int sendSlotIndMacToSch(SlotTimingInfo *slotInd)
{
   printf("\n[MWNL] %s\n", __FUNCTION__);
   /* fill Pst structure to send to lwr_mac to MAC */
   Pst pst;

   FILL_PST_MAC_TO_SCH(pst, EVENT_SLOT_IND_TO_SCH);
   return(*macSchSlotIndOpts[pst.selector])(&pst,slotInd);
}

/*******************************************************************
 *
 * @brief Send cell up indication to DU APP
 *
 * @details
 *
 *    Function : sendCellUpIndMacToDuApp
 *
 *    Functionality:
 *       Send cell up indication to DU APP
 *
 * @params[in] Cell Up indication info 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
int sendCellUpIndMacToDuApp(uint16_t cellId)
{
   Pst pst;
   uint16_t ret;
   OduCellId *oduCellId;

   /*  Allocate sharable memory */
   MAC_ALLOC_SHRABL_BUF(oduCellId, sizeof(OduCellId));
   if(!oduCellId)
   {
      DU_LOG("\nERROR  -->  MAC : Memory allocation failed for cell up indication");
      return RFAILED;
   }
   oduCellId->cellId = cellId;

   /* Fill Pst */
   FILL_PST_MAC_TO_DUAPP(pst, EVENT_MAC_CELL_UP_IND);

   ret = MacDuAppCellUpInd(&pst, oduCellId);
   if(ret != ROK)
   {
      DU_LOG("\nERROR  -->  MAC: Failed to send cell up indication to DU APP");
      MAC_FREE_SHRABL_BUF(MAC_MEM_REGION, MAC_POOL, oduCellId, sizeof(OduCellId));
   }

   return ret;
} /* sendCellUpIndMacToDuApp */

/*******************************************************************
 *
 * @brief Process slot indication at MAC
 *
 * @details
 *
 *    Function : macProcSlotInd
 *
 *    Functionality: Process slot indication at MAC
 *
 * @params[in] Slot indication info
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t macProcSlotInd(SlotTimingInfo curr_slotInd)
{
   printf("[MWNL] %s\n", __FUNCTION__);
   uint16_t  cellIdx = 0;
   SlotTimingInfo target_slotInd;
   GET_CELL_IDX(curr_slotInd.cellId, cellIdx);
   
   if(macCb.macCell[cellIdx] == NULLP)
   {
      DU_LOG("ERROR  --> MAC : macProcSlotInd(): \
               CellId[%d] does not exist. Error occurred at SFN [%d] Slot [%d]",\
               target_slotInd.cellId, target_slotInd.sfn, target_slotInd.slot);
      DU_LOG("ERROR  --> MAC : macProcSlotInd(): \
               CellId[%d] does not exist. Error occurred at SFN [%d] Slot [%d]",\
               curr_slotInd.cellId, curr_slotInd.sfn, curr_slotInd.slot);
      return RFAILED;
   }

   /* Store current time info */
   macCb.macCell[cellIdx]->currTime.cellId = curr_slotInd.cellId;
   macCb.macCell[cellIdx]->currTime.slot = curr_slotInd.slot;
   macCb.macCell[cellIdx]->currTime.sfn = curr_slotInd.sfn;

   target_slotInd.cellId = curr_slotInd.cellId;
   target_slotInd.module_id = curr_slotInd.module_id;

   ADD_DELTA_TO_TIME(curr_slotInd, target_slotInd, ORAN_OAI_VNF_ADVANCE_SCH); // Add advance slot number
   /* Mux Pdu for Msg4 */
   buildAndSendMuxPdu(target_slotInd);

   /* Trigger for DL TTI REQ */
   // fillDlTtiReq(slotInd);
   ORAN_OAI_fillDlTtiReq(target_slotInd);

   return ROK;
}  /* macProcSlotInd */

/**
 * @brief Matching crc SFN & Slot.
 *
 * @details
 *
 *     Function : crc_sfn_slot_matcher 
 *      
 *      This API is from OAI used to check the crc SFN slot
 *           
 *  @param[in]  wanted              *wanted
 *  @param[in]  candidate           *candidate 
 * 
 *  @return  
 *      -# ROK 
 *      -# RFAILED 
 **/
static bool crc_sfn_slot_matcher(void *wanted, void *candidate)
{
  nfapi_p7_message_header_t *msg = candidate;
  int sfn_sf = *(int*)wanted;

  switch (msg->message_id)
  {
    case NFAPI_NR_PHY_MSG_TYPE_CRC_INDICATION:
    {
      nfapi_nr_crc_indication_t *ind = candidate;
      return NFAPI_SFNSLOT2SFN(sfn_sf) == ind->sfn && NFAPI_SFNSLOT2SLOT(sfn_sf) == ind->slot;
    }

    default:
      DU_LOG("[MWNL] sfn_slot_match bad ID: %d\n", msg->message_id);

  }
  return false;
}


static void remove_rx_pdu(nfapi_nr_rx_data_indication_t *rx_ind, int index) {
  AssertFatal(index >= 0, "Invalid index %d\n", index);
  AssertFatal(index < rx_ind->number_of_pdus, "Invalid index %d\n", index);
  AssertFatal(rx_ind->number_of_pdus > 0, "Invalid rx_ind->number_of_pdus %d\n", rx_ind->number_of_pdus);

  memmove(rx_ind->pdu_list + index,
          rx_ind->pdu_list + index + 1,
          sizeof(*rx_ind->pdu_list) * (rx_ind->number_of_pdus - index - 1));
  rx_ind->number_of_pdus--;
}


static bool crc_ind_has_rnti(nfapi_nr_crc_indication_t *crc_ind, uint16_t rnti) {
  for (int i = 0; i < crc_ind->number_crcs; i++) {
    if (rnti == crc_ind->crc_list[i].rnti) {
      return true;
    }
  }
  return false;
}


static bool rx_ind_has_rnti(nfapi_nr_rx_data_indication_t *rx_ind, uint16_t rnti) {
  for (int i = 0; i < rx_ind->number_of_pdus; i++) {
    if (rnti == rx_ind->pdu_list[i].rnti) {
      return true;
    }
  }
  return false;
}

static void remove_crc_pdu(nfapi_nr_crc_indication_t *crc_ind, int index) {
  AssertFatal(index >= 0, "Invalid index %d\n", index);
  AssertFatal(index < crc_ind->number_crcs, "Invalid index %d\n", index);
  AssertFatal(crc_ind->number_crcs > 0, "Invalid crc_ind->number_crcs %d\n", crc_ind->number_crcs);

  memmove(crc_ind->crc_list + index,
          crc_ind->crc_list + index + 1,
          sizeof(*crc_ind->crc_list) * (crc_ind->number_crcs - index - 1));
  crc_ind->number_crcs--;
}

static void match_crc_rx_pdu(nfapi_nr_rx_data_indication_t *rx_ind, nfapi_nr_crc_indication_t *crc_ind) {
  AssertFatal(crc_ind->number_crcs > 0 &&  rx_ind->number_of_pdus > 0,
              "Invalid number of crc_ind->number_crcs %d or rx_ind->number_of_pdus %d\n",
              crc_ind->number_crcs, rx_ind->number_of_pdus);
  if (crc_ind->number_crcs > rx_ind->number_of_pdus) {
    int num_unmatched_crcs = 0;
    nfapi_nr_crc_indication_t *crc_ind_unmatched = calloc(1, sizeof(*crc_ind_unmatched));
    crc_ind_unmatched->header = crc_ind->header;
    crc_ind_unmatched->sfn = crc_ind->sfn;
    crc_ind_unmatched->slot = crc_ind->slot;
    crc_ind_unmatched->number_crcs = crc_ind->number_crcs - rx_ind->number_of_pdus;
    crc_ind_unmatched->crc_list = calloc(crc_ind_unmatched->number_crcs, sizeof(nfapi_nr_crc_t));
    for (int i = 0; i < crc_ind->number_crcs; i++) {
      if (!rx_ind_has_rnti(rx_ind, crc_ind->crc_list[i].rnti)) {
          DU_LOG("[MWNL] crc_ind->crc_list[%d].rnti %x does not match any rx_ind pdu rnti\n",
                i, crc_ind->crc_list[i].rnti);
          crc_ind_unmatched->crc_list[num_unmatched_crcs] = crc_ind->crc_list[i];
          num_unmatched_crcs++;
          remove_crc_pdu(crc_ind, i);
      }
      if (crc_ind->number_crcs == rx_ind->number_of_pdus) {
        break;
      }
    }
    AssertFatal(crc_ind_unmatched->number_crcs == num_unmatched_crcs, "crc_ind num_pdus %d doesnt match %d\n",
                crc_ind_unmatched->number_crcs, num_unmatched_crcs);
    if (!requeue(&gnb_crc_ind_queue, crc_ind_unmatched))
    {
      DU_LOG("[MWNL] requeue failed for crc_ind_unmatched.\n");
      free_and_zero(crc_ind_unmatched->crc_list);
      free_and_zero(crc_ind_unmatched);
    }
  }
  else if (crc_ind->number_crcs < rx_ind->number_of_pdus) {
    int num_unmatched_rxs = 0;
    nfapi_nr_rx_data_indication_t *rx_ind_unmatched = calloc(1, sizeof(*rx_ind_unmatched));
    rx_ind_unmatched->header = rx_ind->header;
    rx_ind_unmatched->sfn = rx_ind->sfn;
    rx_ind_unmatched->slot = rx_ind->slot;
    rx_ind_unmatched->number_of_pdus = rx_ind->number_of_pdus - crc_ind->number_crcs;
    rx_ind_unmatched->pdu_list = calloc(rx_ind_unmatched->number_of_pdus, sizeof(nfapi_nr_pdu_t));
    for (int i = 0; i < rx_ind->number_of_pdus; i++) {
      if (!crc_ind_has_rnti(crc_ind, rx_ind->pdu_list[i].rnti)) {
        DU_LOG("[MWNL] rx_ind->pdu_list[%d].rnti %d does not match any crc_ind pdu rnti\n",
              i, rx_ind->pdu_list[i].rnti);
        rx_ind_unmatched->pdu_list[num_unmatched_rxs] = rx_ind->pdu_list[i];
        num_unmatched_rxs++;
        remove_rx_pdu(rx_ind, i);
      }
      if (rx_ind->number_of_pdus == crc_ind->number_crcs) {
        break;
      }
    }
    AssertFatal(rx_ind_unmatched->number_of_pdus == num_unmatched_rxs, "rx_ind num_pdus %d doesnt match %d\n",
                rx_ind_unmatched->number_of_pdus, num_unmatched_rxs);
    if (!requeue(&gnb_rx_ind_queue, rx_ind_unmatched))
    {
      DU_LOG("[MWNL] requeue failed for rx_ind_unmatched.\n");
      free_and_zero(rx_ind_unmatched->pdu_list);
      free_and_zero(rx_ind_unmatched);
    }
  }
  else {
    DU_LOG("[MWNL] The number of crc pdus %d = the number of rx pdus %d\n",
          crc_ind->number_crcs, rx_ind->number_of_pdus);
  }
}


static void free_unqueued_nfapi_indications(nfapi_nr_rach_indication_t *rach_ind,
                                            nfapi_nr_uci_indication_t *uci_ind,
                                            nfapi_nr_rx_data_indication_t *rx_ind,
                                            nfapi_nr_crc_indication_t *crc_ind) {
  if (rach_ind && rach_ind->number_of_pdus > 0)
  {
    for(int i = 0; i < rach_ind->number_of_pdus; i++)
    {
      free_and_zero(rach_ind->pdu_list[i].preamble_list);
    }
    free_and_zero(rach_ind->pdu_list);
    free_and_zero(rach_ind);
  }
  if (uci_ind && uci_ind->num_ucis > 0)
  {
    for (int i = 0; i < uci_ind->num_ucis; i++) {
      switch (uci_ind->uci_list[i].pdu_type) {
        case NFAPI_NR_UCI_FORMAT_0_1_PDU_TYPE:
          if (uci_ind->uci_list[i].pucch_pdu_format_0_1.harq) {
            free_and_zero(uci_ind->uci_list[i].pucch_pdu_format_0_1.harq->harq_list);
          }
          free_and_zero(uci_ind->uci_list[i].pucch_pdu_format_0_1.harq);
          free_and_zero(uci_ind->uci_list[i].pucch_pdu_format_0_1.sr);
          break;

        case NFAPI_NR_UCI_FORMAT_2_3_4_PDU_TYPE:
          free_and_zero(uci_ind->uci_list[i].pucch_pdu_format_2_3_4.harq.harq_payload);
          free_and_zero(uci_ind->uci_list[i].pucch_pdu_format_2_3_4.csi_part1.csi_part1_payload);
          free_and_zero(uci_ind->uci_list[i].pucch_pdu_format_2_3_4.csi_part2.csi_part2_payload);
          break;
      }
    }
    free_and_zero(uci_ind->uci_list);
    free_and_zero(uci_ind);
  }
  if (rx_ind && rx_ind->number_of_pdus > 0)
  {
    free_and_zero(rx_ind->pdu_list);
    free_and_zero(rx_ind);
  }
  if (crc_ind && crc_ind->number_crcs > 0)
  {
    free_and_zero(crc_ind->crc_list);
    free_and_zero(crc_ind);
  }
}

/*******************************************************************
 *
 * @brief Processes Rach Indication from PHY and sends to MAC
 *
 * @details
 *
 *    Function : macprocRachInd
 *
 *    Functionality:
 *         Processes Rach Indication from PHY and sends to MAC
 *
 * @params[in] nfapi_nr_rach_indication_t pointer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t macprocRachInd(nfapi_nr_rach_indication_t *nfapi_rach_ind)
{
   uint8_t      pduIdx, ret;
   uint8_t      prmbleIdx;
   RachPduInfo  *rachPdu = NULLP;
   RachInd      *rachInd = NULLP;

   //MAC_ALLOC_SHRABL_BUF(rachInd, sizeof(RachInd));
   rachInd = malloc(sizeof(RachInd));
   if(!rachInd)
   {
      DU_LOG("\nERROR  -->  LWR_MAC : Memory Allocation failed in procRachInd");
      return RFAILED;
   }
   if(!nfapi_rach_ind->number_of_pdus)
   {
      DU_LOG("\nDEBUG  -->  LWR_MAC : No PDU in RACH.indication at [%d, %d]", nfapi_rach_ind->sfn, nfapi_rach_ind->slot);
      return ROK;
   }

   rachInd->cellId = lwrMacCb.cellCb[0].cellId;
   rachInd->timingInfo.sfn = nfapi_rach_ind->sfn;
   rachInd->timingInfo.slot = nfapi_rach_ind->slot;
   rachInd->numPdu = nfapi_rach_ind->number_of_pdus;
   for(pduIdx=0; pduIdx < rachInd->numPdu; pduIdx++)
   {
      rachPdu = &rachInd->rachPdu[pduIdx];
      rachPdu->pci = nfapi_rach_ind->pdu_list[pduIdx].phy_cell_id;
      rachPdu->symbolIdx = nfapi_rach_ind->pdu_list[pduIdx].symbol_index;
      rachPdu->slotIdx = nfapi_rach_ind->pdu_list[pduIdx].slot_index;
      rachPdu->freqIdx = nfapi_rach_ind->pdu_list[pduIdx].freq_index;
      rachPdu->numPream = nfapi_rach_ind->pdu_list[pduIdx].num_preamble;
      for(prmbleIdx=0; prmbleIdx<rachPdu->numPream; prmbleIdx++)
      {
         rachPdu->preamInfo[prmbleIdx].preamIdx = \
	    nfapi_rach_ind->pdu_list[pduIdx].preamble_list[prmbleIdx].preamble_index;
         rachPdu->preamInfo[prmbleIdx].timingAdv = \
	    nfapi_rach_ind->pdu_list[pduIdx].preamble_list[prmbleIdx].timing_advance;
      }
   }

ret = ORAN_OAI_fapiMacRachInd(rachPdu);
free(rachInd);

return ret;
}

/**
 * @brief Transmission Uci indication from PHY.
 *
 * @details
 *
 *     Function : macprocUciInd 
 *      
 *      This API is invoked by PHY to indicate UCI indication to MAC for a cell.
 *           
 *  @param[in]  Pst            *pst
 *  @param[in]  SuId           suId 
 *  @param[in]  SlotTimingInfo    *slotInd
 *  @return  
 *      -# ROK 
 *      -# RFAILED 
 **/
uint8_t macprocUciInd(nfapi_nr_uci_indication_t *nfapi_uci_ind)
{
   uint8_t pduIdx;
   uint8_t ret = ROK;
   UciInd  *macUciInd = NULLP;

   macUciInd = malloc(sizeof(UciInd));
   if(!macUciInd)
   {
      DU_LOG("\nERROR  -->  LWR_MAC: Memory Allocation failed in procUciInd");
      return RFAILED;
   }

   DU_LOG("\nDEBUG  -->  LWR_MAC: Processing UCI Indication");
   memset(macUciInd, 0, sizeof(UciInd));
   macUciInd->cellId = lwrMacCb.cellCb[0].cellId;
   macUciInd->slotInd.sfn = nfapi_uci_ind->sfn;
   macUciInd->slotInd.slot = nfapi_uci_ind->slot;
   macUciInd->numUcis = nfapi_uci_ind->num_ucis;

   for(pduIdx = 0; pduIdx < macUciInd->numUcis; pduIdx++)
   {
      macUciInd->pdus[pduIdx].pduType = nfapi_uci_ind->uci_list[pduIdx].pdu_type;
      switch(macUciInd->pdus[pduIdx].pduType)
      {
         case UCI_IND_PUSCH:
         break;
         case UCI_IND_PUCCH_F0F1:
         {
            UciPucchF0F1 *pduInfo = NULLP;
            macUciInd->pdus[pduIdx].pduSize = nfapi_uci_ind->uci_list[pduIdx].pdu_size;
            pduInfo = &macUciInd->pdus[pduIdx].uci.uciPucchF0F1;
            ret = fillUciIndPucchF0F1(pduInfo, &nfapi_uci_ind->uci_list[pduIdx].pucch_pdu_format_0_1);
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
      ret = ORAN_OAI_FapiMacUciInd(macUciInd);
   }
   else
   {
      DU_LOG("\nERROR  -->  LWR_MAC: Failed sending UCI Ind to MAC");
   }
   free(macUciInd);
   return ret;
}

/*******************************************************************
 *
 * @brief ORAN_OAI Handles Rx Data indication from PHY and sends to MAC
 *
 * @details
 *
 *    Function : macprocRxDataInd
 *
 *    Functionality:
 *      Handles Rx Data indication from PHY and sends to MAC
 *
 * @params[in] nfapi_nr_rx_data_indication_t message pointer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t macprocRxDataInd(nfapi_nr_rx_data_indication_t *nfapi_rxdataInd)
{
   uint8_t       pduIdx =0;
   uint8_t       ret;
   RxDataInd     *rxDataInd = NULLP;
   RxDataIndPdu  *pdu = NULLP; 
   MAC_ALLOC(rxDataInd, sizeof(RxDataInd));
   if(!rxDataInd)
   {
      DU_LOG("\nERROR  -->  LWR_MAC : Memory Allocation failed in procRxDataInd");
      return RFAILED;
   }   
   if(!nfapi_rxdataInd->number_of_pdus)
   {
      DU_LOG("\nDEBUG  -->  LWR_MAC : No PDU in RX_Data.indication at [%d, %d]", nfapi_rxdataInd->sfn, nfapi_rxdataInd->slot);
      return ROK;
   }

   rxDataInd->cellId = lwrMacCb.cellCb[0].cellId;
   rxDataInd->timingInfo.sfn = nfapi_rxdataInd->sfn;
   rxDataInd->timingInfo.slot = nfapi_rxdataInd->slot;
   rxDataInd->numPdus = nfapi_rxdataInd->number_of_pdus;

   for(pduIdx = 0; pduIdx < rxDataInd->numPdus; pduIdx++)
   {
      pdu = &rxDataInd->pdus[pduIdx];
      pdu->handle = nfapi_rxdataInd->pdu_list[pduIdx].handle;
      pdu->rnti = nfapi_rxdataInd->pdu_list[pduIdx].rnti;
      pdu->harqId = nfapi_rxdataInd->pdu_list[pduIdx].harq_id;
      pdu->pduLength = nfapi_rxdataInd->pdu_list[pduIdx].pdu_length;
      pdu->ul_cqi = nfapi_rxdataInd->pdu_list[pduIdx].ul_cqi;
      pdu->timingAdvance = nfapi_rxdataInd->pdu_list[pduIdx].timing_advance;
      pdu->rssi = nfapi_rxdataInd->pdu_list[pduIdx].rssi;

      MAC_ALLOC(pdu->pduData,pdu->pduLength);
      memcpy(pdu->pduData, nfapi_rxdataInd->pdu_list[pduIdx].pdu, pdu->pduLength);
   }

   ret = ORAN_OAI_fapiMacRxDataInd(rxDataInd);
   for(pduIdx = 0; pduIdx < rxDataInd->numPdus; pduIdx++)
   {
      MAC_FREE(pdu->pduData,pdu->pduLength);
   }
   MAC_FREE(rxDataInd, sizeof(RxDataInd));

   return ret;
}

/*******************************************************************
 *
 * @brief ORAN_OAI Handles CRC indication from PHY and sends to MAC
 *
 * @details
 *
 *    Function : macprocCrcInd
 *
 *    Functionality:
 *      Handles CRC indication from PHY and sends to MAC
 *
 * @params[in] nfapi_nr_crc_indication_t message pointer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t macprocCrcInd(nfapi_nr_crc_indication_t  *nfapiCrcInd)
{
   uint8_t      crcInfoIdx, ret;
   uint8_t      crcStatusIdx;
   CrcInfo      *crcIndInfo = NULLP;
   CrcInd       *crcInd = NULLP;

   MAC_ALLOC(crcInd, sizeof(CrcInd));
   if(!crcInd)
   {
      DU_LOG("\nERROR  -->  LWR_MAC : Memory Allocation failed in procCrcInd");
      return RFAILED;
   }
   if(!nfapiCrcInd->number_crcs)
   {
      DU_LOG("\nDEBUG  --> LWR_MAC : No CRC PDUs in CRC.indication at [%d, %d]", nfapiCrcInd->sfn, nfapiCrcInd->slot);
      return ROK;
   }

   crcInd->cellId = lwrMacCb.cellCb[0].cellId;
   crcInd->timingInfo.sfn = nfapiCrcInd->sfn;
   crcInd->timingInfo.slot = nfapiCrcInd->slot;
   crcInd->numCrc = nfapiCrcInd->number_crcs;

   for(crcInfoIdx = 0; crcInfoIdx < crcInd->numCrc; crcInfoIdx++)
   {
      crcIndInfo = &crcInd->crcInfo[crcInfoIdx];
      crcIndInfo->handle      = nfapiCrcInd->crc_list[crcInfoIdx].handle;
      crcIndInfo->rnti      = nfapiCrcInd->crc_list[crcInfoIdx].rnti;
      crcIndInfo->harqId      = nfapiCrcInd->crc_list[crcInfoIdx].harq_id;
      crcIndInfo->tbCrcStatus      = nfapiCrcInd->crc_list[crcInfoIdx].tb_crc_status;
      crcIndInfo->numCb      = nfapiCrcInd->crc_list[crcInfoIdx].num_cb;
      for(crcStatusIdx = 0; crcStatusIdx < crcIndInfo->numCb; crcStatusIdx++)
      {
         crcIndInfo->cbCrcStatus[crcStatusIdx] = \
	    nfapiCrcInd->crc_list[crcInfoIdx].cb_crc_status[crcStatusIdx];
      }
      crcIndInfo->ul_cqi  = nfapiCrcInd->crc_list[crcInfoIdx].ul_cqi;
      crcIndInfo->timingAdvance  = nfapiCrcInd->crc_list[crcInfoIdx].timing_advance;
      crcIndInfo->rssi = nfapiCrcInd->crc_list[crcInfoIdx].rssi;
   }

   ret = ORAN_OAI_fapiMacCrcInd(crcInd);
   MAC_FREE(crcInd, sizeof(CrcInd));
return ret;
}

/**
 * @brief Transmission time interval indication from PHY.
 *
 * @details
 *
 *     Function : fapiMacSlotInd 
 *      
 *      This API is invoked by PHY to indicate TTI indication to MAC for a cell.
 *           
 *  @param[in]  Pst            *pst
 *  @param[in]  SuId           suId 
 *  @param[in]  SlotTimingInfo    *slotInd
 *  @return  
 *      -# ROK 
 *      -# RFAILED 
 **/
uint8_t fapiMacSlotInd(Pst *pst, NR_UL_IND_t *UL_INFO)
{
   uint8_t               ret = ROK;
   uint16_t              cellIdx;
   volatile uint32_t     startTime=0;
   SlotTimingInfo *slotInd;
   // NR_IF_Module_t *ifi = nr_if_inst[module_id];
   
// #ifdef ODU_SLOT_IND_DEBUG_LOG
//    DU_LOG("\nDEBUG  -->  MAC : Slot Indication received. [%d : %d]", slotInd->sfn, slotInd->slot);
// #endif

   DU_LOG("MWNL  -->  MAC : Slot Indication received. [%d : %d]\n", slotInd->sfn, slotInd->slot);

   /*starting Task*/
   ODU_START_TASK(&startTime, PID_MAC_TTI_IND);

   gSlotCount++;

   if(gSlotCount == 1)
   {
	   GET_CELL_IDX(UL_INFO->cellId, cellIdx);
	   macCb.macCell[cellIdx]->state = CELL_STATE_UP;
   }

   nfapi_nr_rach_indication_t *rach_ind = NULL;
   nfapi_nr_uci_indication_t *uci_ind = NULL;
   nfapi_nr_rx_data_indication_t *rx_ind = NULL;
   nfapi_nr_crc_indication_t *crc_ind = NULL;

   if(gnb_rach_ind_queue.num_items > 0)
   {
      DU_LOG("[MWNL] gnb_rach_ind_queue size = %zu\n", gnb_rach_ind_queue.num_items);
      rach_ind = get_queue(&gnb_rach_ind_queue);
      AssertFatal(rach_ind->number_of_pdus > 0, "Invalid number of PDUs\n");
      UL_INFO->rach_ind = *rach_ind;
   }

   if(gnb_uci_ind_queue.num_items > 0)
   {
      DU_LOG("[MWNL] gnb_uci_ind_queue size = %zu\n", gnb_uci_ind_queue.num_items);
      uci_ind = get_queue(&gnb_uci_ind_queue);
      AssertFatal(uci_ind->num_ucis > 0, "Invalid number of PDUs\n");
      UL_INFO->uci_ind = *uci_ind;
   }

   if (gnb_rx_ind_queue.num_items > 0 && gnb_crc_ind_queue.num_items > 0)
   {
      DU_LOG("[MWNL] gnb_rx_ind_queue size = %zu and gnb_crc_ind_queue size = %zu\n", 
               gnb_rx_ind_queue.num_items, gnb_crc_ind_queue.num_items);

      rx_ind = get_queue(&gnb_rx_ind_queue);
      int sfn_slot = NFAPI_SFNSLOT2HEX(rx_ind->sfn, rx_ind->slot);
      crc_ind = unqueue_matching(&gnb_crc_ind_queue,
                                 MAX_QUEUE_SIZE,
                                 crc_sfn_slot_matcher,
                                 &sfn_slot);
      if (!crc_ind) {
        DU_LOG("[MWNL] No crc indication with the same SFN SLOT of rx indication %u %u\n", rx_ind->sfn, rx_ind->slot);
        requeue(&gnb_rx_ind_queue, rx_ind);
      }else {
        AssertFatal(rx_ind->number_of_pdus > 0, "Invalid number of PDUs\n");
        AssertFatal(crc_ind->number_crcs > 0, "Invalid number of PDUs\n");
        if (crc_ind->number_crcs != rx_ind->number_of_pdus) match_crc_rx_pdu(rx_ind, crc_ind); 
        UL_INFO->rx_ind = *rx_ind;
        UL_INFO->crc_ind = *crc_ind;
      }
   }

   macprocRachInd(&UL_INFO->rach_ind);
   macprocUciInd(&UL_INFO->uci_ind);
   macprocRxDataInd(&UL_INFO->rx_ind);
   macprocCrcInd(&UL_INFO->crc_ind);

   free_unqueued_nfapi_indications(rach_ind, uci_ind, rx_ind, crc_ind);

/* When testing L2 with Intel-L1, any changes specific to 
 * timer mode testing must be guarded under INTEL_TIMER_MODE*/
#ifndef INTEL_TIMER_MODE
   /* send slot indication to scheduler */
   slotInd->cellId = lwrMacCb.cellCb[0].cellId;
   slotInd->module_id = UL_INFO->module_id;
   slotInd->sfn = UL_INFO->frame;
   slotInd->slot = UL_INFO->slot;

   // MAC to Sch (排程)
   ret = sendSlotIndMacToSch(slotInd);
   //Transmission time interval indication from PHY
   if(ret != ROK)
   {
      DU_LOG("\nERROR  -->  MAC : Sending of slot ind msg from MAC to SCH failed");
      MAC_FREE_SHRABL_BUF(pst->region, pst->pool, UL_INFO, sizeof(NR_UL_IND_t));
      return ret;
   }

   // change gNB_dlach_ulsch_scheduler()
   ret = macProcSlotInd(*slotInd);
   //Process slot indication at MAC
   if(ret != ROK)
   {
      DU_LOG("\nERROR  -->  MAC : macProcSlotInd failed");
      MAC_FREE_SHRABL_BUF(pst->region, pst->pool, slotInd, sizeof(SlotTimingInfo));
      return ret;
   }
//

#endif

   /* send slot indication to du app */
   if(gSlotCount == 1)   
   {
      ret = sendCellUpIndMacToDuApp(slotInd->cellId);
      if(ret != ROK)
      {
         DU_LOG("\nERROR  -->  MAC :Sending of slot ind msg from MAC to DU APP failed");
         MAC_FREE_SHRABL_BUF(pst->region, pst->pool, slotInd, sizeof(SlotTimingInfo));
         return ret;
      }
   }

   /*stoping Task*/
   ODU_STOP_TASK(startTime, PID_MAC_TTI_IND);
   MAC_FREE_SHRABL_BUF(pst->region, pst->pool, slotInd, sizeof(SlotTimingInfo));

#ifdef INTEL_WLS_MEM
   lwrMacCb.phySlotIndCntr++;
   if(lwrMacCb.phySlotIndCntr > WLS_MEM_FREE_PRD)
   {
      lwrMacCb.phySlotIndCntr = 1;
   }
   freeWlsBlockList(lwrMacCb.phySlotIndCntr - 1);
#endif

   return ret;
}  /* fapiMacSlotInd */

/**********************************************************************
  End of file
 **********************************************************************/

