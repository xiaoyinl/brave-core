/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_rewards/browser/pending_contribution.h"

namespace brave_rewards {

PendingContributionInfo::PendingContributionInfo() :
  amount(0),
  added_date(0) {
}

PendingContributionInfo::PendingContributionInfo(
    const PendingContributionInfo& info) {
  publisher_key = info.publisher_key;
  verified = info.verified;
  name = info.name;
  favicon_url = info.favicon_url;
  url = info.url;
  provider = info.provider;
  amount = info.amount;
  added_date = info.added_date;
  viewing_id = info.viewing_id;
  type = info.type;
  expiration_date = info.expiration_date;
}

PendingContributionInfo::~PendingContributionInfo() = default;

}  // namespace brave_rewards
