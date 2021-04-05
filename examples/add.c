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
  netio_t *io = netio_create(party == alice ? NULL : "127.0.0.1", port);
  setup_semi_honest_c(io, party);
  integer_t *v1;
  integer_t *v2;
  if (party == alice) {
    v1 = integer_create(64, input, alice);
    v2 = integer_create(64, 0, bob);
  }
  if (party == bob) {
    v1 = integer_create(64, 0, alice);
    v2 = integer_create(64, input, bob);
  }
  integer_t *sum = integer_add(v1, v2);
  printf("Sum is %d\n", integer_reveal(sum, public));
  integer_destroy(v1);
  integer_destroy(v2);
  integer_destroy(sum);
  netio_destroy(io);
  return 0;
}
