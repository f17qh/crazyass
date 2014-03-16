#include "main.h"
#include "AppDelegate.h"
#include "CCEGLView.h"
#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

USING_NS_CC;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // create the application instance
    AppDelegate app;
    CCEGLView* eglView = CCEGLView::sharedOpenGLView();
    eglView->setViewName("crazyass");
    eglView->setFrameSize(800, 480);
    return CCApplication::sharedApplication()->run();
}

void CAWriteFile(char *file, char *content) {
  int fd = _open(file, _O_RDWR | _O_CREAT, _S_IREAD | _S_IWRITE);
   _write(fd, content, strlen(content));
  _close(fd);
}

void CAReadFile(char *file, char *content, size_t len) {
  int fd = _open(file, _O_RDWR | _O_CREAT, _S_IREAD | _S_IWRITE);
   _read(fd, content, len);
  _close(fd);
}

void * ProductList(){return NULL;};
bool ProductBuy(void *iap, char *, void *){return false;};
