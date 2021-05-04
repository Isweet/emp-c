#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

struct protocol;
typedef struct protocol protocol_t;

protocol_t *sh_create(const char *address, int port, int party);
protocol_t *plain_create();
void protocol_destroy(protocol_t *p);

struct bit;
typedef struct bit bit_t;
bit_t *bit_create(protocol_t *p, bool b, int party);
bit_t *bit_not(protocol_t *p, bit_t *v);
bit_t *bit_and(protocol_t *p, bit_t *lhsc, bit_t *rhsc);
bit_t *bit_or(protocol_t *p, bit_t *lhsc, bit_t *rhsc);
bit_t *bit_xor(protocol_t *p, bit_t *lhsc, bit_t *rhsc);
bit_t *bit_cond(protocol_t *p, bit_t *guard, bit_t *lhsc, bit_t *rhsc);
bool bit_reveal(protocol_t *p, bit_t *vc, int party);
void bit_destroy(bit_t *v);

struct integer;
typedef struct integer integer_t;
integer_t *integer_create(protocol_t *p, int length, int64_t v, int party);
integer_t *integer_add(protocol_t *p, integer_t *lhsc, integer_t *rhsc);
integer_t *integer_sub(protocol_t *p, integer_t *lhsc, integer_t *rhsc);
integer_t *integer_mult(protocol_t *p, integer_t *lhsc, integer_t *rhsc);
integer_t *integer_div(protocol_t *p, integer_t *lhsc, integer_t *rhsc);
integer_t *integer_mod(protocol_t *p, integer_t *lhsc, integer_t *rhsc);
bit_t *integer_eq(protocol_t *p, integer_t *lhsc, integer_t *rhsc);
bit_t *integer_lt(protocol_t *p, integer_t *lhsc, integer_t *rhsc);
bit_t *integer_lte(protocol_t *p, integer_t *lhsc, integer_t *rhsc);
bit_t *integer_gt(protocol_t *p, integer_t *lhsc, integer_t *rhsc);
integer_t *integer_cond(protocol_t *p, bit_t *guard, integer_t *lhsc, integer_t *rhsc);
int64_t integer_reveal(protocol_t *p, integer_t *vc, int party);
void integer_destroy(integer_t *v);

#ifdef __cplusplus
}
#endif
