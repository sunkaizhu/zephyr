/*
 * Copyright (c) 2016 Intel Corporation.
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

#include <zephyr.h>
#include <errno.h>

#include <net/net_if.h>
#include <net/net_core.h>
#include <net/net_mgmt.h>
#include <net/ieee802154.h>

#include <misc/printk.h>
#define PRINT	printk

#ifndef CONFIG_NET_L2_IEEE802154_SHELL

static struct ieee802154_req_params scan_ctx;
static struct net_mgmt_event_callback scan_cb;

static void scan_result_cb(struct net_mgmt_event_callback *cb,
			   uint32_t mgmt_event, struct net_if *iface)
{
	PRINT("Got a scan result:\n");
	PRINT("\tChannel: %u\n", scan_ctx.channel);
	PRINT("\tPAN ID : %u\n", scan_ctx.pan_id);
	PRINT("\tLQI    : %u\n", scan_ctx.lqi);
}

static inline void scan(struct net_if *iface)
{
	size_t len = sizeof(struct ieee802154_req_params);
	int ret;

	net_mgmt_init_event_callback(&scan_cb, scan_result_cb,
				     NET_EVENT_IEEE802154_SCAN_RESULT);
	memset(&scan_ctx, 0, len);
	scan_ctx.channel_set = IEEE802154_ALL_CHANNELS;
	scan_ctx.duration = K_SECONDS(1);

	ret = net_mgmt(NET_REQUEST_IEEE802154_ACTIVE_SCAN,
		       iface, &scan_ctx, len);
	if (ret < 0) {
		PRINT("Scan did not proceed well %u\n", ret);
	}
}

#else
#define scan(...)
#endif /* CONFIG_NET_L2_IEEE802154_SHELL */

static struct net_if *init_device(void)
{
	struct net_if *iface;
	struct device *dev;

	dev = device_get_binding(CONFIG_TI_CC2520_DRV_NAME);
	if (!dev) {
		PRINT("Cannot get CC250 device\n");
		return NULL;
	}

	iface = net_if_lookup_by_dev(dev);
	if (!iface) {
		PRINT("Cannot get CC2520 network interface\n");
		return NULL;
	}

	PRINT("802.15.4 device up and running\n");

	return iface;
}
void main(void)
{
	struct net_if *iface;

	iface = init_device();
	if (!iface) {
		goto end;
	}

	k_sleep(K_SECONDS(5));

	scan(iface);
end:
	return;
}
