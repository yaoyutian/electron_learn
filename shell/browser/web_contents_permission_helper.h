// Copyright (c) 2016 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#ifndef ELECTRON_SHELL_BROWSER_WEB_CONTENTS_PERMISSION_HELPER_H_
#define ELECTRON_SHELL_BROWSER_WEB_CONTENTS_PERMISSION_HELPER_H_

#include "base/values.h"
#include "content/public/browser/media_stream_request.h"
#include "content/public/browser/web_contents_user_data.h"
#include "third_party/blink/public/common/mediastream/media_stream_request.h"
#include "third_party/blink/public/common/permissions/permission_utils.h"

namespace electron {

// Applies the permission requested for WebContents.
class WebContentsPermissionHelper
    : public content::WebContentsUserData<WebContentsPermissionHelper> {
 public:
  ~WebContentsPermissionHelper() override;

  // disable copy
  WebContentsPermissionHelper(const WebContentsPermissionHelper&) = delete;
  WebContentsPermissionHelper& operator=(const WebContentsPermissionHelper&) =
      delete;

  enum class PermissionType {
    POINTER_LOCK = static_cast<int>(blink::PermissionType::NUM) + 1,
    FULLSCREEN,
    OPEN_EXTERNAL,
    SERIAL,
    HID
  };

  // Asynchronous Requests
  void RequestFullscreenPermission(base::OnceCallback<void(bool)> callback);
  void RequestMediaAccessPermission(const content::MediaStreamRequest& request,
                                    content::MediaResponseCallback callback);
  void RequestPointerLockPermission(
      bool user_gesture,
      bool last_unlocked_by_target,
      base::OnceCallback<void(content::WebContents*, bool, bool, bool)>
          callback);
  void RequestWebNotificationPermission(
      base::OnceCallback<void(bool)> callback);
  void RequestOpenExternalPermission(base::OnceCallback<void(bool)> callback,
                                     bool user_gesture,
                                     const GURL& url);

  // Synchronous Checks
  bool CheckMediaAccessPermission(const GURL& security_origin,
                                  blink::mojom::MediaStreamType type) const;
  bool CheckSerialAccessPermission(const url::Origin& embedding_origin) const;
  bool CheckSerialPortPermission(
      const url::Origin& origin,
      base::Value device,
      content::RenderFrameHost* render_frame_host) const;
  void GrantSerialPortPermission(
      const url::Origin& origin,
      base::Value device,
      content::RenderFrameHost* render_frame_host) const;
  bool CheckHIDAccessPermission(const url::Origin& embedding_origin) const;
  bool CheckHIDDevicePermission(
      const url::Origin& origin,
      base::Value device,
      content::RenderFrameHost* render_frame_host) const;
  void GrantHIDDevicePermission(
      const url::Origin& origin,
      base::Value device,
      content::RenderFrameHost* render_frame_host) const;

 private:
  explicit WebContentsPermissionHelper(content::WebContents* web_contents);
  friend class content::WebContentsUserData<WebContentsPermissionHelper>;

  void RequestPermission(blink::PermissionType permission,
                         base::OnceCallback<void(bool)> callback,
                         bool user_gesture = false,
                         const base::DictionaryValue* details = nullptr);

  bool CheckPermission(blink::PermissionType permission,
                       const base::DictionaryValue* details) const;

  bool CheckDevicePermission(blink::PermissionType permission,
                             const url::Origin& origin,
                             const base::Value* device,
                             content::RenderFrameHost* render_frame_host) const;

  void GrantDevicePermission(blink::PermissionType permission,
                             const url::Origin& origin,
                             const base::Value* device,
                             content::RenderFrameHost* render_frame_host) const;

  // TODO(clavin): refactor to use the WebContents provided by the
  // WebContentsUserData base class instead of storing a duplicate ref
  content::WebContents* web_contents_;

  WEB_CONTENTS_USER_DATA_KEY_DECL();
};

}  // namespace electron

#endif  // ELECTRON_SHELL_BROWSER_WEB_CONTENTS_PERMISSION_HELPER_H_
