/*
 * ServiceDispatcherRouter.c
 *
 *  Created on: Nov 4, 2016
 *      Author: nxa03718
 */
#include <string.h>

#include "ServiceDispatcher.h"
#include "ServiceDispatcherRouter.h"
/******************************************************************************
 * PRIVATE DATATYPES
 ******************************************************************************/
typedef struct
{
    RemoteFunctionItem_t *actions;
    eTOPIC_t topic;
    char    *topicName;
    uint8_t nofActions;
}actionItem_t;

typedef struct
{
    actionItem_t services[SD_NOF_TOPICS];
    int8_t nofServices;
}ServicesList_t;

/******************************************************************************
 * LOCAL VARIABLE
 ******************************************************************************/
static ServicesList_t srvs;

/******************************************************************************
 * PRIVATE FUNCTIONS
 ******************************************************************************/

/******************************************************************************
 * Search in srvs if there is a topicId match
 * return value: >= 0 : return value is a valid serviceId
 *                < 0 : no result found
 */
static int8_t sdr_SearchServiceByTopicId(eTOPIC_t topic)
{
    int8_t idx = srvs.nofServices-1;
    while(idx >= 0)
    {
        if (srvs.services[idx].topic == topic)
        {
            break;
        }
        idx--;
    }
    return idx;
}

/******************************************************************************
 * Search in srvs if there is a topicName match
 * return value: >= 0 : return value is a valid serviceId
 *                < 0 : no result found
 */
static int8_t sdr_SearchServiceByTopicName(char *topicName)
{
    int8_t idx = srvs.nofServices -1;
    while(idx >=0)
    {
        if(0 == strcmp(srvs.services[idx].topicName, topicName))
        {
            break;
        }
        idx--;
    }
    return idx;
}

/******************************************************************************
 * Parse topic(string) into topic part and action part
 * param topic:  [in/out]   expect that incomming string is like "xxxxx/y"
 *                          multiple x, single y!
 *                          return value is only the x part
 * param action: [out]      return the y part of topic
 * return value: >= 0 : return value is a valid serviceId
 *                < 0 : no result found
 */
int8_t sdr_ParseTopic(char *topic, uint8_t *action)
{
    char *p = strchr(topic, (int)'/');
    if(NULL != p)
    {
        *p = 0;
        *action = *(p + 1);
        return 1;
    }
    return 0;
}

/******************************************************************************
 * PUBLIC FUNCTIONS
 *****************************************************************************/

/******************************************************************************
 * SDR_Init, nothing to init at the moment...
 */
void   SDR_Init()
{
}

/******************************************************************************
 * SDR_SubscribeService
 * Add new services to our service list
 * param pActionList [in]   pointer to array with actions
 * param nofActions  [in]   number of actions in the actionlist
 * param topic       [in]   topic id
 * param topicName   [in]   topic name in human-readable-text
 * return:                  nothing
 */
void SDR_SubscribeService(RemoteFunctionItem_t *pActionList, uint8_t nofActions, eTOPIC_t topic, char *topicName)
{
    if(srvs.nofServices < SD_NOF_TOPICS)
    {
//        LOG(myName, eLOG_Info, "Add List: %s", *topic);
        srvs.services[srvs.nofServices].actions     = pActionList;
        srvs.services[srvs.nofServices].topic       = topic;
        srvs.services[srvs.nofServices].topicName   = topicName;
        srvs.services[srvs.nofServices].nofActions  = nofActions;
        srvs.nofServices++;
//        LOG_INFO("Subscribe %s", topicName);
    }
    else
    {
//        LOG(myName, eLOG_Error, "Message list full");
    }
}

/******************************************************************************
 * SDR_Rout
 * Search the service list for a service with the same id/name and a matching action
 * param handle      [in]   NOT USED?!?!?
 * param topic       [in]   string pointer, contains id and action: "xxxx/y"
 *                              multiple x possible, only one y
 *                              when one x, x is a topicId, else x is topicName
 * param topicLength [in]   length of topic
 * param data        [in]   can contain data for the function to call
 * param dataLength  [in]   size of data
 * return: >= 0 when succeed
 *         <  0 when not succeed
 */
int8_t SDR_Rout(SDHandle_t handle, uint8_t *topic, uint8_t topicLength, uint8_t *data, uint8_t dataLength)
{
    int8_t idxService = -1;
    RemoteFunctionItem_t *pActionList;

    uint8_t action;
    if(topicLength==3)
    {
        action = topic[2];
        idxService = sdr_SearchServiceByTopicId((eTOPIC_t)topic[0]);
    }
    else if(topicLength >3)
    {
        if(1 == sdr_ParseTopic((char*)topic, &action))
        {
            idxService = sdr_SearchServiceByTopicName((char*)topic);
        }
    }

    if(0 > idxService)
    {
        return -1;
    }

    if(srvs.services[idxService].nofActions <= action)
    {
        return -2;
    }

    //Matching list found, now search for command!
    pActionList = srvs.services[idxService].actions;
    pActionList += action;
    if( (pActionList->remoteFunctionId == action) && //check if id matches requested action, just to be sure!
        (pActionList->remoteFunction != NULL) )
    {
//            LOG(myName, eLOG_Info, "Execute FP:%d, %d", idxList,idxMsg);
        (*pActionList->remoteFunction)(data);
    }

    return 0;
}
