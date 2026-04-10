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

#define log(f, ...)                                                            \
  do {                                                                         \
    fprintf(stderr, "%s", KMAG);                                               \
    fprintf(stderr, f, ##__VA_ARGS__);                                         \
    fprintf(stderr, "\n%s", KNRM);                                             \
  } while (0)

#define info(f, ...)                                                            \
  do {                                                                         \
    fprintf(stderr, "%s", KGRN);                                               \
    fprintf(stderr, f, ##__VA_ARGS__);                                         \
    fprintf(stderr, "\n%s", KNRM);                                             \
  } while (0)
