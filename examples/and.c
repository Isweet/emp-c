#include <stdlib.h>
#include <stdio.h>
#include "symphony.h"

int8_t public = 0;
int8_t alice = 1;
int8_t bob = 2;

int main(int argc, char **argv) {
    // run computation with semi-honest model
  int8_t party = atoi(argv[1]);
  bool input = atoi(argv[2]) ? true : false;
  int port = 12345;
  netio_t *io = netio_create(party == alice ? NULL : "127.0.0.1", port, true);
  emp_semi_ctx_t *p = emp_semi_ctx_create(party, io);
  emp_semi_bit_t v1;
  emp_semi_bit_t v2;
  if (party == alice) {
    v1 = emp_semi_bit_create_stack(p, alice, input);
    v2 = emp_semi_bit_create_stack(p, bob, false);
  }
  if (party == bob) {
    v1 = emp_semi_bit_create_stack(p, alice, false);
    v2 = emp_semi_bit_create_stack(p, bob, input);
  }
  emp_semi_bit_t sum = emp_semi_bit_and_stack(p, &v1, &v2);
  printf("Result: %s\n", emp_semi_bit_reveal(p, public, &sum) ? "true" : "false");
  netio_destroy(io);
  emp_semi_ctx_destroy(p);

  return 0;
}
