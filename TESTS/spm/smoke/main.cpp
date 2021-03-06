/* Copyright (c) 2017 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#if !ENABLE_SPM
    #error [NOT_SUPPORTED] SPM is not supported on this platform
#endif

/* -------------------------------------- Includes ----------------------------------- */

#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include "spm_client.h"
#include "psa_smoke_test_part1_ifs.h"

using namespace utest::v1;

/* ------------------------------------ Definitions ---------------------------------- */

#define CLIENT_MINOR_VERSION            0
#define CLIENT_RSP_BUF_SIZE             20

#define CLIENT_TX_MSG                   "Hello and welcome SPM"
#define CLIENT_EXPECTED_RESPONSE        "Response1"

/* ------------------------------------ Client Code ---------------------------------- */

char msg_buf[] = CLIENT_TX_MSG;

void example_main(void)
{
    psa_handle_t conn_handle = psa_connect(SF1, CLIENT_MINOR_VERSION);
    TEST_ASSERT_MESSAGE(conn_handle > 0, "psa_connect() failed");


    psa_invec_t iovec[PSA_MAX_INVEC_LEN] = {
        { msg_buf, 6 },
        { msg_buf + 6, 12 },
        { msg_buf + 18, 4 }
    };

    uint8_t *response_buf = (uint8_t*)malloc(sizeof(uint8_t) * CLIENT_RSP_BUF_SIZE);
    memset(response_buf, 0, CLIENT_RSP_BUF_SIZE);
    psa_outvec_t outvec = {response_buf, CLIENT_RSP_BUF_SIZE};

    psa_error_t status = psa_call(conn_handle, iovec, 3, &outvec, 1);
    TEST_ASSERT_MESSAGE(PSA_SUCCESS == status, "psa_call() failed");
    TEST_ASSERT_EQUAL_STRING(CLIENT_EXPECTED_RESPONSE, response_buf);

    free(response_buf);
    status = psa_close(conn_handle);
    TEST_ASSERT_MESSAGE(PSA_SUCCESS == status, "psa_close() failed");
}

// --------------------------------- Test Framework ---------------------------------- */

utest::v1::status_t greentea_setup(const size_t number_of_cases) {
#ifndef NO_GREENTEA
    GREENTEA_SETUP(20, "default_auto");
#endif
    // Call the default reporting function
    return greentea_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("example", example_main)
};

// Declare your test specification with a custom setup handler
Specification specification(greentea_setup, cases);

int main(int, char **)
{   // Run the test specification
    Harness::run(specification);
    return 0;
}
