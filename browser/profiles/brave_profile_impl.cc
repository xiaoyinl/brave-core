/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/browser/profiles/brave_profile_impl.h"

#include "chrome/browser/browser_process.h"
#include "chrome/browser/profiles/profile_manager.h"

BraveProfileImpl::BraveProfileImpl(
    const base::FilePath& path,
    Delegate* delegate,
    CreateMode create_mode,
    scoped_refptr<base::SequencedTaskRunner> io_task_runner)
  : ProfileImpl(path, delegate, create_mode, io_task_runner) {
}

BraveProfileImpl::~BraveProfileImpl() {
}

Profile* BraveProfileImpl::GetOriginalProfile() {
  if (IsTorProfile()) {
    // For profile path user_data_dir/Default/session_profiles/Tor Profile,
    // the original profile path is user_data_dir/Default.
    base::FilePath original_path = GetPath().DirName().DirName();
    ProfileManager* profile_manager = g_browser_process->profile_manager();
    Profile* original_profile = profile_manager->GetProfileByPath(original_path);
    DCHECK(original_profile);
    return original_profile;
  }
  return ProfileImpl::GetOriginalProfile();
}

bool BraveProfileImpl::IsSameProfile(Profile* profile) {
  // Match between Tor profile and its original profile.
  if ((IsTorProfile() &&
        GetPath().DirName().DirName() == profile->GetPath()) ||
      (profile->IsTorProfile() &&
       profile->GetPath().DirName().DirName() == GetPath())) {
    return true;
  }
  return ProfileImpl::IsSameProfile(profile);
}

bool BraveProfileImpl::IsOffTheRecord() const {
  return IsTorProfile() || ProfileImpl::IsOffTheRecord();
}
