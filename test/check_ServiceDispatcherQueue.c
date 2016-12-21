#include <stdio.h>
#include <stdlib.h>
#include <check.h>
#include <string.h>
#include "fifo.h"
#include "ServiceDispatcher.h"
#include "ServiceDispatcherQueue.h"


START_TEST(ServiceDispatcher_test_Receive)
{
    //INIT!
    void SDQ_Init_ExternalRouter(fpSDQ_SendTo_t fpToService, fpSDQ_SendTo_t fpToCommunication)


    SDHandle_t SDQ_AddToRecvQueue(char *topic, uint8_t *data, uint8_t dataLength, fpDelivered_t cbDelivered)
    void SDQ_Thread()

    //Check receive fp

}
END_TEST


START_TEST(ServiceDispatcher_test_Send)
{
    //INIT!
    void SDQ_Init_ExternalRouter(fpSDQ_SendTo_t fpToService, fpSDQ_SendTo_t fpToCommunication)
    SDHandle_t SDQ_AddToSendQueue(eTOPIC_t topic, uint8_t action, uint8_t *data, uint8_t dataLength, fpDelivered_t cbDelivered)
    void SDQ_Thread()


    //Check send fp
}
END_TEST


int main(void)
{
	int number_failed;
	Suite *suite   = suite_create("ServiceDispatcherQueue");
    TCase *tcase   = tcase_create("Case1");

	/* ADD TESTS HERE*/
    tcase_add_test(tcase, ServiceDispatcher_test_1);

	suite_add_tcase(suite, tcase);		//TODO: move aboud tcase_add_test!!!

	SRunner *runner = srunner_create(suite);
	srunner_run_all(runner, CK_VERBOSE);
	number_failed = srunner_ntests_failed(runner);
	srunner_free(runner);

	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
