#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

struct netio;
typedef struct netio netio_t;
netio_t *netio_create(const char *address, int port);
void netio_send_data(netio_t *io, void *data, int nbyte);
void netio_recv_data(netio_t *io, void *data, int nbyte);
void netio_destroy(netio_t *io);

struct semihonest;
typedef struct semihonest semihonest_t;
semihonest_t *setup_semi_honest_c(netio_t *io, int party);
void finalize_semi_honest_c(semihonest_t *sh);

struct bit;
typedef struct bit bit_t;
bit_t *bit_create(semihonest_t *sh, bool b, int party);
bit_t *bit_not(semihonest_t *sh, bit_t *v);
bit_t *bit_and(semihonest_t *sh, bit_t *lhsc, bit_t *rhsc);
bit_t *bit_or(semihonest_t *sh, bit_t *lhsc, bit_t *rhsc);
bit_t *bit_xor(semihonest_t *sh, bit_t *lhsc, bit_t *rhsc);
bit_t *bit_cond(semihonest_t *sh, bit_t *guard, bit_t *lhsc, bit_t *rhsc);
bool bit_reveal(semihonest_t *sh, bit_t *vc, int party);
void bit_destroy(bit_t *v);

struct integer;
typedef struct integer integer_t;
integer_t *integer_create(semihonest_t *sh, int length, int64_t v, int party);
integer_t *integer_add(semihonest_t *sh, integer_t *lhsc, integer_t *rhsc);
integer_t *integer_sub(semihonest_t *sh, integer_t *lhsc, integer_t *rhsc);
integer_t *integer_mult(semihonest_t *sh, integer_t *lhsc, integer_t *rhsc);
integer_t *integer_div(semihonest_t *sh, integer_t *lhsc, integer_t *rhsc);
integer_t *integer_mod(semihonest_t *sh, integer_t *lhsc, integer_t *rhsc);
bit_t *integer_eq(semihonest_t *sh, integer_t *lhsc, integer_t *rhsc);
bit_t *integer_lt(semihonest_t *sh, integer_t *lhsc, integer_t *rhsc);
bit_t *integer_gt(semihonest_t *sh, integer_t *lhsc, integer_t *rhsc);
integer_t *integer_cond(semihonest_t *sh, bit_t *guard, integer_t *lhsc, integer_t *rhsc);
int64_t integer_reveal(semihonest_t *sh, integer_t *vc, int party);
void integer_destroy(integer_t *v);

#ifdef __cplusplus
}
#endif
