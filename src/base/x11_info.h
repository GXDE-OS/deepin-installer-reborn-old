// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by Commercial License that can be found
// in the LICENSE file.

#ifndef INSTALLER_BASE_X11_INFO_H
#define INSTALLER_BASE_X11_INFO_H

#include <X11/Xlib.h>

#include <QGuiApplication>
#include <QtGui/qguiapplication_platform.h>

// Minimal replacement for QX11Info from Qt5X11Extras, which was removed in
// Qt 6. Values are obtained through the QNativeInterface::QX11Application
// platform integration of the running QGuiApplication, so they are only
// valid while the app runs on the X11 (xcb) platform.
class X11Info {
 public:
  static Display *display() {
    const auto *x11 =
        qGuiApp->nativeInterface<QNativeInterface::QX11Application>();
    return x11 ? x11->display() : nullptr;
  }

  static int appScreen() {
    Display *xd = display();
    return xd ? DefaultScreen(xd) : 0;
  }

  static Window appRootWindow(int screen = -1) {
    Display *xd = display();
    if (!xd) {
      return 0;
    }
    return XRootWindow(xd, screen < 0 ? DefaultScreen(xd) : screen);
  }

  static Time appTime() {
    // Qt5 tracked the timestamp of the last user event here. All callers in
    // this codebase only need a valid timestamp for XUngrabPointer(), for
    // which CurrentTime is accepted.
    return CurrentTime;
  }
};

#endif  // INSTALLER_BASE_X11_INFO_H
