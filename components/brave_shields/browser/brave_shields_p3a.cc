/* Copyright 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_shields/browser/brave_shields_p3a.h"

#include "base/logging.h"
#include "base/metrics/histogram_macros.h"
#include "components/prefs/pref_registry_simple.h"
#include "components/prefs/pref_service.h"

namespace brave_shields {

namespace {
// TODO(iefremov): Move to separate header when needed.
constexpr char kPrefName[] = "brave_shields.p3a_usage";
}  // namespace

void RecordShieldsUsageP3A(ShieldsIconUsage usage, PrefService* local_state) {
  DCHECK(local_state);
  int last_value = local_state->GetInteger(kPrefName);

  if (last_value < usage) {
    UMA_HISTOGRAM_ENUMERATION("Brave.Shields.UsageStatus", usage, kSize);
    local_state->SetInteger(kPrefName, usage);
  }
}

void RegisterShieldsP3APrefs(PrefRegistrySimple* local_state) {
  local_state->RegisterIntegerPref(kPrefName, -1);
}

}  // namespace brave_shields
