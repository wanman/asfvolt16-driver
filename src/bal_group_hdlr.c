/*
** Copyright 2017-present Open Networking Foundation
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
** http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

#include <stdio.h>
#include <bal_api.h>

#undef _SYS_QUEUE_H_

/* Includes related to proto buf */
#include "asfvolt16_driver.h"
#include "bal_group_hdlr.h"


/********************************************************************\
 * Function    : bal_group_cfg_req                                  *
 * Description : Configures the OLT device                          *
 ********************************************************************/

uint32_t bal_group_cfg_set(BalGroupCfg *tm_group_cfg)
{
    bcmos_errno err = BCM_ERR_OK;
    bcmbal_group_cfg grp_cfg_obj;   /**< declare main API struct */
    bcmbal_group_key key = { };      /**< declare key */
    int grp_mem_idx;

    if(tm_group_cfg->key->has_group_id)
    {
       key.group_id = tm_group_cfg->key->group_id;
    }
    else
    {
       ASFVOLT_LOG(ASFVOLT_ERROR, "Failed to configure the group cfg(OLT): Group Id was not present \n");
       return BAL_ERRNO__BAL_ERR_NOENT;
    }

    ASFVOLT_LOG(ASFVOLT_DEBUG, "Configuration of OLT(group cfg) starts\n");

    /* init the API struct */
    BCMBAL_CFG_INIT(&grp_cfg_obj, group, key);

    /* decode API parameters from GRPC */
    if(tm_group_cfg->data->has_members_cmd)
    {
        ASFVOLT_CFG_PROP_SET(grp_cfg_obj, group, members_cmd, 
                             tm_group_cfg->data->has_members_cmd, 
                             tm_group_cfg->data->members_cmd);
    }

    if(tm_group_cfg->data->has_cookie)
    {
        ASFVOLT_CFG_PROP_SET(grp_cfg_obj, group, cookie, 
                             tm_group_cfg->data->has_cookie, 
                             tm_group_cfg->data->cookie);
    }

    BalIdList *balFlows = (BalIdList *)tm_group_cfg->data->flows;
    bcmbal_flow_id_list_u32 valFlows = {};

    valFlows.len = balFlows->n_val;
    valFlows.val = (bcmbal_flow_id *)malloc((valFlows.len)*sizeof(bcmbal_flow_id));
    if(!valFlows.val)
    {
       ASFVOLT_LOG(ASFVOLT_ERROR, "Failed to configure the group cfg(OLT): Memory Exhausted\n");
       return BAL_ERRNO__BAL_ERR_NOMEM;
    }
    memcpy((void *)valFlows.val, (const void *)balFlows->val, 
           (balFlows->n_val)*sizeof(bcmbal_flow_id));
    ASFVOLT_CFG_PROP_SET(grp_cfg_obj, group, flows, BCMOS_TRUE, valFlows); 

    if(tm_group_cfg->data->has_owner)
    {
        ASFVOLT_CFG_PROP_SET(grp_cfg_obj, group, owner, 
                             tm_group_cfg->data->has_owner, 
                             tm_group_cfg->data->owner);
    }

    BalGroupMemberInfoList *balMembers = 
              (BalGroupMemberInfoList *)tm_group_cfg->data->members;
    bcmbal_group_member_info_list_u16 valMembers = {};
    /*if(balMembers->has_len)*/
    {
       valMembers.len = balMembers->n_val;
       valMembers.val = 
        (bcmbal_group_member_info *)malloc((valMembers.len)*sizeof(bcmbal_group_member_info));
       if(!valMembers.val)
       {
          ASFVOLT_LOG(ASFVOLT_ERROR, "Failed to configure the group cfg(OLT): Memory Exhausted\n");
          return BAL_ERRNO__BAL_ERR_NOMEM;
       }
       //check
       //valMembers.val = &balMembers->val;
       for(grp_mem_idx = 0; grp_mem_idx < valMembers.len; grp_mem_idx++)
       {
          if(balMembers->val[grp_mem_idx]->has_intf_id)
          {
             valMembers.val[grp_mem_idx].intf_id 
                                  = balMembers->val[grp_mem_idx]->intf_id;
          }
          if(balMembers->val[grp_mem_idx]->has_svc_port_id)
          {
             valMembers.val[grp_mem_idx].svc_port_id 
                               = balMembers->val[grp_mem_idx]->svc_port_id;
          }
          if(balMembers->val[grp_mem_idx]->action->has_presence_mask)
          {
             valMembers.val[grp_mem_idx].action.presence_mask 
                     = balMembers->val[grp_mem_idx]->action->presence_mask;
          }
          if(balMembers->val[grp_mem_idx]->action->has_cmds_bitmask)
          {
             valMembers.val[grp_mem_idx].action.cmds_bitmask
                     = balMembers->val[grp_mem_idx]->action->cmds_bitmask;
          }
          if(balMembers->val[grp_mem_idx]->action->has_o_vid)
          {
             valMembers.val[grp_mem_idx].action.o_vid
                     = balMembers->val[grp_mem_idx]->action->o_vid;
          }
          if(balMembers->val[grp_mem_idx]->action->has_o_pbits)
          {
             valMembers.val[grp_mem_idx].action.o_pbits
                     = balMembers->val[grp_mem_idx]->action->o_pbits;
          }
          if(balMembers->val[grp_mem_idx]->action->has_o_tpid)
          {
             valMembers.val[grp_mem_idx].action.o_tpid
                     = balMembers->val[grp_mem_idx]->action->o_tpid;
          }
          if(balMembers->val[grp_mem_idx]->action->has_i_vid)
          {
             valMembers.val[grp_mem_idx].action.i_vid
                     = balMembers->val[grp_mem_idx]->action->i_vid;
          }
          if(balMembers->val[grp_mem_idx]->action->has_i_pbits)
          {
             valMembers.val[grp_mem_idx].action.i_pbits
                     = balMembers->val[grp_mem_idx]->action->i_pbits;
          }
          if(balMembers->val[grp_mem_idx]->action->has_i_tpid)
          {
             valMembers.val[grp_mem_idx].action.i_tpid
                     = balMembers->val[grp_mem_idx]->action->i_tpid;
          }
          if(balMembers->val[grp_mem_idx]->queue->has_sched_id)
          {
             valMembers.val[grp_mem_idx].queue.sched_id
                     = balMembers->val[grp_mem_idx]->queue->sched_id;
          }
          if(balMembers->val[grp_mem_idx]->queue->has_queue_id)
          {
             valMembers.val[grp_mem_idx].queue.queue_id
                     = balMembers->val[grp_mem_idx]->queue->queue_id;
          }
       }
       ASFVOLT_CFG_PROP_SET(grp_cfg_obj, group, members, BCMOS_TRUE, valMembers); 
    }

    err = bcmbal_cfg_set(DEFAULT_ATERM_ID, &(grp_cfg_obj.hdr));

    if(BCM_ERR_OK != err)
    {
        ASFVOLT_LOG(ASFVOLT_ERROR, "Failed to configure the group cfg(OLT)\n");
        return err;
    }

    ASFVOLT_LOG(ASFVOLT_INFO, "\nSet Group configuration sent to OLT for Group Id(%d)\n", 
                               key.group_id);
    return err;
}


/********************************************************************\
 * Function    : bal_group_get_req                              *
 * Description : get the OLT device group cfg                    *
 ********************************************************************/

uint32_t bal_group_cfg_get(BalGroupKey *tm_group_cfg_key, 
                           BalGroupCfg *tm_group_cfg)
{
    bcmos_errno err = BCM_ERR_OK;
    bcmbal_group_cfg grp_cfg_obj;   /**< declare main API struct */
    bcmbal_group_key key = { };      /**< declare key */

    if(tm_group_cfg_key->has_group_id)
    {
       key.group_id = tm_group_cfg_key->group_id;
    }
    else
    {
       ASFVOLT_LOG(ASFVOLT_ERROR, "Failed to get the group cfg(OLT): Group Id was not present\n");
       return BAL_ERRNO__BAL_ERR_NOENT;
    }

    ASFVOLT_LOG(ASFVOLT_DEBUG, "Get group cfg(for OLT) starts\n");

    /* init the API struct */
    BCMBAL_CFG_INIT(&grp_cfg_obj, group, key);

    /* request all properties, include everything */
    BCMBAL_CFG_PROP_GET(&grp_cfg_obj, group, all_properties);

    err = bcmbal_cfg_get(DEFAULT_ATERM_ID, &grp_cfg_obj.hdr);

    if(BCM_ERR_OK != err)
    {
        ASFVOLT_LOG(ASFVOLT_ERROR, "Failed to get the group Cfg(OLT)\n");
        return err;
    }

    ASFVOLT_LOG(ASFVOLT_INFO, "\nGet Group cfg sent to OLT for Group Id(%d)\n", 
                               key.group_id);
    return err;
}

/********************************************************************\
 * Function    : bal_group_clear_req                            *
 * Description : clears the OLT device group cfg                    *
 ********************************************************************/

uint32_t bal_group_cfg_clear(BalGroupKey *tm_group_cfg_key)
{
    bcmos_errno err = BCM_ERR_OK;
    bcmbal_group_cfg grp_cfg_obj;   /**< declare main API struct */
    bcmbal_group_key key = { };      /**< declare key */

    if(tm_group_cfg_key->has_group_id)
    {
       key.group_id = tm_group_cfg_key->group_id;
    }
    else
    {
       ASFVOLT_LOG(ASFVOLT_ERROR, "Failed to clear the group cfg(OLT): Group Id was not present\n");
       return BAL_ERRNO__BAL_ERR_NOENT;
    }

    ASFVOLT_LOG(ASFVOLT_DEBUG, "Clearing of OLT(group cfg) starts\n");

    /* init the API struct */
    BCMBAL_CFG_INIT(&grp_cfg_obj, group, key);

    err = bcmbal_cfg_clear(DEFAULT_ATERM_ID, &grp_cfg_obj.hdr);

    if(BCM_ERR_OK != err)
    {
        ASFVOLT_LOG(ASFVOLT_ERROR, "Failed to clear the group Cfg(OLT)\n");
        return err;
    }

    ASFVOLT_LOG(ASFVOLT_INFO, "\nClear Group cfg clear sent to OLT for Group Id(%d) \n", 
                               key.group_id);
    return err;
}

/********************************************************************\
 * Function    : bal_group_cfg_indication_cb                        *
 * Description : Call back function registered with BAL to handle   *
 *               event related to access terminal                   *
 ********************************************************************/
bcmos_errno bal_group_cfg_indication_cb(bcmbal_obj *obj)
{
    bcmos_errno result = BCM_ERR_OK;
    ASFVOLT_LOG(ASFVOLT_INFO, "Processing API ('%s)\n' IND callback status is (%s)\n",
		    bcmbal_objtype_str(obj->obj_type),
		    bcmos_strerror(obj->status));
    
    return result;
}


