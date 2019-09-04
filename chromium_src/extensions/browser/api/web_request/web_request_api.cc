/* Copyright 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "../../../../../../extensions/browser/api/web_request/web_request_proxying_websocket.h"
#include "../../../../../../brave/extensions/browser/api/web_request/brave_web_request_proxying_websocket.h"

#define WebRequestProxyingWebSocket BraveWebRequestProxyingWebSocket
#include "../../../../../../extensions/browser/api/web_request/web_request_api.cc"
#undef WebRequestProxyingWebSocket
