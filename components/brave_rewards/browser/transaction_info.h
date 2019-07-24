/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_REWARDS_BROWSER_TRANSACTION_INFO_H_
#define BRAVE_COMPONENTS_BRAVE_REWARDS_BROWSER_TRANSACTION_INFO_H_

#include <stdint.h>
#include <string>
#include <vector>
#include <map>

#include "bat/ledger/publisher_info.h"

namespace brave_rewards {

struct TransactionInfo {
  TransactionInfo();
  TransactionInfo(const TransactionInfo& info);
  ~TransactionInfo();

  std::string id;
  int type;
  double amount;
  std::string probi;
  uint32_t created_date;
  uint32_t reconciled_date;

  std::map<std::string, double> rates;

  std::map<std::string, int> ballots;
};

}  // namespace brave_rewards

#endif  // BRAVE_COMPONENTS_BRAVE_REWARDS_BROWSER_TRANSACTION_INFO_H_
