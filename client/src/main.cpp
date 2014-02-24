#include "main.h"
#include "AppDelegate.h"
#include "CCEGLView.h"

USING_NS_CC;

static int AppRun() {
  // create the application instance
  AppDelegate app;
  CCEGLView* eglView = CCEGLView::sharedOpenGLView();
  eglView->setViewName("DogRun2");
  eglView->setFrameSize(800, 480);
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
