/*
 * ServiceDispatcherQueue.h
 *
 *  Created on: Nov 4, 2016
 *      Author: nxa03718
 */

#ifndef SERVICEDISPATCHER_INC_SERVICEDISPATCHERQUEUE_H_
#define SERVICEDISPATCHER_INC_SERVICEDISPATCHERQUEUE_H_

#include "ServiceDispatcher.h"

typedef void   (*fpSDQ_Delivered_t) (SDHandle_t sdqHandle);
typedef int8_t (*fpSDQ_SendTo_t)    (SDHandle_t sdqHandle, uint8_t *topic, uint8_t topicLength, uint8_t *data, uint8_t dataLength);

void        SDQ_Init(fpSDQ_SendTo_t fpToCommunication);
void        SDQ_Init_ExternalRouter(fpSDQ_SendTo_t fpToService, fpSDQ_SendTo_t fpToCommunication);
void        SDQ_Thread();
SDHandle_t  SDQ_AddToRecvQueue(uint8_t *topic, uint8_t topicLength, uint8_t *data, uint8_t dataLength, fpDelivered_t cbDelivered);
SDHandle_t  SDQ_AddToSendQueue(eTOPIC_t topic, uint8_t action,      uint8_t *data, uint8_t dataLength, fpDelivered_t cbDelivered);

/* TKE: Not necessary?!
int8_t      SDQ_DeleteFromRecvQueue(SDHandle_t sdqHandle);
int8_t      SDQ_DeleteFromSendQueue(SDHandle_t sdqHandle);
*/
#endif /* SERVICEDISPATCHER_INC_SERVICEDISPATCHERQUEUE_H_ */
