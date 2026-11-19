// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by Commercial License that can be found
// in the LICENSE file.

#ifndef INSTALLER_BASE_X11_INFO_H
#define INSTALLER_BASE_X11_INFO_H

#include <QGuiApplication>
#include <QtGui/qguiapplication_platform.h>

// Same forward declaration as in X11/Xlib.h, so that this header never
// includes X11 headers: Xlib defines macros (Bool, None, Status, ...) that
// clash with Qt enums and wreck the Qt headers included afterwards.
typedef struct _XDisplay Display;

// Minimal replacement for QX11Info from Qt5X11Extras, which was removed in
// Qt 6. Values are obtained through the QNativeInterface::QX11Application
// platform integration of the running QGuiApplication, so they are only
// valid while the app runs on the X11 (xcb) platform. Mirrors the API and
// include hygiene of qx11info_x11.h from Qt5X11Extras.
class X11Info {
 public:
  static Display *display() {
    const auto *x11 =
        qGuiApp->nativeInterface<QNativeInterface::QX11Application>();
    return x11 ? x11->display() : nullptr;
  }

  static int appScreen();

  // Returns the root window of |screen|, or of the default screen when
  // |screen| is negative. On Linux, Window and Time are both aliases of
  // unsigned long, so callers can assign the result directly.
  static unsigned long appRootWindow(int screen = -1);

  // Qt5 tracked the timestamp of the last user event here. All callers in
  // this codebase only need a valid timestamp for XUngrabPointer(), for
  // which X11 CurrentTime (0) is accepted.
  static unsigned long appTime() { return 0UL; }
};

#endif  // INSTALLER_BASE_X11_INFO_H
