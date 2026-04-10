#include <X11/Xlib.h>
#include <cstdio>
#include <cstdlib>

#define bail(f, ...)                                                           \
  do {                                                                         \
    fprintf(stderr, f, ##__VA_ARGS__);                                         \
    fprintf(stderr, "\n");                                                     \
    exit(1);                                                                   \
  } while (0)

int main(int argc, char *argv[]) {

  Display *display = XOpenDisplay(0);
  if (display == NULL) {
    bail("failed to connect to X display");
  }

  return 0;
}
