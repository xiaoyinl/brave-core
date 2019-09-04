/* Copyright 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_EXTENSIONS_BROWSER_API_WEB_REQUEST_BRAVE_WEB_REQUEST_PROXYING_WEB_SOCKET_H_
#define BRAVE_EXTENSIONS_BROWSER_API_WEB_REQUEST_BRAVE_WEB_REQUEST_PROXYING_WEB_SOCKET_H_

#include "extensions/browser/api/web_request/web_request_api.h"
#include "extensions/browser/api/web_request/web_request_proxying_websocket.h"
#include "services/network/public/cpp/resource_request.h"

class BraveRequestHandler;
namespace brave {
struct BraveRequestInfo;
}  // namespace brave

class BraveWebRequestProxyingWebSocket
    : public extensions::WebRequestProxyingWebSocket {
 public:
  using WebSocketFactory =
      extensions::WebRequestProxyingWebSocket::WebSocketFactory;

  BraveWebRequestProxyingWebSocket(
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
      BraveRequestHandler* handler);

  ~BraveWebRequestProxyingWebSocket() override;

  static void StartProxying(
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
      extensions::WebRequestAPI::ProxySet* proxies);

  // Note that the base's Start method is not virtual, but it's ok since it's
  // called explicitly from StartProxying of this class.
  void Start();

 private:
  void OnBeforeRequestComplete(int error_code) override;
  void ContinueToHeadersReceived() override;

  // Continuations that call into base methods.
  void ContinueToBase_Start(int error_code);
  void ContinueToBase_OnHeadersReceivedComplete(int error_code);
  void ContinueToBase_OnBeforeRequestComplete(int error_code);

  const int frame_tree_node_id_;
  BraveRequestHandler* request_handler_;
  network::ResourceRequest request_;
  // TODO(iefremov): Get rid of shared_ptr, we should clearly own the pointer.
  // TODO(iefremov): Init this only once.
  std::shared_ptr<brave::BraveRequestInfo> ctx_;

  base::WeakPtrFactory<BraveWebRequestProxyingWebSocket> weak_factory_{this};
  DISALLOW_COPY_AND_ASSIGN(BraveWebRequestProxyingWebSocket);
};

#endif  // BRAVE_EXTENSIONS_BROWSER_API_WEB_REQUEST_BRAVE_WEB_REQUEST_PROXYING_WEB_SOCKET_H_
