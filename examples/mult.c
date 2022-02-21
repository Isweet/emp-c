#include <stdlib.h>
#include <stdio.h>
#include "symphony.h"

uint16_t alice = 0;
uint16_t bob = 1;

int main(int argc, char **argv) {
  /*
  int party = atoi(argv[1]);
  int64_t input = atoi(argv[2]);
  int port  = 9999;
  spdz_ctx_t *p = spdz_create(party, 2);
  spdz_int64_t *v1 = spdz_int64_create(p, alice, input);
  spdz_int64_t *v2 = spdz_int64_create(p, bob, input);
  spdz_int64_t *sum = spdz_int64_add(p, v1, v2);
  spdz_int64_t *prod = spdz_int64_mult(p, v1, v2);
  printf("Sum is %lld\n", spdz_int64_reveal(p, -1, sum));
  printf("Prod is %lld\n", spdz_int64_reveal(p, -1, prod));
  spdz_int64_destroy(v1);
  spdz_int64_destroy(v2);
  spdz_int64_destroy(sum);
  spdz_int64_destroy(prod);
  spdz_destroy(p);
  */
  return 0;
}
