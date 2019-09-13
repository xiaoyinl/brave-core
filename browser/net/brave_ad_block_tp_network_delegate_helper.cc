/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/browser/net/brave_ad_block_tp_network_delegate_helper.h"

#include <memory>
#include <string>

#include "base/base64url.h"
#include "base/strings/string_util.h"
#include "brave/browser/brave_browser_process_impl.h"
#include "brave/browser/net/url_context.h"
#include "brave/common/network_constants.h"
#include "brave/common/shield_exceptions.h"
#include "brave/components/brave_shields/browser/ad_block_custom_filters_service.h"
#include "brave/components/brave_shields/browser/ad_block_regional_service_manager.h"
#include "brave/components/brave_shields/browser/ad_block_service.h"
#include "brave/components/brave_shields/browser/brave_shields_util.h"
#include "brave/components/brave_shields/browser/brave_shields_web_contents_observer.h"
#include "brave/components/brave_shields/common/brave_shield_constants.h"
#include "brave/grit/brave_generated_resources.h"
#include "content/public/browser/browser_thread.h"
#include "extensions/common/url_pattern.h"
#include "ui/base/resource/resource_bundle.h"

using content::ResourceType;

namespace brave {

void OnBeforeURLRequestAdBlockTP(
    std::shared_ptr<BraveRequestInfo> ctx) {
  DCHECK_CURRENTLY_ON(content::BrowserThread::IO);
  // If the following info isn't available, then proper content settings can't
  // be looked up, so do nothing.
  if (ctx->tab_origin.is_empty() || !ctx->tab_origin.has_host() ||
      ctx->request_url.is_empty()) {
    return;
  }
  DCHECK_NE(ctx->request_identifier, 0UL);

  bool did_match_exception = false;
  std::string redirect;
  std::string tab_host = ctx->tab_origin.host();
  if (!g_brave_browser_process->ad_block_service()->ShouldStartRequest(
          ctx->request_url, ctx->resource_type, tab_host,
          &did_match_exception, &ctx->cancel_request_explicitly,
          &ctx->redirect)) {
    ctx->blocked_by = kAdBlocked;
  } else if (!did_match_exception &&
             !g_brave_browser_process->ad_block_regional_service_manager()
                  ->ShouldStartRequest(ctx->request_url, ctx->resource_type,
                                       tab_host, &did_match_exception,
                                       &ctx->cancel_request_explicitly,
                                       &redirect)) {
    ctx->blocked_by = kAdBlocked;
  } else if (!did_match_exception &&
             !g_brave_browser_process->ad_block_custom_filters_service()
                  ->ShouldStartRequest(ctx->request_url, ctx->resource_type,
                                       tab_host, &did_match_exception,
                                       &ctx->cancel_request_explicitly,
                                       &redirect)) {
    ctx->blocked_by = kAdBlocked;
  }

  if (!redirect.empty()) {
    ctx->new_url_spec = redirect;
    LOG(ERROR) << "=====USING A REDIRECT RESOURCE OF: " << ctx->new_url_spec;
  }

  if (ctx->blocked_by == kAdBlocked) {
    brave_shields::DispatchBlockedEventFromIO(ctx->request_url,
        ctx->render_frame_id, ctx->render_process_id, ctx->frame_tree_node_id,
        brave_shields::kAds);
  }
}

int OnBeforeURLRequest_AdBlockTPPreWork(
    const ResponseCallback& next_callback,
    std::shared_ptr<BraveRequestInfo> ctx) {

  if (ctx->request_url.is_empty()) {
    return net::OK;
  }

  // Most blocked resources have been moved to our ad block lists.
  // This is only for special cases like the PDFjs ping which can
  // occur before the ad block lists are fully loaded.
  if (IsBlockedResource(ctx->request_url)) {
    ctx->new_url_spec = kEmptyDataURI;

    return net::OK;
  }

  // If the following info isn't available, then proper content settings can't
  // be looked up, so do nothing.
  if (ctx->tab_origin.is_empty() || !ctx->allow_brave_shields ||
      ctx->allow_ads ||
      ctx->resource_type == BraveRequestInfo::kInvalidResourceType) {
    return net::OK;
  }

  OnBeforeURLRequestAdBlockTP(ctx);

  return net::OK;
}

}  // namespace brave
