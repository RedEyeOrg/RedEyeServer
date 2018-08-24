#include <stdio.h>
#include <sqlite3.h>

int sqlite_init() {
  printf("%s\n", sqlite3_libversion());
}
