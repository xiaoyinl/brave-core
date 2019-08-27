/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "brave/browser/profiles/profile_util.h"

// The dummy regular profile we created for Tor shares the same extension
// registry with its parent profile, since the registry is already being
// observed when parent profile is added, we will not add the registry to the
// observer again when the dummy regular profile is added. Without this, we
// would hit DCHECK by trying to add the same registry to the observer again.
#define BRAVE_IGNORE_TOR_PROFILE_ADDED_                      \
  Profile* profile = content::Source<Profile>(source).ptr(); \
  if (brave::IsTorProfile(profile)) {                        \
    break;                                                   \
  }

#include "../../../../../chrome/browser/extensions/user_script_listener.cc"
#undef BRAVE_IGNORE_TOR_PROFILE_ADDED_
