#ifndef MING_PRINT_H
#define MING_PRINT_H

#define KNRM "\x1B[0m"
#define KRED "\x1B[31m"
#define KGRN "\x1B[32m"
#define KYEL "\x1B[33m"
#define KBLU "\x1B[34m"
#define KMAG "\x1B[35m"
#define KCYN "\x1B[36m"

#define bail(f, ...)                                                           \
  do {                                                                         \
    fprintf(stderr, "%sError: ", KRED);                                        \
    fprintf(stderr, f, ##__VA_ARGS__);                                         \
    fprintf(stderr, "\n%s", KNRM);                                             \
    exit(1);                                                                   \
  } while (0)

#define debug(f, ...)                                                            \
  do {                                                                         \
    fprintf(stdout, "%s", KCYN);                                               \
    fprintf(stdout, f, ##__VA_ARGS__);                                         \
    fprintf(stdout, "\n%s", KNRM);                                             \
  } while (0)

#define info(f, ...)                                                            \
  do {                                                                         \
    fprintf(stdout, "%s", KGRN);                                               \
    fprintf(stdout, f, ##__VA_ARGS__);                                         \
    fprintf(stdout, "\n%s", KNRM);                                             \
  } while (0)

#define warn(f, ...)                                                            \
  do {                                                                         \
    fprintf(stderr, "%s", KYEL);                                               \
    fprintf(stderr, f, ##__VA_ARGS__);                                         \
    fprintf(stderr, "\n%s", KNRM);                                             \
  } while (0)

#define error(f, ...)                                                            \
  do {                                                                         \
    fprintf(stderr, "%s", KRED);                                               \
    fprintf(stderr, f, ##__VA_ARGS__);                                         \
    fprintf(stderr, "\n%s", KNRM);                                             \
  } while (0)

#endif
