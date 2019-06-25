/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#define BRAVE_DISABLE_DOWNLOAD_PROTECTION_REMOTE_LOOKUPS \
  FinishRequest(DownloadCheckResult::UNKNOWN, REASON_PING_DISABLED); \
  return;

#include "../../../../../chrome/browser/safe_browsing/download_protection/check_client_download_request.cc"  // NOLINT
#undef BRAVE_DISABLE_DOWNLOAD_PROTECTION_REMOTE_LOOKUPS
