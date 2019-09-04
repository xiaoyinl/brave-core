/* Copyright 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/extensions/browser/api/web_request/brave_web_request_proxying_websocket.h"

#include "base/task/post_task.h"
#include "brave/browser/net/brave_request_handler.h"
#include "brave/browser/net/resource_context_data.h"
#include "content/public/browser/browser_context.h"
#include "content/public/browser/browser_task_traits.h"
#include "content/public/browser/resource_context.h"

// static
void BraveWebRequestProxyingWebSocket::StartProxying(
  int frame_tree_node_id,
  WebSocketFactory factory,
  const GURL& url,
  const GURL& site_for_cookies,
  const base::Optional<std::string>& user_agent,
  network::mojom::WebSocketHandshakeClientPtrInfo handshake_client,
  bool has_extra_headers,
  int process_id,
  int render_frame_id,
    scoped_refptr<extensions::WebRequestAPI::RequestIDGenerator>
        request_id_generator,
  const url::Origin& origin,
  content::BrowserContext* browser_context,
  extensions::WebRequestAPI::ProxySet* proxies) {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);
  auto* resource_context_data =
      ResourceContextData::GetResourceContextDataForBrowserContext(
          browser_context);
  network::ResourceRequest request;
  request.url = url;
  // TODO(iefremov): site_for_cookies is not enough, we should find a way
  // to initialize NetworkIsolationKey.
  request.site_for_cookies = site_for_cookies;
  if (user_agent) {
    request.headers.SetHeader(net::HttpRequestHeaders::kUserAgent, *user_agent);
  }
  request.request_initiator = origin;
  request.render_frame_id = render_frame_id;

  auto proxy = std::make_unique<BraveWebRequestProxyingWebSocket>(
      std::move(factory), request,
      network::mojom::WebSocketHandshakeClientPtr(std::move(handshake_client)),
      has_extra_headers, process_id, render_frame_id, frame_tree_node_id,
      browser_context, request_id_generator, proxies,
      resource_context_data->request_handler());

  auto* raw_proxy = proxy.get();
  proxies->AddProxy(std::move(proxy));
  raw_proxy->Start();
}

BraveWebRequestProxyingWebSocket::BraveWebRequestProxyingWebSocket(
    WebSocketFactory factory,
    const network::ResourceRequest& request,
    network::mojom::WebSocketHandshakeClientPtr handshake_client,
    bool has_extra_headers,
    int process_id,
    int render_frame_id,
    int frame_tree_node_id,
    content::BrowserContext* browser_context,
    scoped_refptr<extensions::WebRequestAPI::RequestIDGenerator>
        request_id_generator,
    extensions::WebRequestAPI::ProxySet* proxies,
    BraveRequestHandler* handler)
    : WebRequestProxyingWebSocket(std::move(factory),
                                  request,
                                  std::move(handshake_client),
                                  has_extra_headers,
                                  process_id,
                                  render_frame_id,
                                  browser_context,
                                  request_id_generator,
                                  proxies),
      frame_tree_node_id_(frame_tree_node_id),
      request_handler_(handler),
      request_(request) {}

BraveWebRequestProxyingWebSocket::~BraveWebRequestProxyingWebSocket()
{
  if (ctx_) {
    request_handler_->OnURLRequestDestroyed(ctx_);
  }
}

void BraveWebRequestProxyingWebSocket::Start() {
  auto continuation = base::BindRepeating(
      &BraveWebRequestProxyingWebSocket::ContinueToBase_Start,
      weak_factory_.GetWeakPtr());

  ctx_ = std::make_shared<brave::BraveRequestInfo>();
  brave::BraveRequestInfo::FillCTX(request_, info_.render_process_id,
                                   frame_tree_node_id_, info_.id,
                                   browser_context_, ctx_);
  int result =
      request_handler_->OnBeforeURLRequest(ctx_, continuation, &redirect_url_);

  if (result == net::ERR_BLOCKED_BY_CLIENT) {
    OnError(result);
    return;
  }

  if (result == net::ERR_IO_PENDING) {
    return;
  }

  DCHECK_EQ(net::OK, result);
  ContinueToBase_Start(result);
}

void BraveWebRequestProxyingWebSocket::ContinueToBase_Start(int error_code) {
  if (error_code == net::ERR_BLOCKED_BY_CLIENT) {
    OnError(error_code);
    return;
  }

  if (error_code == net::ERR_IO_PENDING) {
    return;
  }

  DCHECK_EQ(net::OK, error_code);
  WebRequestProxyingWebSocket::Start();
}

void BraveWebRequestProxyingWebSocket::ContinueToHeadersReceived() {
  base::RepeatingCallback<void(int)> continuation;
  continuation =
      base::BindRepeating(&BraveWebRequestProxyingWebSocket::
                              ContinueToBase_OnHeadersReceivedComplete,
                          weak_factory_.GetWeakPtr());
  ctx_ = std::make_shared<brave::BraveRequestInfo>();
  brave::BraveRequestInfo::FillCTX(request_, info_.render_process_id,
                                   frame_tree_node_id_,
                                   info_.id, browser_context_, ctx_);
  int result = request_handler_->OnHeadersReceived(
      ctx_, continuation, response_.headers.get(), &override_headers_,
      &redirect_url_);

  if (result == net::ERR_BLOCKED_BY_CLIENT) {
    OnError(result);
    return;
  }

  PauseIncomingMethodCallProcessing();
  if (result == net::ERR_IO_PENDING)
    return;

  DCHECK_EQ(net::OK, result);
  ContinueToBase_OnHeadersReceivedComplete(result);
}

void BraveWebRequestProxyingWebSocket::ContinueToBase_OnHeadersReceivedComplete(
    int error_code) {
  if (error_code == net::ERR_BLOCKED_BY_CLIENT) {
    OnError(error_code);
    return;
  }

  if (error_code == net::ERR_IO_PENDING) {
    return;
  }

  DCHECK_EQ(net::OK, error_code);
  WebRequestProxyingWebSocket::ContinueToHeadersReceived();
}

void BraveWebRequestProxyingWebSocket::OnBeforeRequestComplete(int error_code) {
  DCHECK(binding_as_header_client_ || !binding_as_handshake_client_.is_bound());
  DCHECK(info_.url.SchemeIsWSOrWSS());
  if (error_code != net::OK) {
    OnError(error_code);
    return;
  }

  auto continuation = base::BindRepeating(
      &BraveWebRequestProxyingWebSocket::ContinueToBase_OnBeforeRequestComplete,
      weak_factory_.GetWeakPtr());

  ctx_ = std::make_shared<brave::BraveRequestInfo>();
  brave::BraveRequestInfo::FillCTX(request_, info_.render_process_id,
                                   frame_tree_node_id_,
                                   info_.id, browser_context_, ctx_);
  int result = request_handler_->OnBeforeStartTransaction(ctx_, continuation,
                                                          &request_.headers);

  if (result == net::ERR_BLOCKED_BY_CLIENT) {
    OnError(result);
    return;
  }

  if (result == net::ERR_IO_PENDING)
    return;

  DCHECK_EQ(net::OK, result);
  ContinueToBase_OnBeforeRequestComplete(error_code);
}

void BraveWebRequestProxyingWebSocket::ContinueToBase_OnBeforeRequestComplete(
  int error_code) {
  WebRequestProxyingWebSocket::OnBeforeRequestComplete(error_code);
}

