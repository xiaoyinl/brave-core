/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/browser/net/brave_system_request_handler.h"

#include <memory>
#include <string>
#include <vector>

#include "brave/browser/net/url_context.h"
#include "brave/common/network_constants.h"
#include "chrome/test/base/chrome_render_view_host_test_harness.h"
#include "net/traffic_annotation/network_traffic_annotation_test_helper.h"
#include "net/url_request/url_request_test_util.h"
#include "services/network/public/cpp/resource_request.h"

#if !defined(BRAVE_SERVICES_KEY)
#define BRAVE_SERVICES_KEY "dummytoken"
#endif

namespace brave {

class BraveSystemRequestHandlerTest: public testing::Test {
 public:
  BraveSystemRequestHandlerTest()
      : thread_bundle_(content::TestBrowserThreadBundle::IO_MAINLOOP),
        context_(new net::TestURLRequestContext(true)) {
  }
  ~BraveSystemRequestHandlerTest() override {}
  void SetUp() override {
    context_->Init();
  }
  net::TestURLRequestContext* context() { return context_.get(); }

 private:
  content::TestBrowserThreadBundle thread_bundle_;
  std::unique_ptr<net::TestURLRequestContext> context_;
};

TEST_F(BraveSystemRequestHandlerTest, AddBraveServiceKeyHeader) {
  GURL url("https://demo.brave.com");
  net::TestDelegate test_delegate;
  network::ResourceRequest request;
  
  request.url = url;
  brave::AddBraveServicesKeyHeader(&request);
  std::string key;
  EXPECT_TRUE(request.headers.GetHeader(kBraveServicesKeyHeader, &key));
  EXPECT_EQ(key, BRAVE_SERVICES_KEY);
}

TEST_F(BraveSystemRequestHandlerTest, DontAddBraveServiceKeyHeader) {
  GURL url("https://demo.example.com");
  net::TestDelegate test_delegate;
  network::ResourceRequest request;
  
  request.url = url;
  brave::AddBraveServicesKeyHeader(&request);
  std::string key;
  EXPECT_FALSE(request.headers.GetHeader(kBraveServicesKeyHeader, &key));
}

} // namespace brave