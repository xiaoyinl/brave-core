/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "brave/browser/extensions/api/brave_extensions_api_client.h"
#include "brave/browser/extensions/updater/brave_update_client_config.h"
#include "brave/browser/profiles/profile_util.h"
#include "chrome/browser/extensions/chrome_extensions_browser_client.h"
#include "chrome/browser/extensions/updater/chrome_update_client_config.h"

#define BRAVE_GET_ORIGINAL_CONTEXT_             \
  if (brave::IsTorProfile(context)) {           \
    return brave::GetTorParentProfile(context); \
  }
// define BRAVE_GET_ORIGINAL_CONTEXT_

#define ChromeUpdateClientConfig BraveUpdateClientConfig
#define ChromeExtensionsAPIClient BraveExtensionsAPIClient
#include "../../../../../chrome/browser/extensions/chrome_extensions_browser_client.cc"  // NOLINT
#undef ChromeUpdateClientConfig
#undef ChromeExtensionsAPIClient
#undef BRAVE_GET_ORIGINAL_CONTEXT_
