#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <check.h>

#include "fifo.h"
#include "ServiceDispatcher.h"
#include "ServiceDispatcherQueue.h"

#define CBSEND_MAX  (10)
typedef struct
{
    SDHandle_t sdqHandle;
    uint8_t topic[SDQ_Q_TOPIC];
    uint8_t topicLength;
    uint8_t data[SDQ_Q_DATA];
    uint8_t dataLength;
}cbSend_t;

typedef struct
{
    uint8_t write;
    uint8_t read;
    uint8_t calls;
    cbSend_t expectations[CBSEND_MAX];
}cbSendArray_t;


cbSendArray_t cbSendArray;

void cbSend_SetExpectations(SDHandle_t sdqHandle, uint8_t *topic, uint8_t topicLength, uint8_t *data, uint8_t dataLength)
{
    static uint8_t firsttime = 0;

    if(firsttime == 0)
    {
        firsttime++;
        cbSendArray.write = 0;
        cbSendArray.read = 0;
    }

    ck_assert(cbSendArray.write <= CBSEND_MAX);

    //Add item
    cbSend_t *p = &cbSendArray.expectations[cbSendArray.write];
    cbSendArray.write++;
    cbSendArray.calls++;

    p->sdqHandle = sdqHandle;
    memcpy(p->topic, topic, topicLength);
    p->topicLength = topicLength;
    memcpy(p->data, data, dataLength);
    p->dataLength = dataLength;
}

void cbSend_RoundUp()
{
    ck_assert_int_eq(cbSendArray.write, cbSendArray.read);
}

int8_t cbSend(SDHandle_t sdqHandle, uint8_t *topic, uint8_t topicLength, uint8_t *data, uint8_t dataLength)
{
    ck_assert(cbSendArray.write > cbSendArray.read);

    cbSend_t *p = &cbSendArray.expectations[cbSendArray.read];
    cbSendArray.read++;

    ck_assert_int_eq(p->topicLength, topicLength);
    ck_assert(memcmp(p->topic, topic, topicLength) == 0);

    ck_assert_int_eq(p->dataLength, dataLength);
    ck_assert(memcmp(p->data,  data,  dataLength)  == 0);
}

int8_t cbReceive(SDHandle_t sdqHandle, uint8_t *topic, uint8_t topicLength, uint8_t *data, uint8_t dataLength)
{

}

START_TEST(ServiceDispatcher_test_Receive)
{
    //INIT!
    SDQ_Init_ExternalRouter(&cbSend, &cbReceive);

    //Add something to Receive queue
    cbSend_SetExpectations(0, "topic", 5, "data", 4);
    cbSend_SetExpectations(0, "abcdefg", 7, "123123", 6);

    ck_assert(SDQ_AddToRecvQueue(   "topic" /*char**/,
                                    "data" /*uint8_t**/,
                                    4 /*uint8_t*/,
                                    NULL /*fpDelivered_t*/) >= 0);

    ck_assert(SDQ_AddToRecvQueue(   "abcdefg" /*char**/,
                                    "123123" /*uint8_t**/,
                                    6 /*uint8_t*/,
                                    NULL /*fpDelivered_t*/) >= 0);

    //Tickele thread
    SDQ_Thread();
    SDQ_Thread();

    cbSend_RoundUp();

}
END_TEST


START_TEST(ServiceDispatcher_test_Send)
{
    //INIT!
//    void SDQ_Init_ExternalRouter(fpSDQ_SendTo_t fpToService, fpSDQ_SendTo_t fpToCommunication)
//    SDHandle_t SDQ_AddToSendQueue(eTOPIC_t topic, uint8_t action, uint8_t *data, uint8_t dataLength, fpDelivered_t cbDelivered)
//    void SDQ_Thread()


    //Check send fp
}
END_TEST


int main(void)
{
	int number_failed;
	Suite *suite   = suite_create("ServiceDispatcherQueue");
    TCase *tcase   = tcase_create("Case1");

	/* ADD TESTS HERE*/
    tcase_add_test(tcase, ServiceDispatcher_test_Receive);
    tcase_add_test(tcase, ServiceDispatcher_test_Send);

	suite_add_tcase(suite, tcase);		//TODO: move aboud tcase_add_test!!!

	SRunner *runner = srunner_create(suite);
	srunner_run_all(runner, CK_VERBOSE);
	number_failed = srunner_ntests_failed(runner);
	srunner_free(runner);

	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
