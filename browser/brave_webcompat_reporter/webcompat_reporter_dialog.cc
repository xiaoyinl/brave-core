/* Copyright 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/browser/brave_webcompat_reporter/webcompat_reporter_dialog.h"

#include <algorithm>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "base/json/json_reader.h"
#include "base/json/json_writer.h"
#include "base/values.h"
#include "brave/common/webui_url_constants.h"
#include "chrome/browser/ui/browser_finder.h"
#include "chrome/browser/ui/browser_window.h"
#include "chrome/browser/ui/webui/chrome_web_contents_handler.h"
#include "chrome/browser/ui/webui/constrained_web_dialog_ui.h"
#include "components/guest_view/browser/guest_view_base.h"
#include "components/web_modal/web_contents_modal_dialog_host.h"
#include "content/public/browser/web_contents.h"
#include "ui/web_dialogs/web_dialog_delegate.h"

using content::WebContents;
using content::WebUIMessageHandler;

constexpr int kDialogMargin = 25;
constexpr int kDialogMinHeight = 100;
constexpr int kDialogMaxHeight = 700;
constexpr int kDialogMaxWidth = 375;

// A ui::WebDialogDelegate that specifies the webcompat reporter's appearance.
class WebcompatReporterDialogDelegate : public ui::WebDialogDelegate {
 public:
  explicit WebcompatReporterDialogDelegate(WebContents* initiator,
      std::unique_ptr<base::DictionaryValue> params);
  ~WebcompatReporterDialogDelegate() override;

  ui::ModalType GetDialogModalType() const override;
  base::string16 GetDialogTitle() const override;
  GURL GetDialogContentURL() const override;
  void GetWebUIMessageHandlers(
      std::vector<WebUIMessageHandler*>* handlers) const override;
  void GetDialogSize(gfx::Size* size) const override;
  std::string GetDialogArgs() const override;
  void OnDialogClosed(const std::string& json_retval) override;
  void OnCloseContents(WebContents* source, bool* out_close_dialog) override;
  bool ShouldShowDialogTitle() const override;

 private:
  WebContents* initiator_;
  std::unique_ptr<base::DictionaryValue> params_;

  DISALLOW_COPY_AND_ASSIGN(WebcompatReporterDialogDelegate);
};

WebcompatReporterDialogDelegate::WebcompatReporterDialogDelegate(WebContents* initiator,
    std::unique_ptr<base::DictionaryValue> params)
  : initiator_(initiator), params_(std::move(params)) {
}

WebcompatReporterDialogDelegate::~WebcompatReporterDialogDelegate() {
}

ui::ModalType WebcompatReporterDialogDelegate::GetDialogModalType() const {
  // Not used, returning dummy value.
  NOTREACHED();
  return ui::MODAL_TYPE_WINDOW;
}

base::string16 WebcompatReporterDialogDelegate::GetDialogTitle() const {
  // Only used on Windows?
  return base::string16();
}

GURL WebcompatReporterDialogDelegate::GetDialogContentURL() const {
  return GURL(kBraveUIWebcompatReporterURL);
}

void WebcompatReporterDialogDelegate::GetWebUIMessageHandlers(
    std::vector<WebUIMessageHandler*>* /* handlers */) const {
  // WebcompatReporterWebUI should add its own message handlers.
}

void WebcompatReporterDialogDelegate::GetDialogSize(gfx::Size* size) const {
  DCHECK(size);

  gfx::Size target_size;

  web_modal::WebContentsModalDialogHost* host = nullptr;
  WebContents* outermost_web_contents =
      guest_view::GuestViewBase::GetTopLevelWebContents(initiator_);

  Browser* browser = chrome::FindBrowserWithWebContents(outermost_web_contents);
  if (browser)
    host = browser->window()->GetWebContentsModalDialogHost();

  if (host)
    target_size = host->GetMaximumDialogSize();
  else
    target_size = outermost_web_contents->GetContainerBounds().size();
  // initial size in between min and max
  const int max_height =
      kDialogMinHeight + (kDialogMaxHeight - kDialogMinHeight);
  int width = std::min(target_size.width() - kDialogMargin, kDialogMaxWidth);
  size->SetSize(width, max_height);
}

std::string WebcompatReporterDialogDelegate::GetDialogArgs() const {
  std::string json;
  base::JSONWriter::Write(*params_, &json);
  return json;
}

void WebcompatReporterDialogDelegate::OnDialogClosed(
    const std::string& /* json_retval */) {
}

void WebcompatReporterDialogDelegate::OnCloseContents(WebContents* /* source */,
                                                 bool* out_close_dialog) {
  *out_close_dialog = true;
}

bool WebcompatReporterDialogDelegate::ShouldShowDialogTitle() const {
  return false;
}

void OpenWebcompatReporterDialog(content::WebContents* initiator) {
  GURL site_url = initiator->GetLastCommittedURL();
  auto params_dict = std::make_unique<base::DictionaryValue>();
  params_dict->SetString("siteUrl", site_url.GetOrigin().spec());

  gfx::Size host_size = initiator->GetContainerBounds().size();
  int width = std::min(host_size.width() - kDialogMargin, kDialogMaxWidth);
  gfx::Size min_size(width, kDialogMinHeight);
  gfx::Size max_size(width, kDialogMaxHeight);
  ShowConstrainedWebDialogWithAutoResize(
      initiator->GetBrowserContext(),
      std::make_unique<WebcompatReporterDialogDelegate>(initiator, std::move(params_dict)),
      initiator, min_size, max_size);
}
