/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_BROWSER_BRAVE_WEBCOMPAT_REPORTER_WEBCOMPAT_REPORTER_SERVICE_H_
#define BRAVE_BROWSER_BRAVE_WEBCOMPAT_REPORTER_WEBCOMPAT_REPORTER_SERVICE_H_

#include <string>

#include "base/macros.h"

namespace network {
class SimpleURLLoader;
}

namespace brave {

class BraveWebcompatReporterService {
 public:
  explicit BraveWebcompatReporterService();
  ~BraveWebcompatReporterService();

  void SubmitReport(std::string report_domain);

 private:
  std::unique_ptr<network::SimpleURLLoader> simple_url_loader_;
  void CreateAndStartURLLoader(const std::string& upload_url, const std::string& post_data);
  void OnSimpleURLLoaderComplete(std::unique_ptr<std::string> response_body);

  DISALLOW_COPY_AND_ASSIGN(BraveWebcompatReporterService);
};

} // namespace brave

#endif // BRAVE_BROWSER_BRAVE_WEBCOMPAT_REPORTER_WEBCOMPAT_REPORTER_SERVICE_H_
