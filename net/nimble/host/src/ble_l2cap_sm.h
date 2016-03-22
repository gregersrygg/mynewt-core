/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#ifndef H_BLE_L2CAP_SM_
#define H_BLE_L2CAP_SM_

struct hci_le_lt_key_req;

#define BLE_L2CAP_SM_ERR_PASSKEY            0x01
#define BLE_L2CAP_SM_ERR_OOB                0x02
#define BLE_L2CAP_SM_ERR_AUTHREQ            0x03
#define BLE_L2CAP_SM_ERR_CONFIRM_MISMATCH   0x04
#define BLE_L2CAP_SM_ERR_PAIR_NOT_SUPP      0x05
#define BLE_L2CAP_SM_ERR_ENC_KEY_SZ         0x06
#define BLE_L2CAP_SM_ERR_CMD_NOT_SUPP       0x07
#define BLE_L2CAP_SM_ERR_UNSPECIFIED        0x08
#define BLE_L2CAP_SM_ERR_REPEATED           0x09
#define BLE_L2CAP_SM_ERR_INVAL              0x0a
#define BLE_L2CAP_SM_ERR_DHKEY              0x0b
#define BLE_L2CAP_SM_ERR_NUM_CMP            0x0c
#define BLE_L2CAP_SM_ERR_ALREADY            0x0d
#define BLE_L2CAP_SM_ERR_CROSS_TRANS        0x0e

#define BLE_L2CAP_SM_MTU            65

#define BLE_L2CAP_SM_HDR_SZ         1

/**
 * | Parameter                          | Size (octets)     |
 * +------------------------------------+-------------------+
 * | (Code=0x01/0x02 [req/rsp])         | 1                 |
 * | IO Capability                      | 1                 |
 * | OOB data flag                      | 1                 |
 * | AuthReq                            | 1                 |
 * | Maximum Encryption Key Size        | 1                 |
 * | Initiator Key Distribution         | 1                 |
 * | Responder Key Distribution         | 1                 |
 */
#define BLE_L2CAP_SM_PAIR_CMD_SZ        6
struct ble_l2cap_sm_pair_cmd {
    uint8_t io_cap;
    uint8_t oob_data_flag;
    uint8_t authreq;
    uint8_t max_enc_key_size;
    uint8_t init_key_dist;
    uint8_t resp_key_dist;
};

/**
 * | Parameter                          | Size (octets)     |
 * +------------------------------------+-------------------+
 * | (Code=0x03)                        | 1                 |
 * | Confirm Value                      | 16                |
 */
#define BLE_L2CAP_SM_PAIR_CONFIRM_SZ    16
struct ble_l2cap_sm_pair_confirm {
    uint8_t value[16];
};

/**
 * | Parameter                          | Size (octets)     |
 * +------------------------------------+-------------------+
 * | (Code=0x04)                        | 1                 |
 * | Random Value                       | 16                |
 */
#define BLE_L2CAP_SM_PAIR_RANDOM_SZ     16
struct ble_l2cap_sm_pair_random {
    uint8_t value[16];
};

/**
 * | Parameter                          | Size (octets)     |
 * +------------------------------------+-------------------+
 * | (Code=0x05)                        | 1                 |
 * | Reason                             | 1                 |
 */
#define BLE_L2CAP_SM_PAIR_FAIL_SZ       1
struct ble_l2cap_sm_pair_fail {
    uint8_t reason;
};

#ifdef BLE_HS_DEBUG
void ble_l2cap_sm_dbg_set_next_rand(uint8_t *next_rand);
int ble_l2cap_sm_dbg_num_procs(void);
#endif

int ble_l2cap_sm_locked_by_cur_task(void);

struct ble_l2cap_chan *ble_l2cap_sm_create_chan(void);

int ble_l2cap_sm_pair_cmd_parse(void *payload, int len,
                                struct ble_l2cap_sm_pair_cmd *cmd);
int ble_l2cap_sm_pair_cmd_write(void *payload, int len, int is_req,
                                struct ble_l2cap_sm_pair_cmd *cmd);
int ble_l2cap_sm_pair_cmd_tx(uint16_t conn_handle, int is_req,
                             struct ble_l2cap_sm_pair_cmd *cmd);
int ble_l2cap_sm_pair_confirm_parse(void *payload, int len,
                                    struct ble_l2cap_sm_pair_confirm *cmd);
int ble_l2cap_sm_pair_confirm_write(void *payload, int len,
                                    struct ble_l2cap_sm_pair_confirm *cmd);
int ble_l2cap_sm_pair_confirm_tx(uint16_t conn_handle,
                                 struct ble_l2cap_sm_pair_confirm *cmd);
int ble_l2cap_sm_pair_random_parse(void *payload, int len,
                                   struct ble_l2cap_sm_pair_random *cmd);
int ble_l2cap_sm_pair_random_write(void *payload, int len,
                                   struct ble_l2cap_sm_pair_random *cmd);
int ble_l2cap_sm_pair_random_tx(uint16_t conn_handle,
                                struct ble_l2cap_sm_pair_random *cmd);
int ble_l2cap_sm_pair_fail_parse(void *payload, int len,
                                 struct ble_l2cap_sm_pair_fail *cmd);
int ble_l2cap_sm_pair_fail_write(void *payload, int len,
                                 struct ble_l2cap_sm_pair_fail *cmd);
int ble_l2cap_sm_pair_fail_tx(uint16_t conn_handle,
                              struct ble_l2cap_sm_pair_fail *cmd);

int ble_l2cap_sm_alg_s1(uint8_t *k, uint8_t *r1, uint8_t *r2, uint8_t *out);
int ble_l2cap_sm_alg_c1(uint8_t *k, uint8_t *r,
                        uint8_t *preq, uint8_t *pres,
                        uint8_t iat, uint8_t rat,
                        uint8_t *ia, uint8_t *ra,
                        uint8_t *out_enc_data);

int ble_l2cap_sm_rx_lt_key_req(struct hci_le_lt_key_req *evt);

void ble_l2cap_sm_wakeup(void);
int ble_l2cap_sm_init(void);


#endif
