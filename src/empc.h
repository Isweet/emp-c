#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

struct netio;
typedef struct netio netio_t;
netio_t *netio_create(const char *address, int port);
void netio_destroy(netio_t *io);
void setup_semi_honest_c(netio_t *io, int party);

struct bit;
typedef struct bit bit_t;
bit_t *bit_create(bool b, int party);
void bit_destroy(bit_t *v);

struct integer;
typedef struct integer integer_t;
integer_t *integer_create(int length, int64_t v, int party);
integer_t *integer_add(integer_t *lhsc, integer_t *rhsc);
int64_t integer_reveal(integer_t *vc, int party);
void integer_destroy(integer_t *v);

#ifdef __cplusplus
}
#endif
