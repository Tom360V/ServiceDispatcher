/*
 * ServiceDispatcherRouter.h
 *
 *  Created on: Nov 4, 2016
 *      Author: nxa03718
 */

#ifndef SERVICEDISPATCHER_INC_SERVICEDISPATCHERROUTER_H_
#define SERVICEDISPATCHER_INC_SERVICEDISPATCHERROUTER_H_

#include "ServiceDispatcher.h"

void    SDR_Init();
void    SDR_SubscribeService(RemoteFunctionItem_t *pActionList, uint8_t nofActions, eTOPIC_t topic, char *topicName);
int8_t  SDR_Rout(SDHandle_t handle, uint8_t *topic, uint8_t topicLength,
                                    uint8_t *data,  uint8_t dataLength );

#endif /* SERVICEDISPATCHER_INC_SERVICEDISPATCHERROUTER_H_ */
