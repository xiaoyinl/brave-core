/* Copyright 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_rewards/browser/rewards_p3a.h"

#include "base/metrics/histogram_macros.h"
#include "base/strings/string_number_conversions.h"
#include "base/values.h"

namespace brave_rewards {

void RecordWalletBalanceP3A(bool wallet_created, size_t b) {
  int answer = 0;
  if (wallet_created) {
    if (b < 10) {
      answer = 1;
    } else if (10 <= b && b < 50) {
      answer = 2;
    } else if (50 <= b) {
      answer = 3;
    }
  }
  UMA_HISTOGRAM_EXACT_LINEAR("Brave.Rewards.WalletBalance", answer, 3);
}

void RecordAutoContributionsState(AutoContributionsP3AState state, int count) {
  DCHECK_GE(count, 0);
  int answer = 0;
  switch (state) {
    case AutoContributionsP3AState::kNoWallet:
      break;
    case AutoContributionsP3AState::kWalletCreatedAutoContributeOff:
      answer = 1;
      break;
    case AutoContributionsP3AState::kAutoContributeOn:
      switch (count) {
        case 0:
          answer = 2;
          break;
        case 1:
          answer = 3;
          break;
        default:
          answer = 4;
      }
      break;
    default:
      NOTREACHED();
  }
  UMA_HISTOGRAM_EXACT_LINEAR("Brave.Rewards.AutoContributionsState", answer, 4);
}

void RecordTipsState(bool wallet_created,
                     int one_time_count,
                     int recurring_count) {
  DCHECK_GE(one_time_count, 0);
  DCHECK_GE(recurring_count, 0);

  int answer = 0;
  if (wallet_created) {
    if (one_time_count == 0 && recurring_count == 0) {
      answer = 1;
    } else if (one_time_count > 0 && recurring_count > 0) {
      answer = 4;
    } else if (one_time_count > 0) {
      answer = 2;
    } else {
      answer = 3;
    }
  }
  UMA_HISTOGRAM_EXACT_LINEAR("Brave.Rewards.TipsState", answer, 4);
}

void RecordAdsState(AdsP3AState state) {
  UMA_HISTOGRAM_ENUMERATION("Brave.Rewards.AdsState", state);
}

void RecordNoWalletCreatedForAllMetrics() {
  RecordWalletBalanceP3A(false, 0);
  RecordAutoContributionsState(AutoContributionsP3AState::kNoWallet, 0);
  RecordTipsState(false, 0, 0);
  RecordAdsState(AdsP3AState::kNoWallet);
}

double CalcWalletBalanceForP3A(base::flat_map<std::string, double> wallets,
                               std::string user_funds) {
  double balance_minus_grant = 0.0;
  for (const auto& wallet : wallets) {
    // Skip anonymous wallet, since it can contain grants.
    if (wallet.first == "anonymous") {
      continue;
    }
    balance_minus_grant += static_cast<size_t>(wallet.second);
  }

  // `user_funds` is the amount of user-funded BAT
  // in the anonymous wallet (ex: not grants).
  double user_funds_value;
  balance_minus_grant +=
      base::StringToDouble(user_funds, &user_funds_value);
  return balance_minus_grant;
}

uint64_t RoundProbiToUint64(base::StringPiece probi) {
  if (probi.size() < 18) return 0;
  uint64_t grant = 0;
  base::StringToUint64(probi.substr(0, probi.size() - 18), &grant);
  return grant;
}

void ExtractAndLogP3AStats(const base::DictionaryValue& dict) {
  const base::Value* probi_value =
      dict.FindPath({"walletProperties", "probi_"});
  if (!probi_value || !probi_value->is_string()) {
    LOG(WARNING) << "Bad ledger state";
    return;
  }

  // Get grants.
  const base::Value* grants_value = dict.FindKey("grants");
  uint64_t total_grants = 0;
  if (grants_value) {
    if (!grants_value->is_list()) {
      LOG(WARNING) << "Bad grant value in ledger_state.";
    } else {
      const auto& grants = grants_value->GetList();
      // Sum all grants.
      for (const auto& grant : grants) {
        if (!grant.is_dict()) {
          LOG(WARNING) << "Bad grant value in ledger_state.";
          continue;
        }
        const base::Value* grant_amount = grant.FindKey("probi_");
        const base::Value* grant_currency = grant.FindKey("altcurrency");
        if (grant_amount->is_string() && grant_currency->is_string() &&
            grant_currency->GetString() == "BAT") {
          // Some kludge computations because we don't want to be very precise
          // for P3A purposes. Assuming grants can't be negative and are
          // greater than 1 BAT.
          const std::string& grant_str = grant_amount->GetString();
          total_grants += RoundProbiToUint64(grant_str);
        }
      }
    }
  }
  const uint64_t total =
      RoundProbiToUint64(probi_value->GetString()) - total_grants;
  RecordWalletBalanceP3A(true, total);
}

}  // namespace brave_rewards
