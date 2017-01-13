/*
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
#include "test_oic.h"

#include <os/os.h>
#include <oic/oc_api.h>

static int test_discovery_state;
static volatile int test_discovery_done;

static void test_discovery_next_step(struct os_event *);
static struct os_event test_discovery_next_ev = {
    .ev_cb = test_discovery_next_step
};

/*
 * Discovery.
 */
static void
test_discovery_init(void)
{
    oc_init_platform("TestPlatform", NULL, NULL);
}

static void
test_discovery_client_requests(void)
{
}

static oc_handler_t test_discovery_handler = {
    .init = test_discovery_init,
    .requests_entry = test_discovery_client_requests
};

static oc_discovery_flags_t
test_discovery_cb(const char *di, const char *uri, oc_string_array_t types,
                  oc_interface_mask_t interfaces, oc_server_handle_t *server)
{
    if ((server->endpoint.oe.flags & IP) == 0) {
        return 0;
    }
    switch (test_discovery_state) {
    case 1:
        TEST_ASSERT(!strcmp(uri, "/oic/p"));
        TEST_ASSERT(interfaces == (OC_IF_BASELINE | OC_IF_R));
        os_eventq_put(&oic_tapp_evq, &test_discovery_next_ev);
        return OC_STOP_DISCOVERY;
        break;
    case 2: {
        static int seen_p = 0;
        static int seen_d = 0;

        if (!strcmp(uri, "/oic/p")) {
            seen_p = 1;
        } else if (!strcmp(uri, "/oic/d")) {
            seen_d = 1;
        } else {
            TEST_ASSERT(0);
        }
        if (seen_p && seen_d) {
            os_eventq_put(&oic_tapp_evq, &test_discovery_next_ev);
            return OC_STOP_DISCOVERY;
        } else {
            return OC_CONTINUE_DISCOVERY;
        }
    }
    case 3: {
        static int seen_p = 0;
        static int seen_d = 0;
        static int seen_light = 0;

        if (!strcmp(uri, "/oic/p")) {
            seen_p = 1;
        } else if (!strcmp(uri, "/oic/d")) {
            seen_d = 1;
        } else if (!strcmp(uri, "/light/test")) {
            seen_light = 1;
        } else {
            TEST_ASSERT(0);
        }
        if (seen_p && seen_d && seen_light) {
            os_eventq_put(&oic_tapp_evq, &test_discovery_next_ev);
            /*
             * Done.
             */
            test_discovery_done = 1;
            return OC_STOP_DISCOVERY;
        } else {
            return OC_CONTINUE_DISCOVERY;
        }
    }
    default:
        break;
    }
    TEST_ASSERT_FATAL(0);
    return 0;
}

static void
test_discovery_get(oc_request_t *req, oc_interface_mask_t interface)
{
}

static void
test_discovery_next_step(struct os_event *ev)
{
    test_discovery_state++;
    switch (test_discovery_state) {
    case 1:
        /*
         * No resources registered yet.
         */
        oc_do_ip_discovery(NULL, test_discovery_cb);
        oic_test_reset_tmo();
        break;
    case 2:
        oc_add_device("/oic/d", "oic.d.light", "TestDev", "1.0", "1.1",
          NULL, NULL);
        oc_do_ip_discovery(NULL, test_discovery_cb);
        oic_test_reset_tmo();
        break;
    case 3: {
        oc_resource_t *res = oc_new_resource("/light/test", 1, 0);

        oc_resource_bind_resource_type(res, "oic.r.light");
        oc_resource_bind_resource_interface(res, OC_IF_RW);
        oc_resource_set_default_interface(res, OC_IF_RW);

        oc_resource_set_discoverable(res);
        oc_resource_set_request_handler(res, OC_GET, test_discovery_get);
        oc_add_resource(res);
        oc_do_ip_discovery(NULL, test_discovery_cb);
        oic_test_reset_tmo();
        break;
    }
    default:
        TEST_ASSERT(0);
        break;
    }
}

void
test_discovery(void)
{
    oc_main_init(&test_discovery_handler);
    os_eventq_put(&oic_tapp_evq, &test_discovery_next_ev);

    while (!test_discovery_done) {
        os_eventq_run(&oic_tapp_evq);
    }
    oc_main_shutdown();
}

