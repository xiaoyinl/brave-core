/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_CHROMIUM_SRC_CHROME_BROWSER_PROFILES_PROFILE_IMPL_H_
#define BRAVE_CHROMIUM_SRC_CHROME_BROWSER_PROFILES_PROFILE_IMPL_H_

// parent_profile_ is put in ProfileImpl instead of BraveProfileImpl because
// it will be accessed when running ProfileImpl's constructor, which is before
// the subclass is constructed, so we will access an uninitialized value if we
// put it in the subclass.
#define BRAVE_PROFILE_IMPL_H_         \
  Profile* GetParentProfile();        \
                                      \
 private:                             \
  friend class BraveProfileImpl;      \
  Profile* parent_profile_ = nullptr; \
                                      \
 public:
// define BRAVE_PROFILE_IMPL_H_

#include "../../../../../../chrome/browser/profiles/profile_impl.h"

#undef BRAVE_PROFILE_IMPL_H_

#endif  // BRAVE_CHROMIUM_SRC_CHROME_BROWSER_PROFILES_PROFILE_IMPL_H_
