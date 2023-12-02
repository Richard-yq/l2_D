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
#ifndef _LWR_MAC_PHY_H_
#define _LWR_MAC_PHY_H_

#include "../nfapi/nfapi_interface.h"
#include "../nfapi/vnf_p7.h"
#include "../cm/common_def.h"

typedef enum
{
   MSG_OK,
   MSG_INVALID_STATE,
   MSG_INVALID_CONFIG,
   SFN_OUT_OF_SYNC,
   MSG_SLOT_ERR,
   MSG_BCH_MISSING,
   MSG_INVALID_SFN,
   MSG_UL_DCI_ERR,
   MSG_TX_ERR
}ErrorCode;

#ifdef INTEL_WLS_MEM
#define LWR_MAC_WLS_BUF_SIZE   32000      /* Size of WLS memory block */
#define EVT_START_WLS_RCVR     1

/* allocate static buffer from WLS memory */
#define WLS_MEM_ALLOC(_datPtr, _size)                        \
{                                                            \
   uint8_t _ret;                                             \
   _ret = SGetSBufWls(0, 0, (Data **)&_datPtr, _size);       \
   if(_ret == ROK)                                           \
      memset(_datPtr, 0, _size);                      \
   else                                                      \
      _datPtr = NULLP;                                       \
}                                                              

/* free a static buffer from WLS memory*/
#define WLS_MEM_FREE(_datPtr, _size)                         \
   if(_datPtr)                                               \
   {                                                         \
      SPutSBufWls(0, 0, (Data *)_datPtr, _size);             \
      _datPtr = NULLP;                                       \
   }

typedef struct wlsBlockToFree
{
   void     *ptr;
   uint32_t size;
}WlsBlockToFree;

CmLListCp wlsBlockToFreeList[WLS_MEM_FREE_PRD];

void freeWlsBlockList(uint8_t idx);
void LwrMacEnqueueWlsBlock();
void LwrMacRecvPhyMsg();
void LwrMacStartWlsRcvr();
#endif /* INTEL_WLS_MEM */

uint8_t LwrMacSendToL1(void *msg);
#endif
void ORAN_OAI_procP7_message(nfapi_p7_message_header_t header, void *pRecvMsg, int recvMsgLen, vnf_p7_t* vnf_p7);
uint8_t ORAN_OAI_procSlotInd(NR_UL_IND_t *UL_INFO);
uint8_t ORAN_OAI_procRxDataInd(nfapi_nr_rx_data_indication_t  *fapiRxDataInd);
uint8_t ORAN_OAI_procCrcInd(nfapi_nr_crc_indication_t  *fapiCrcInd);
uint8_t ORAN_OAI_procUciInd(nfapi_nr_uci_indication_t  *fapiUciInd);
uint8_t ORAN_OAI_procRachInd(nfapi_nr_rach_indication_t  *fapiRachInd);
/**********************************************************************
  End of file
 **********************************************************************/

