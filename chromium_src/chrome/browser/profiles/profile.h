/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_CHROMIUM_SRC_CHROME_BROWSER_PROFILES_PROFILE_H_
#define BRAVE_CHROMIUM_SRC_CHROME_BROWSER_PROFILES_PROFILE_H_

#define BRAVE_PROFILE_H_              \
  Profile* GetParentProfile();        \
                                      \
 private:                             \
  Profile* parent_profile_ = nullptr; \
                                      \
 public:
// define BRAVE_PROFILE_H_

#include "brave/browser/profiles/profile_util.h"
#include "../../../../../chrome/browser/profiles/profile.h"

#endif  // BRAVE_CHROMIUM_SRC_CHROME_BROWSER_PROFILES_PROFILE_H_
