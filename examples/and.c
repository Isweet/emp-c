#include <stdlib.h>
#include <stdio.h>
#include "empc.h"

int public = 0;
int alice = 1;
int bob = 2;

int main(int argc, char **argv) {
    // run computation with semi-honest model
  int party = atoi(argv[1]);
  bool input = atoi(argv[2]) ? true : false;
  int port = 12345;
  protocol_t *p = sh_create(party == alice ? NULL : "127.0.0.1", port, party);
  bit_t v1;
  bit_t v2;
  if (party == alice) {
    v1 = bit_create_s(p, input, alice);
    v2 = bit_create_s(p, false, bob);
  }
  if (party == bob) {
    v1 = bit_create_s(p, false, alice);
    v2 = bit_create_s(p, input, bob);
  }
  bit_t sum = bit_and_s(p, &v1, &v2);
  printf("Result: %s\n", bit_reveal(p, &sum, public) ? "true" : "false");
  protocol_destroy(p);
  return 0;
}
