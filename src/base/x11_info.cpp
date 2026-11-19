// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by Commercial License that can be found
// in the LICENSE file.

#include "base/x11_info.h"

#include <X11/Xlib.h>

int X11Info::appScreen() {
  Display *xd = display();
  return xd ? DefaultScreen(xd) : 0;
}

unsigned long X11Info::appRootWindow(int screen) {
  Display *xd = display();
  if (!xd) {
    return 0;
  }
  return XRootWindow(xd, screen < 0 ? DefaultScreen(xd) : screen);
}
