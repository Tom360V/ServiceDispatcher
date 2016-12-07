/*
 * ServiceDispatchConfig.h
 *
 *  Created on: Nov 4, 2016
 *      Author: nxa03718
 */

#ifndef SERVICEDISPATCHER_SRC_SERVICEDISPATCHCONFIG_H_
#define SERVICEDISPATCHER_SRC_SERVICEDISPATCHCONFIG_H_

#include <stdint.h>
#include <stddef.h>

#define TEST_MAKE_HUMAN_READABLE    (1)

typedef enum        //TODO: Not the correct place to define this, should be somewhere eg "Services.h"
{
    eTOPIC_GENERAL = 0,
    eTOPIC_RGB_CLIENT,
    eTOPIC_RGB_SERVER,
} eTOPIC_t;

#define SDQ_Q_TOPIC         (15)
#define SDQ_Q_DATA          (20)
#define SD_NOF_TOPICS       (10)

typedef int8_t SDHandle_t;

typedef uint8_t RemoteFuctionId_t;
typedef void (*RemoteFP_t)(void *pData);
typedef void (*fpDelivered_t)(SDHandle_t handle, int8_t succeed);

typedef struct
{
    RemoteFuctionId_t   remoteFunctionId;
    RemoteFP_t          remoteFunction;
}RemoteFunctionItem_t;

typedef void (*fpServiceSubscriber_t)(RemoteFunctionItem_t *pActionList, uint8_t nof_entries, eTOPIC_t topic, char *topicLong);


#endif /* SERVICEDISPATCHER_SRC_SERVICEDISPATCHCONFIG_H_ */
