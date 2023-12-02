/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.1  (the "License"); you may not use this file
 * except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.openairinterface.org/?page_id=698
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *-------------------------------------------------------------------------------
 * For more information about the OpenAirInterface (OAI) Software Alliance:
 *      contact@openairinterface.org
 */

#define _GNU_SOURCE
#define TEST

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "nfapi_nr_interface_scf.h"
#include "nfapi_vnf_interface.h"
#include "nfapi_vnf.h"
#include "debug.h"
#include "assertions.h"

#include "vnf_intgr_mwnl.h"

// To get param response
extern nfapi_nr_param_response_scf_t *temp_resp; // Define in vnf.c


int pnf_nr_connection_indication_cb(nfapi_vnf_config_t *config, int p5_idx) {
  printf("\n[NFAPI VNF]  ->  pnf connection indication idx:%d", p5_idx);
  // oai_create_gnb();
  nfapi_nr_pnf_param_request_t req;
  memset(&req, 0, sizeof(req));
  req.header.message_id = NFAPI_NR_PHY_MSG_TYPE_PNF_PARAM_REQUEST;
  nfapi_nr_vnf_pnf_param_req(config, p5_idx, &req);
  return 0;
}

int pnf_disconnection_indication_cb(nfapi_vnf_config_t *config, int p5_idx) {
  printf("\n[NFAPI VNF]  ->  pnf disconnection indication idx:%d", p5_idx);
  vnf_info *vnf = (vnf_info *)(config->user_data);
  pnf_info *pnf = vnf->pnfs;
  phy_info *phy = pnf->phys;
  vnf_p7_info *p7_vnf = vnf->p7_vnfs;
  nfapi_vnf_p7_del_pnf((p7_vnf->config), phy->id);
  return 0;
}

int pnf_nr_param_resp_cb(nfapi_vnf_config_t *config, int p5_idx, nfapi_nr_pnf_param_response_t *resp) {
  printf("\n[NFAPI VNF]  ->  pnf param response idx:%d error:%d", p5_idx, resp->error_code);
  vnf_info *vnf = (vnf_info *)(config->user_data);
  pnf_info *pnf = vnf->pnfs;

  for(int i = 0; i < resp->pnf_phy.number_of_phys; ++i) {
    phy_info phy;
    memset(&phy,0,sizeof(phy));
    phy.index = resp->pnf_phy.phy[i].phy_config_index;
    printf("\n[NFAPI VNF]  ->  (PHY:%d) phy_config_idx:%d", i, resp->pnf_phy.phy[i].phy_config_index);
    nfapi_vnf_allocate_phy(config, p5_idx, &(phy.id));

    for(int j = 0; j < resp->pnf_phy.phy[i].number_of_rfs; ++j) {
      printf("\n[NFAPI VNF]  ->  (PHY:%d) (RF%d) %d", i, j, resp->pnf_phy.phy[i].rf_config[j].rf_config_index);
      phy.rfs[0] = resp->pnf_phy.phy[i].rf_config[j].rf_config_index;
    }

    pnf->phys[0] = phy;
  }
  
  nfapi_nr_pnf_config_request_t req;
  memset(&req, 0, sizeof(req));
  req.header.message_id = NFAPI_PNF_CONFIG_REQUEST;
  req.pnf_phy_rf_config.tl.tag = NFAPI_PNF_PHY_RF_TAG;
  req.pnf_phy_rf_config.number_phy_rf_config_info = 2; // DJP pnf.phys.size();
  printf("\nNFAPI\t->\tDJP:Hard coded num phy rf to 2");

  for(unsigned i = 0; i < 2; ++i) {
    req.pnf_phy_rf_config.phy_rf_config[i].phy_id = pnf->phys[i].id;
    req.pnf_phy_rf_config.phy_rf_config[i].phy_config_index = pnf->phys[i].index;
    req.pnf_phy_rf_config.phy_rf_config[i].rf_config_index = pnf->phys[i].rfs[0];
  }

  nfapi_nr_vnf_pnf_config_req(config, p5_idx, &req);
  return 0;
}

int pnf_nr_config_resp_cb(nfapi_vnf_config_t *config, int p5_idx, nfapi_nr_pnf_config_response_t *resp) {
  printf("\n[NFAPI VNF]  ->  pnf config response idx:%d resp[header[phy_id:%u message_id:%02x message_length:%u]]", p5_idx, resp->header.phy_id, resp->header.message_id, resp->header.message_length);

  if(1) {
    nfapi_nr_pnf_start_request_t req;
    memset(&req, 0, sizeof(req));
    req.header.phy_id = resp->header.phy_id;
    req.header.message_id = NFAPI_PNF_START_REQUEST;
    nfapi_nr_vnf_pnf_start_req(config, p5_idx, &req);
  } else {
    /* !!! There is no need to use vender extension !!! */
    // Rather than send the pnf_start_request we will demonstrate
    // sending a vendor extention message. The start request will be
    // send when the vendor extension response is received
    //vnf_info* vnf = (vnf_info*)(config->user_data);
    // vendor_ext_p5_req req;
    // memset(&req, 0, sizeof(req));
    // req.header.message_id = P5_VENDOR_EXT_REQ;
    // req.dummy1 = 45;
    // req.dummy2 = 1977;
    // nfapi_vnf_vendor_extension(config, p5_idx, &req.header);
  }
  return 0;
}


void *vnf_allocate(size_t size) {
  //return (void*)memory_pool::allocate(size);
  return (void *)malloc(size);
}

void vnf_deallocate(void *ptr) {
  //memory_pool::deallocate((uint8_t*)ptr);
  free(ptr);
}

static pthread_t vnf_start_pthread;
static pthread_t vnf_p7_start_pthread;

void *vnf_nr_p7_start_thread(void *ptr) {
  printf("\n[NFAPI VNF]  ->  %s()", __FUNCTION__);
  pthread_setname_np(pthread_self(), "VNF_P7");
  nfapi_vnf_p7_config_t *p7_config = (nfapi_vnf_p7_config_t *)ptr;
  nfapi_nr_vnf_p7_start(p7_config);
  return p7_config;
}

void set_thread_priority(int priority)
{
    set_priority(priority);

    pthread_attr_t ptAttr;
    if (pthread_attr_setschedpolicy(&ptAttr, SCHED_RR) != 0)
    {
        printf("\n[NFAPI VNF]  ->  Failed to set pthread sched policy SCHED_RR");
    }

    pthread_attr_setinheritsched(&ptAttr, PTHREAD_EXPLICIT_SCHED);
    struct sched_param thread_params;
    thread_params.sched_priority = 20;

    if (pthread_attr_setschedparam(&ptAttr, &thread_params) != 0)
    {
        printf("\n[NFAPI VNF]  ->  failed to set sched param");
    }
}

void add_slot(uint16_t *frameP, uint16_t *slotP, int offset)
{
	uint16_t num_slots = 20; // set based on numerlogy (fixing for 1)

    *frameP    = (*frameP + ((*slotP + offset) / num_slots))%1024; 

    *slotP = ((*slotP + offset) % num_slots);
}

uint32_t sfnslot_add_slot(uint16_t sfn, uint16_t slot, int offset)
{
  uint32_t new_sfnslot;
//   uint16_t sfn = NFAPI_SFNSLOT2SFN(sfnslot);
//   uint16_t slot  = NFAPI_SFNSLOT2SLOT(sfnslot);

  //printf("%s() sfn:%u sf:%u\n", __FUNCTION__, sfn, sf);
  add_slot(&sfn, &slot, offset);

  new_sfnslot = sfn<<6|slot;

  //printf("%s() sfn:%u sf:%u offset:%d sfnsf:%d(DEC:%d) new:%d(DEC:%d)\n", __FUNCTION__, sfn, sf, offset, sfnsf, NFAPI_SFNSF2DEC(sfnsf), new_sfnsf, NFAPI_SFNSF2DEC(new_sfnsf));

  return new_sfnslot;
}

//NR phy indication

int phy_nr_slot_indication(nfapi_nr_slot_indication_scf_t *ind) {

  uint8_t vnf_slot_ahead = 0;
  uint32_t vnf_sfn_slot = sfnslot_add_slot(ind->sfn, ind->slot, vnf_slot_ahead);
  uint16_t vnf_sfn = NFAPI_SFNSLOT2SFN(vnf_sfn_slot);
  uint8_t vnf_slot = NFAPI_SFNSLOT2SLOT(vnf_sfn_slot);
  printf("VNF SFN/Slot %d.%d \n", vnf_sfn, vnf_slot);

  nfapi_nr_slot_indication_scf_t *nr_slot_ind = calloc(1, sizeof(*nr_slot_ind));
  nr_slot_ind->header = ind->header;
  nr_slot_ind->sfn = vnf_sfn;
  nr_slot_ind->slot = vnf_slot;
  if (!put_queue(&gnb_slot_ind_queue, nr_slot_ind))
  {
    //LOG_E(NR_MAC, "Put_queue failed for slot_ind\n");
    free(nr_slot_ind);
    nr_slot_ind = NULL;
  }

  return 1;
}

int phy_nr_rx_data_indication(nfapi_nr_rx_data_indication_t *ind) {

  printf("In %s() NFAPI SFN/SF: %d/%d number_of_pdus :%u, and pdu %p\n",
          __FUNCTION__,ind->sfn, ind->slot, ind->number_of_pdus, ind->pdu_list[0].pdu);

  if(1)
  {
    nfapi_nr_rx_data_indication_t *rx_ind = calloc(1, sizeof(*rx_ind));
    rx_ind->header.message_id = ind->header.message_id;
    rx_ind->sfn = ind->sfn;
    rx_ind->slot = ind->slot;
    rx_ind->number_of_pdus = ind->number_of_pdus;

    if (ind->number_of_pdus > 0) {
      rx_ind->pdu_list = calloc(NFAPI_NR_RX_DATA_IND_MAX_PDU, sizeof(nfapi_nr_rx_data_pdu_t));
      AssertFatal(rx_ind->pdu_list != NULL, "Memory not allocated for rx_ind->pdu_list in phy_nr_rx_data_indication.");
    }
    for (int j = 0; j < ind->number_of_pdus; j++)
    {
      rx_ind->pdu_list[j].handle = ind->pdu_list[j].handle;
      rx_ind->pdu_list[j].harq_id = ind->pdu_list[j].harq_id;
      rx_ind->pdu_list[j].pdu = ind->pdu_list[j].pdu;
      rx_ind->pdu_list[j].pdu_length = ind->pdu_list[j].pdu_length;
      rx_ind->pdu_list[j].rnti = ind->pdu_list[j].rnti;
      rx_ind->pdu_list[j].timing_advance = ind->pdu_list[j].timing_advance;
      rx_ind->pdu_list[j].ul_cqi = ind->pdu_list[j].ul_cqi;
    }
    if (!put_queue(&gnb_rx_ind_queue, rx_ind))
    {
      printf("[MWNL] Put_queue failed for rx_ind\n");
      free(rx_ind->pdu_list);
      free(rx_ind);
    }
  }
  else
  {
    // LOG_E(NR_MAC, "NFAPI_MODE = %d not NFAPI_MODE_VNF(2)\n", nfapi_getmode());
  }
  return 1;
}

int phy_nr_crc_indication(nfapi_nr_crc_indication_t *ind) {

  printf("In %s() NFAPI SFN/SF: %d/%d number_of_pdus :%u\n",
          __FUNCTION__,ind->sfn, ind->slot, ind->number_crcs);

  if(1)
  {
    nfapi_nr_crc_indication_t *crc_ind = calloc(1, sizeof(*crc_ind));
    crc_ind->header.message_id = ind->header.message_id;
    crc_ind->number_crcs = ind->number_crcs;
    crc_ind->sfn = ind->sfn;
    crc_ind->slot = ind->slot;
    if (ind->number_crcs > 0) {
      crc_ind->crc_list = calloc(NFAPI_NR_CRC_IND_MAX_PDU, sizeof(nfapi_nr_crc_t));
      AssertFatal(crc_ind->crc_list != NULL, "Memory not allocated for crc_ind->crc_list in phy_nr_crc_indication.");
    }
    for (int j = 0; j < ind->number_crcs; j++)
    {
      crc_ind->crc_list[j].handle = ind->crc_list[j].handle;
      crc_ind->crc_list[j].harq_id = ind->crc_list[j].harq_id;
      crc_ind->crc_list[j].num_cb = ind->crc_list[j].num_cb;
      crc_ind->crc_list[j].rnti = ind->crc_list[j].rnti;
      crc_ind->crc_list[j].tb_crc_status = ind->crc_list[j].tb_crc_status;
      crc_ind->crc_list[j].timing_advance = ind->crc_list[j].timing_advance;
      crc_ind->crc_list[j].ul_cqi = ind->crc_list[j].ul_cqi;
      printf("Received crc_ind.harq_id = %d for %d index SFN SLot %u %u with rnti %x\n",
                    ind->crc_list[j].harq_id, j, ind->sfn, ind->slot, ind->crc_list[j].rnti);
    }
    if (!put_queue(&gnb_crc_ind_queue, crc_ind))
    {
      printf("Put_queue failed for crc_ind\n");
      free(crc_ind->crc_list);
      free(crc_ind);
    }
  }
  else
  {
    // LOG_E(NR_MAC, "NFAPI_MODE = %d not NFAPI_MODE_VNF(2)\n", nfapi_getmode());
  }
  return 1;
}

int phy_nr_uci_indication(nfapi_nr_uci_indication_t *ind)
{

  printf("[MWNL]In %s() NFAPI SFN/SF: %d/%d number_of_pdus :%u\n",
          __FUNCTION__,ind->sfn, ind->slot, ind->num_ucis);
  if(1)
  {
    nfapi_nr_uci_indication_t *uci_ind = calloc(1, sizeof(*uci_ind));
    AssertFatal(uci_ind != NULL, "Memory not allocated for uci_ind in phy_nr_uci_indication.");
    *uci_ind = *ind;

    uci_ind->uci_list = calloc(NFAPI_NR_UCI_IND_MAX_PDU, sizeof(nfapi_nr_uci_t));
    AssertFatal(uci_ind->uci_list != NULL, "Memory not allocated for uci_ind->uci_list in phy_nr_uci_indication.");
    for (int i = 0; i < ind->num_ucis; i++)
    {
      uci_ind->uci_list[i] = ind->uci_list[i];

      switch (uci_ind->uci_list[i].pdu_type) {
        case NFAPI_NR_UCI_PUSCH_PDU_TYPE:
          printf("%s(): unhandled NFAPI_NR_UCI_PUSCH_PDU_TYPE\n", __func__);
          break;

        case NFAPI_NR_UCI_FORMAT_0_1_PDU_TYPE: {
          nfapi_nr_uci_pucch_pdu_format_0_1_t *uci_ind_pdu = &uci_ind->uci_list[i].pucch_pdu_format_0_1;
          nfapi_nr_uci_pucch_pdu_format_0_1_t *ind_pdu = &ind->uci_list[i].pucch_pdu_format_0_1;
          if (ind_pdu->sr) {
            uci_ind_pdu->sr = calloc(1, sizeof(*uci_ind_pdu->sr));
            AssertFatal(uci_ind_pdu->sr != NULL, "Memory not allocated for uci_ind_pdu->harq in phy_nr_uci_indication.");
            *uci_ind_pdu->sr = *ind_pdu->sr;
          }
          if (ind_pdu->harq) {
            uci_ind_pdu->harq = calloc(1, sizeof(*uci_ind_pdu->harq));
            AssertFatal(uci_ind_pdu->harq != NULL, "Memory not allocated for uci_ind_pdu->harq in phy_nr_uci_indication.");
            *uci_ind_pdu->harq = *ind_pdu->harq;

            uci_ind_pdu->harq->harq_list = calloc(uci_ind_pdu->harq->num_harq, sizeof(*uci_ind_pdu->harq->harq_list));
            AssertFatal(uci_ind_pdu->harq->harq_list != NULL, "Memory not allocated for uci_ind_pdu->harq->harq_list in phy_nr_uci_indication.");
            for (int j = 0; j < uci_ind_pdu->harq->num_harq; j++)
                uci_ind_pdu->harq->harq_list[j].harq_value = ind_pdu->harq->harq_list[j].harq_value;
          }
          break;
        }

        case NFAPI_NR_UCI_FORMAT_2_3_4_PDU_TYPE: {
          nfapi_nr_uci_pucch_pdu_format_2_3_4_t *uci_ind_pdu = &uci_ind->uci_list[i].pucch_pdu_format_2_3_4;
          nfapi_nr_uci_pucch_pdu_format_2_3_4_t *ind_pdu = &ind->uci_list[i].pucch_pdu_format_2_3_4;
          *uci_ind_pdu = *ind_pdu;
          if (ind_pdu->harq.harq_payload) {
            uci_ind_pdu->harq.harq_payload = calloc(1, sizeof(*uci_ind_pdu->harq.harq_payload));
            AssertFatal(uci_ind_pdu->harq.harq_payload != NULL, "Memory not allocated for uci_ind_pdu->harq.harq_payload in phy_nr_uci_indication.");
            *uci_ind_pdu->harq.harq_payload = *ind_pdu->harq.harq_payload;
          }
          if (ind_pdu->sr.sr_payload) {
            uci_ind_pdu->sr.sr_payload = calloc(1, sizeof(*uci_ind_pdu->sr.sr_payload));
            AssertFatal(uci_ind_pdu->sr.sr_payload != NULL, "Memory not allocated for uci_ind_pdu->sr.sr_payload in phy_nr_uci_indication.");
            *uci_ind_pdu->sr.sr_payload = *ind_pdu->sr.sr_payload;
          }
          if (ind_pdu->csi_part1.csi_part1_payload) {
            uci_ind_pdu->csi_part1.csi_part1_payload = calloc(1, sizeof(*uci_ind_pdu->csi_part1.csi_part1_payload));
            AssertFatal(uci_ind_pdu->csi_part1.csi_part1_payload != NULL, "Memory not allocated for uci_ind_pdu->csi_part1.csi_part1_payload in phy_nr_uci_indication.");
            *uci_ind_pdu->csi_part1.csi_part1_payload = *ind_pdu->csi_part1.csi_part1_payload;
          }
          if (ind_pdu->csi_part2.csi_part2_payload) {
            uci_ind_pdu->csi_part2.csi_part2_payload = calloc(1, sizeof(*uci_ind_pdu->csi_part2.csi_part2_payload));
            AssertFatal(uci_ind_pdu->csi_part2.csi_part2_payload != NULL, "Memory not allocated for uci_ind_pdu->csi_part2.csi_part2_payload in phy_nr_uci_indication.");
            *uci_ind_pdu->csi_part2.csi_part2_payload = *ind_pdu->csi_part2.csi_part2_payload;
          }
          break;
        }
      }
    }

    if (!put_queue(&gnb_uci_ind_queue, uci_ind))
    {
      printf("[MWNL] Put_queue failed for uci_ind\n");
      for (int i = 0; i < ind->num_ucis; i++)
      {
          if (uci_ind->uci_list[i].pdu_type == NFAPI_NR_UCI_FORMAT_0_1_PDU_TYPE)
          {
            if (uci_ind->uci_list[i].pucch_pdu_format_0_1.harq) {
              free(uci_ind->uci_list[i].pucch_pdu_format_0_1.harq->harq_list);
              uci_ind->uci_list[i].pucch_pdu_format_0_1.harq->harq_list = NULL;
              free(uci_ind->uci_list[i].pucch_pdu_format_0_1.harq);
              uci_ind->uci_list[i].pucch_pdu_format_0_1.harq = NULL;
            }
            if (uci_ind->uci_list[i].pucch_pdu_format_0_1.sr) {
              free(uci_ind->uci_list[i].pucch_pdu_format_0_1.sr);
              uci_ind->uci_list[i].pucch_pdu_format_0_1.sr = NULL;
            }
          }
          if (uci_ind->uci_list[i].pdu_type == NFAPI_NR_UCI_FORMAT_2_3_4_PDU_TYPE)
          {
            free(uci_ind->uci_list[i].pucch_pdu_format_2_3_4.harq.harq_payload);
            free(uci_ind->uci_list[i].pucch_pdu_format_2_3_4.csi_part1.csi_part1_payload);
            free(uci_ind->uci_list[i].pucch_pdu_format_2_3_4.csi_part2.csi_part2_payload);
          }
      }
      free(uci_ind->uci_list);
      uci_ind->uci_list = NULL;
      free(uci_ind);
      uci_ind = NULL;
    }
  }
  else {
    // LOG_E(NR_MAC, "NFAPI_MODE = %d not NFAPI_MODE_VNF(2)\n", nfapi_getmode());
  }
  return 1;
}

/**** TODO SRS *****/
// int phy_nr_srs_indication(nfapi_nr_srs_indication_t *ind) {
//   struct PHY_VARS_gNB_s *gNB = RC.gNB[0];
//   pthread_mutex_lock(&gNB->UL_INFO_mutex);

//   gNB->UL_INFO.srs_ind = *ind;

//   if (ind->number_of_pdus > 0)
//     gNB->UL_INFO.srs_ind.pdu_list = malloc(sizeof(nfapi_nr_srs_indication_pdu_t)*ind->number_of_pdus);

//   for (int i=0; i<ind->number_of_pdus; i++) {
//     memcpy(&gNB->UL_INFO.srs_ind.pdu_list[i], &ind->pdu_list[i], sizeof(ind->pdu_list[0]));

//     printf("%s() NFAPI SFN/Slot:%d.%d SRS_IND:number_of_pdus:%d UL_INFO:pdus:%d\n",
//         __FUNCTION__,
//         ind->sfn,ind->slot, ind->number_of_pdus, gNB->UL_INFO.srs_ind.number_of_pdus
//         );
//   }

//   pthread_mutex_unlock(&gNB->UL_INFO_mutex);

//   return 1;
// }

int phy_nr_rach_indication(nfapi_nr_rach_indication_t *ind)
{
  if(1)
  {
    nfapi_nr_rach_indication_t *rach_ind = calloc(1, sizeof(*rach_ind));
    rach_ind->header.message_id = ind->header.message_id;
    rach_ind->number_of_pdus = ind->number_of_pdus;
    rach_ind->sfn = ind->sfn;
    rach_ind->slot = ind->slot;
    rach_ind->pdu_list = calloc(rach_ind->number_of_pdus, sizeof(*rach_ind->pdu_list));
    AssertFatal(rach_ind->pdu_list != NULL, "Memory not allocated for rach_ind->pdu_list in phy_nr_rach_indication.");
    for (int i = 0; i < ind->number_of_pdus; i++)
    {
      rach_ind->pdu_list[i].num_preamble = ind->pdu_list[i].num_preamble;
      rach_ind->pdu_list[i].freq_index = ind->pdu_list[i].freq_index;
      rach_ind->pdu_list[i].symbol_index = ind->pdu_list[i].symbol_index;
      rach_ind->pdu_list[i].preamble_list = calloc(ind->pdu_list[i].num_preamble, sizeof(nfapi_nr_prach_indication_preamble_t));
      AssertFatal(rach_ind->pdu_list[i].preamble_list != NULL, "Memory not allocated for rach_ind->pdu_list[i].preamble_list  in phy_nr_rach_indication.");
      for (int j = 0; j < ind->number_of_pdus; j++)
      {
        rach_ind->pdu_list[i].preamble_list[j].preamble_index = ind->pdu_list[i].preamble_list[j].preamble_index;
        rach_ind->pdu_list[i].preamble_list[j].timing_advance = ind->pdu_list[i].preamble_list[j].timing_advance;
      }
    }
    if (!put_queue(&gnb_rach_ind_queue, rach_ind))
    {
      printf("[MWNL] Put_queue failed for rach_ind\n");
      for (int i = 0; i < ind->number_of_pdus; i++)
      {
        free(rach_ind->pdu_list[i].preamble_list);
      }
      free(rach_ind->pdu_list);
      free(rach_ind);
    }
  }
  else {
    //LOG_E(NR_MAC, "NFAPI_MODE = %d not NFAPI_MODE_VNF(2)\n", nfapi_getmode());
  }
  return 1;
}

void *vnf_nr_p7_thread_start(void *ptr) {
  set_thread_priority(79);
  init_queue(&gnb_rach_ind_queue);
  init_queue(&gnb_rx_ind_queue);
  init_queue(&gnb_crc_ind_queue);
  init_queue(&gnb_uci_ind_queue);
  init_queue(&gnb_slot_ind_queue);

  vnf_p7_info *p7_vnf = (vnf_p7_info *)ptr;
  p7_vnf->config->port = p7_vnf->local_port;

  //p7_vnf->config->sync_indication = &phy_sync_indication;
  //p7_vnf->config->slot_indication = &phy_slot_indication;
  //p7_vnf->config->harq_indication = &phy_harq_indication;
  p7_vnf->config->nr_crc_indication = &phy_nr_crc_indication;
  p7_vnf->config->nr_rx_data_indication = &phy_nr_rx_data_indication;
  p7_vnf->config->nr_rach_indication = &phy_nr_rach_indication;
  p7_vnf->config->nr_uci_indication = &phy_nr_uci_indication;
  //p7_vnf->config->srs_indication = &phy_srs_indication;
  //p7_vnf->config->sr_indication = &phy_sr_indication;
  //p7_vnf->config->cqi_indication = &phy_cqi_indication;
  //p7_vnf->config->lbt_dl_indication = &phy_lbt_dl_indication;
  //p7_vnf->config->nb_harq_indication = &phy_nb_harq_indication;
  //p7_vnf->config->nrach_indication = &phy_nrach_indication;
  p7_vnf->config->nr_slot_indication = &phy_nr_slot_indication;
  // p7_vnf->config->nr_srs_indication = &phy_nr_srs_indication; // TODO SRS ind

  p7_vnf->config->malloc = &vnf_allocate;
  p7_vnf->config->free = &vnf_deallocate;
  //p7_vnf->config->vendor_ext = &phy_vendor_ext;
  p7_vnf->config->user_data = p7_vnf;
  p7_vnf->mac->user_data = p7_vnf;
  /*
  p7_vnf->config->codec_config.unpack_p7_vendor_extension = &phy_unpack_p7_vendor_extension;
  p7_vnf->config->codec_config.pack_p7_vendor_extension = &phy_pack_p7_vendor_extension;
  p7_vnf->config->codec_config.unpack_vendor_extension_tlv = &phy_unpack_vendor_extension_tlv;
  p7_vnf->config->codec_config.pack_vendor_extension_tlv = &phy_pack_vendor_extension_tlv;
  */
  p7_vnf->config->codec_config.allocate = &vnf_allocate;
  p7_vnf->config->codec_config.deallocate = &vnf_deallocate;
  /*
  p7_vnf->config->allocate_p7_vendor_ext = &phy_allocate_p7_vendor_ext;
  p7_vnf->config->deallocate_p7_vendor_ext = &phy_deallocate_p7_vendor_ext;
  */
  // printf("\n[NFAPI VNF]  ->  Creating VNF NFAPI P7 start thread %s", __FUNCTION__);
  pthread_create(&vnf_p7_start_pthread, NULL, &vnf_nr_p7_start_thread, p7_vnf->config);
  return 0;
}

int pnf_nr_start_resp_cb(nfapi_vnf_config_t *config, int p5_idx, nfapi_nr_pnf_start_response_t *resp) {
  vnf_info *vnf = (vnf_info *)(config->user_data);
  vnf_p7_info *p7_vnf = vnf->p7_vnfs;
  pnf_info *pnf = vnf->pnfs;
  nfapi_nr_param_request_scf_t req;
  printf("\n[NFAPI VNF]  ->  pnf start response idx:%d config:%p user_data:%p p7_vnf[config:%p thread_started:%d]", p5_idx, config, config->user_data, vnf->p7_vnfs[0].config, vnf->p7_vnfs[0].thread_started);

  if(p7_vnf->thread_started == 0) {
     pthread_t vnf_p7_thread;
     pthread_create(&vnf_p7_thread, NULL, &vnf_nr_p7_thread_start, p7_vnf);
     p7_vnf->thread_started = 1;
     printf("\n[NFAPI VNF]  ->  P7 thread is running");
  } else {
    printf("\n[NFAPI VNF]  ->  P7 thread has been running");
  }

  printf("\n[NFAPI VNF]  ->  Sending NFAPI_VNF_PARAM_REQUEST phy_id:%d", pnf->phys[0].id);
  memset(&req, 0, sizeof(req));
  req.header.message_id = NFAPI_NR_PHY_MSG_TYPE_PARAM_REQUEST;
  req.header.phy_id = pnf->phys[0].id;
  nfapi_nr_vnf_param_req(config, p5_idx, &req);
  return 0;
}

int nr_param_resp_cb(nfapi_vnf_config_t *config, int p5_idx, nfapi_nr_param_response_scf_t *resp){
  printf("\n[NFAPI VNF]  ->  Received NFAPI_PARAM_RESP idx:%d phy_id:%d", p5_idx, resp->header.phy_id);
  vnf_info *vnf = (vnf_info *)(config->user_data);
  vnf_p7_info *p7_vnf = vnf->p7_vnfs;
  pnf_info *pnf = vnf->pnfs;
  phy_info *phy = pnf->phys;
  struct sockaddr_in pnf_p7_sockaddr;
  //phy->remote_port = 50610;//resp->nfapi_config.p7_pnf_port.value;
  phy->remote_port = 32123;//resp->nfapi_config.p7_pnf_port.value;
  memcpy(&pnf_p7_sockaddr.sin_addr.s_addr, &(resp->nfapi_config.p7_pnf_address_ipv4.address[0]), 4);
  phy->remote_addr = inet_ntoa(pnf_p7_sockaddr.sin_addr);
  // for now just 1
  printf("\n[NFAPI VNF]  ->  p5_idx : %d phy_idx : %d pnf p7 %s:%d timing %u %u %u %u", p5_idx, phy->id, phy->remote_addr, \
        phy->remote_port, p7_vnf->timing_window, p7_vnf->periodic_timing_period, \
        p7_vnf->aperiodic_timing_enabled, p7_vnf->periodic_timing_period);


  /* The config request */
  // nfapi_nr_config_request_scf_t *req = &RC.nrmac[0]->config[0]; // check
  nfapi_nr_config_request_scf_t *req = (nfapi_nr_config_request_scf_t*) calloc(1, sizeof(nfapi_nr_config_request_scf_t));
  req->header.message_id = NFAPI_NR_PHY_MSG_TYPE_CONFIG_REQUEST;
  req->header.phy_id = phy->id;

  printf("\n[NFAPI VNF]  ->  Send NFAPI_CONFIG_REQUEST");
  req->nfapi_config.p7_vnf_port.tl.tag = NFAPI_NR_NFAPI_P7_VNF_PORT_TAG;
  req->nfapi_config.p7_vnf_port.value = p7_vnf->local_port;
  req->num_tlv++;
  printf("\n[NFAPI VNF]  ->  DJP local_port:%d", p7_vnf->local_port);
  req->nfapi_config.p7_vnf_address_ipv4.tl.tag = NFAPI_NR_NFAPI_P7_VNF_ADDRESS_IPV4_TAG;
  struct sockaddr_in vnf_p7_sockaddr;
  vnf_p7_sockaddr.sin_addr.s_addr = inet_addr(p7_vnf->local_addr);
  memcpy(&(req->nfapi_config.p7_vnf_address_ipv4.address[0]), &vnf_p7_sockaddr.sin_addr.s_addr, 4);
  req->num_tlv++;
  printf("\n[NFAPI VNF]  ->  DJP local_addr:%s", p7_vnf->local_addr);
  req->nfapi_config.timing_window.tl.tag = NFAPI_NR_NFAPI_TIMING_WINDOW_TAG;
  req->nfapi_config.timing_window.value = p7_vnf->timing_window;
  printf("\nNFAPI\t->\t[VNF]Timing window tag : %d Timing window:%u",NFAPI_NR_NFAPI_TIMING_WINDOW_TAG, p7_vnf->timing_window);
  req->num_tlv++;

  if(p7_vnf->periodic_timing_enabled || p7_vnf->aperiodic_timing_enabled) {
    req->nfapi_config.timing_info_mode.tl.tag = NFAPI_NR_NFAPI_TIMING_INFO_MODE_TAG;
    req->nfapi_config.timing_info_mode.value = (p7_vnf->aperiodic_timing_enabled << 1) | (p7_vnf->periodic_timing_enabled);
    req->num_tlv++;

    if(p7_vnf->periodic_timing_enabled) {
      req->nfapi_config.timing_info_period.tl.tag = NFAPI_NR_NFAPI_TIMING_INFO_PERIOD_TAG;
      req->nfapi_config.timing_info_period.value = p7_vnf->periodic_timing_period;
      req->num_tlv++;
    }
  }
  //TODO: Assign tag and value for P7 message offsets
  req->nfapi_config.dl_tti_timing_offset.tl.tag = NFAPI_NR_NFAPI_DL_TTI_TIMING_OFFSET;
  req->nfapi_config.ul_tti_timing_offset.tl.tag = NFAPI_NR_NFAPI_UL_TTI_TIMING_OFFSET;
  req->nfapi_config.ul_dci_timing_offset.tl.tag = NFAPI_NR_NFAPI_UL_DCI_TIMING_OFFSET;
  req->nfapi_config.tx_data_timing_offset.tl.tag = NFAPI_NR_NFAPI_TX_DATA_TIMING_OFFSET;

  // vendor_ext_tlv_2 ve2;
  // memset(&ve2, 0, sizeof(ve2));
  // ve2.tl.tag = VENDOR_EXT_TLV_2_TAG;
  // ve2.dummy = 2016;
  // req->vendor_extension = &ve2.tl;
  nfapi_nr_vnf_config_req(config, p5_idx, req);
  printf("\n[NFAPI VNF]  ->  Sent NFAPI_VNF_CONFIG_REQ");
  return 0;
}


int nr_config_resp_cb(nfapi_vnf_config_t *config, int p5_idx, nfapi_nr_config_response_scf_t *resp) {
  nfapi_nr_start_request_scf_t req;
  printf("\n[NFAPI VNF]  ->  Received NFAPI_CONFIG_RESP idx:%d phy_id:%d", p5_idx, resp->header.phy_id);
  printf("\n[NFAPI VNF]  ->  Calling oai_enb_init()");
  // oai_enb_init(); // TODO: change to gnb
  memset(&req, 0, sizeof(req));
  req.header.message_id = NFAPI_NR_PHY_MSG_TYPE_START_REQUEST;
  req.header.phy_id = resp->header.phy_id;
  nfapi_nr_vnf_start_req(config, p5_idx, &req);
  return 0;
}


int nr_start_resp_cb(nfapi_vnf_config_t *config, int p5_idx, nfapi_nr_start_response_scf_t *resp) {
  printf("\n[NFAPI VNF]  ->  Received NFAPI_START_RESP idx:%d phy_id:%d", p5_idx, resp->header.phy_id);
  vnf_info *vnf = (vnf_info *)(config->user_data);
  pnf_info *pnf = vnf->pnfs;
  phy_info *phy = pnf->phys;
  vnf_p7_info *p7_vnf = vnf->p7_vnfs;

  nfapi_vnf_p7_add_pnf((p7_vnf->config), phy->remote_addr, phy->remote_port, phy->id);
  return 0;
}
