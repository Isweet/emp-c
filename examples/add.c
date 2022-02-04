#include <stdlib.h>
#include <stdio.h>
#include "empc.h"

int public = 0;
int alice = 1;
int bob = 2;

int main(int argc, char **argv) {
    // run computation with semi-honest model
  int party = atoi(argv[1]);
  int input = atoi(argv[2]);
  int port = 12345;
  netio_t *io = netio_create(party == alice ? NULL : "127.0.0.1", port, true);
  emp_ctx_t *p = emp_sh_create(io, party);
  emp_integer_t *v1;
  emp_integer_t *v2;
  if (party == alice) {
    v1 = emp_integer_create(p, 64, input, alice);
    v2 = emp_integer_create(p, 64, 0, bob);
  }
  if (party == bob) {
    v1 = emp_integer_create(p, 64, 0, alice);
    v2 = emp_integer_create(p, 64, input, bob);
  }
  emp_integer_t *sum = emp_integer_add(p, v1, v2);
  printf("Sum is %lld\n", emp_integer_reveal(p, sum, public));
  emp_integer_destroy(v1);
  emp_integer_destroy(v2);
  emp_integer_destroy(sum);
  netio_destroy(io);
  emp_destroy(p);
  return 0;
}
