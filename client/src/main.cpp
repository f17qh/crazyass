#include "main.h"
#include "app_delegate.h"
#include "CCEGLView.h"

USING_NS_CC;

#if 0
static int AppRun() {
  // create the application instance
  AppDelegate app;
  CCEGLView* eglView = CCEGLView::sharedOpenGLView();
  eglView->setViewName("CrazyAss");
  eglView->setFrameSize(900, 580);

  return CCApplication::sharedApplication()->run();
}

#ifdef WIN32
int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // create the application instance
    return AppRun();
}
#else
int main(int argc, char **argv) {
  return AppRun();
}
#endif
#endif
