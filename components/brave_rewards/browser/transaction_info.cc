/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_rewards/browser/transaction_info.h"

namespace brave_rewards {

  TransactionInfo::TransactionInfo() :
      id(""),
      type(0),
      amount(0),
      probi(""),
      created_date(0),
      reconciled_date(0),
      rates({}),
      ballots({}) {}

  TransactionInfo::TransactionInfo(const TransactionInfo& info) :
      id(info.id),
      type(info.type),
      amount(info.amount),
      probi(info.probi),
      created_date(info.created_date),
      reconciled_date(info.reconciled_date),
      rates(info.rates),
      ballots(info.ballots) {}

  TransactionInfo::~TransactionInfo() = default;

}  // namespace brave_rewards
