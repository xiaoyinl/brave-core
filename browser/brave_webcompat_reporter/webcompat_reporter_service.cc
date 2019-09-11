/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/browser/brave_webcompat_reporter/webcompat_reporter_service.h"

#include <string>

#include "net/base/load_flags.h"
#include "content/public/browser/browser_task_traits.h"
#include "base/task/post_task.h"
#include "services/network/public/cpp/shared_url_loader_factory.h"
#include "services/network/public/cpp/simple_url_loader.h"
#include "chrome/browser/browser_process.h"
#include "base/json/json_writer.h"
#include "base/environment.h"
#include "brave/components/brave_referrals/browser/brave_referrals_service.h"
#include "brave/common/network_constants.h"

namespace brave {

BraveWebcompatReporterService::BraveWebcompatReporterService() {}

BraveWebcompatReporterService::~BraveWebcompatReporterService() {}

void BraveWebcompatReporterService::SubmitReport(std::string report_domain) {
  std::string api_key = brave::GetAPIKey();

  std::unique_ptr<base::Environment> env(base::Environment::Create());
  std::string upload_url;
  env->GetVar("BRAVE_WEBCOMPAT_REPORT_ENDPOINT", &upload_url);
  if (upload_url.empty())
    upload_url = kBraveWebcompatReportEndpoint;

  base::Value post_data_obj(base::Value::Type::DICTIONARY);
  post_data_obj.SetKey("domain", base::Value(report_domain));
  post_data_obj.SetKey("api_key", base::Value(api_key));

  std::string post_data;
  base::JSONWriter::Write(post_data_obj, &post_data);

  base::PostTaskWithTraits(FROM_HERE, {content::BrowserThread::UI},
      base::BindOnce(&BraveWebcompatReporterService::CreateAndStartURLLoader,
          base::Unretained(this), upload_url, post_data));
}

void BraveWebcompatReporterService::CreateAndStartURLLoader(
    const std::string& upload_url,
    const std::string& post_data) {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);

  std::string content_type = "application/json";
  auto resource_request = std::make_unique<network::ResourceRequest>();
  resource_request->url = GURL(upload_url);
  resource_request->method = "POST";
  resource_request->load_flags = net::LOAD_DO_NOT_SEND_COOKIES |
                                 net::LOAD_BYPASS_CACHE |
                                 net::LOAD_DISABLE_CACHE |
                                 net::LOAD_DO_NOT_SEND_AUTH_DATA;

  net::NetworkTrafficAnnotationTag traffic_annotation =
      net::DefineNetworkTrafficAnnotation("background_performance_tracer", R"(
        semantics {
          sender: "Brave Web Compatibility Reporting"
          description:
            "A user-initiated report of a website that appears broken as a"
            "result of having Brave Shields enabled."
          trigger:
            "Though the 'Report a Broken Site' option of the help menu or"
            "the Brave Shields panel."
          data: "Broken domain and IP address."
          destination: OTHER
          destination_other: "Brave developers"
        }
        policy {
          cookies_allowed: NO
        })");

  simple_url_loader_ = network::SimpleURLLoader::Create(
      std::move(resource_request), traffic_annotation);
  simple_url_loader_->AttachStringForUpload(post_data, content_type);
  simple_url_loader_->DownloadToStringOfUnboundedSizeUntilCrashAndDie(
      g_browser_process->shared_url_loader_factory().get(),
      base::BindOnce(&BraveWebcompatReporterService::OnSimpleURLLoaderComplete,
                     base::Unretained(this)));
}

void BraveWebcompatReporterService::OnSimpleURLLoaderComplete(
    std::unique_ptr<std::string> response_body) {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);

  std::string feedback;
  bool success = !!response_body;

  if (success) {
    LOG(INFO) << "Successfully uploaded webcompat report. Thanks!" << std::endl;
  } else {
    LOG(ERROR) << "Uploading webcompat report failed - please try again later!" << std::endl;
  }
}

} // namespace brave
