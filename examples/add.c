#include <stdlib.h>
#include <stdio.h>
#include "symphony.h"

int8_t public = 0;
int8_t alice = 1;
int8_t bob = 2;

int main(int argc, char **argv) {
    // run computation with semi-honest model
  int8_t party = atoi(argv[1]);
  int64_t input = atoi(argv[2]);
  int port = 12345;
  netio_t *io = netio_create(party == alice ? NULL : "127.0.0.1", port, true);
  emp_semi_ctx_t *p = emp_semi_ctx_create(party, io);
  emp_semi_integer_t *v1;
  emp_semi_integer_t *v2;
  if (party == alice) {
    v1 = emp_semi_integer_create(p, alice, 64, input);
    v2 = emp_semi_integer_create(p, bob, 64, 0);
  }
  if (party == bob) {
    v1 = emp_semi_integer_create(p, alice, 64, 0);
    v2 = emp_semi_integer_create(p, bob, 64, input);
  }
  emp_semi_integer_t *sum = emp_semi_integer_add(p, v1, v2);
  printf("Sum is %lld\n", emp_semi_integer_reveal(p, public, sum));
  emp_semi_integer_destroy(v1);
  emp_semi_integer_destroy(v2);
  emp_semi_integer_destroy(sum);
  netio_destroy(io);
  emp_semi_ctx_destroy(p);

  return 0;
}
