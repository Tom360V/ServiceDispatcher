/*
 *
 *  Created on: Nov 4, 2016
 *      Author: nxa03718
 */
#include <string.h>

#include "ServiceDispatcherQueue.h"
#include "ServiceDispatcherRouter.h"
#include "fifo.h"

/******************************************************************************
 * PRIVATE VARIABLE
 *****************************************************************************/
#define SDQ_ID_SEND         (0)
#define SDQ_ID_RECV         (1)
#define SDQ_FIFO_SIZE       (4)

#define MIN(a,b)            ((a<b)?a:b)

typedef struct sdqItem_t
{
    fpDelivered_t cbDeliverd;
    uint8_t topic[SDQ_Q_TOPIC];
    uint8_t topicLength;
    uint8_t data [SDQ_Q_DATA];
    uint8_t dataLength;
    uint8_t handle;
}sdqItem_t;

static sdqItem_t sdqFIFO_RECV[SDQ_FIFO_SIZE];
static sdqItem_t sdqFIFO_SEND[SDQ_FIFO_SIZE];

fifo_t fifo_send, fifo_recv;

static fpSDQ_SendTo_t   sdqfpToService;
static fpSDQ_SendTo_t   sdqfpToCommunication;

/******************************************************************************
 * PRIVATE FUNCTIONS
 *****************************************************************************/

/******************************************************************************
 * sdq_AddToQueue
 * Add new services to our service list
 * param pActionList [in]   pointer to array with actions
 * param nofActions  [in]   number of actions in the actionlist
 * param topic       [in]   topic id
 * param topicName   [in]   topic name in human-readable-text
 * return:                  nothing
 */
static SDHandle_t sdq_AddToQueue(fifo_t *pfifo, uint8_t *topic, uint8_t topicLength, uint8_t *data, uint8_t dataLength, fpDelivered_t cbDelivered)
{
    SDHandle_t sdqHandle = -1;
    sdqItem_t item;

    if(0 < FIFO_FreeSpace(pfifo))
    {
        item.cbDeliverd = *cbDelivered;

        item.topicLength = MIN(topicLength, SDQ_Q_TOPIC);
        //memset(item.topic, 0x0,   item.topicLength);
        memcpy(item.topic, topic, item.topicLength);

        item.dataLength  = MIN(dataLength,  SDQ_Q_DATA);
        //memset(item.data, 0x0,  item.dataLength);
        memcpy(item.data, data, item.dataLength);

        sdqHandle = FIFO_AddItem(pfifo, (void*)&item);
    }

    return sdqHandle;
}

void sdq_CheckForActions(fifo_t *pfifo, fpSDQ_SendTo_t fpSendTo)
{
    sdqItem_t item;
    if(NULL != fpSendTo)
    {
        if(0 <= FIFO_GetItem(pfifo, (void*)&item))
        {
            if (0 == fpSendTo(item.handle, item.topic, item.topicLength, item.data, item.dataLength))
            {
                if (NULL != item.cbDeliverd)
                {
                    (item.cbDeliverd)(item.handle, 1);
                }
            }
            else
            {
                //did not succeed to handle this item, so put it back in the fifo
                //FIFO_AddItem(pfifo, (void*)&item);
            }
        }
    }
}

static void sdq_Initialize(fpSDQ_SendTo_t fpToService, fpSDQ_SendTo_t fpToCommunication)
{
    int8_t itemSize = sizeof(sdqItem_t);

    FIFO_Init(&fifo_send, &sdqFIFO_SEND, itemSize, sizeof(sdqFIFO_SEND)/itemSize);
    FIFO_Init(&fifo_recv, &sdqFIFO_RECV, itemSize, sizeof(sdqFIFO_RECV)/itemSize);

    sdqfpToService       = fpToService;
    sdqfpToCommunication = fpToCommunication;
}


/******************************************************************************
 * PUBLIC FUNCTIONS
 *****************************************************************************/
void SDQ_Init(fpSDQ_SendTo_t fpToCommunication)
{
    sdq_Initialize(&SDR_Rout, fpToCommunication);
}

void SDQ_Init_ExternalRouter(fpSDQ_SendTo_t fpToService, fpSDQ_SendTo_t fpToCommunication)
{
    sdq_Initialize(fpToService, fpToCommunication);
}

void SDQ_Thread()
{
    sdq_CheckForActions(&fifo_send, sdqfpToCommunication);
    sdq_CheckForActions(&fifo_recv, sdqfpToService);
}

SDHandle_t SDQ_AddToRecvQueue(char *topic, uint8_t *data, uint8_t dataLength, fpDelivered_t cbDelivered)
{
    uint8_t topicLength = strlen(topic);
    if( (topicLength == 3) &&
        (topic[1] == '/') )
    {
        //TODO: Extend to support longer topics!
        //TODO: Now only 0 till 9 are valid services!
#ifdef TEST_MAKE_HUMAN_READABLE
        topic[0] = topic[0]-'0';
        topic[2] = topic[2]-'0';
#endif
    }
    else
    {
        char *p = strchr(topic, (int)'/');
        if(NULL != p)
        {
            *(p + 1) -= '0';
        }
    }
    return sdq_AddToQueue(&fifo_recv, (uint8_t*)topic, topicLength, data, dataLength, cbDelivered);
}

SDHandle_t SDQ_AddToSendQueue(eTOPIC_t topic, uint8_t action, uint8_t *data, uint8_t dataLength, fpDelivered_t cbDelivered)
{
    uint8_t t[4] = {0x0};
    t[0] = (char)topic;
    t[1] = '/';
    t[2] = action;

#ifdef TEST_MAKE_HUMAN_READABLE
    t[0] = t[0] + '0';
    t[2] = t[2] + '0';
#endif

    return sdq_AddToQueue(&fifo_send, t, 3, data, dataLength, cbDelivered);
}
