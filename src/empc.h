#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

  // NetIO
  struct netio;
  typedef struct netio netio_t;
  netio_t *netio_create(const char *address, int port, bool quiet);
  void netio_send(netio_t *io, void *data, int nbyte);
  void netio_recv(netio_t *io, void *data, int nbyte);
  void netio_flush(netio_t *io);
  void netio_destroy(netio_t *io);

  // EMP
  struct emp_ctx;
  typedef struct emp_ctx emp_ctx_t;

  emp_ctx_t *emp_sh_create(netio_t *io, int party);
  emp_ctx_t *emp_plain_create();
  void emp_destroy(emp_ctx_t *p);

  typedef struct emp_bit {
    int obj[4];
  } emp_bit_t;

  emp_bit_t emp_bit_create_s(emp_ctx_t *p, bool b, int party);
  emp_bit_t emp_bit_not_s(emp_ctx_t *p, emp_bit_t *v);
  emp_bit_t emp_bit_and_s(emp_ctx_t *p, emp_bit_t *lhsc, emp_bit_t *rhsc);
  emp_bit_t emp_bit_or_s(emp_ctx_t *p, emp_bit_t *lhsc, emp_bit_t *rhsc);
  emp_bit_t emp_bit_xor_s(emp_ctx_t *p, emp_bit_t *lhsc, emp_bit_t *rhsc);
  emp_bit_t emp_bit_cond_s(emp_ctx_t *p, emp_bit_t *guard, emp_bit_t *lhsc, emp_bit_t *rhsc);

  emp_bit_t *emp_bit_create(emp_ctx_t *p, bool b, int party);
  emp_bit_t *emp_bit_not(emp_ctx_t *p, emp_bit_t *v);
  emp_bit_t *emp_bit_and(emp_ctx_t *p, emp_bit_t *lhsc, emp_bit_t *rhsc);
  emp_bit_t *emp_bit_or(emp_ctx_t *p, emp_bit_t *lhsc, emp_bit_t *rhsc);
  emp_bit_t *emp_bit_xor(emp_ctx_t *p, emp_bit_t *lhsc, emp_bit_t *rhsc);
  emp_bit_t *emp_bit_cond(emp_ctx_t *p, emp_bit_t *guard, emp_bit_t *lhsc, emp_bit_t *rhsc);
  bool emp_bit_reveal(emp_ctx_t *p, emp_bit_t *vc, int party);
  void emp_bit_destroy(emp_bit_t *v);

  struct emp_integer;
  typedef struct emp_integer emp_integer_t;
  emp_integer_t *emp_integer_create(emp_ctx_t *p, int length, int64_t v, int party);
  emp_integer_t *emp_integer_add(emp_ctx_t *p, emp_integer_t *lhsc, emp_integer_t *rhsc);
  emp_integer_t *emp_integer_sub(emp_ctx_t *p, emp_integer_t *lhsc, emp_integer_t *rhsc);
  emp_integer_t *emp_integer_mult(emp_ctx_t *p, emp_integer_t *lhsc, emp_integer_t *rhsc);
  emp_integer_t *emp_integer_div(emp_ctx_t *p, emp_integer_t *lhsc, emp_integer_t *rhsc);
  emp_integer_t *emp_integer_mod(emp_ctx_t *p, emp_integer_t *lhsc, emp_integer_t *rhsc);
  emp_bit_t *emp_integer_eq(emp_ctx_t *p, emp_integer_t *lhsc, emp_integer_t *rhsc);
  emp_bit_t *emp_integer_lt(emp_ctx_t *p, emp_integer_t *lhsc, emp_integer_t *rhsc);
  emp_bit_t *emp_integer_lte(emp_ctx_t *p, emp_integer_t *lhsc, emp_integer_t *rhsc);
  emp_bit_t *emp_integer_gt(emp_ctx_t *p, emp_integer_t *lhsc, emp_integer_t *rhsc);
  emp_integer_t *emp_integer_cond(emp_ctx_t *p, emp_bit_t *guard, emp_integer_t *lhsc, emp_integer_t *rhsc);
  int64_t emp_integer_reveal(emp_ctx_t *p, emp_integer_t *vc, int party);
  void emp_integer_destroy(emp_integer_t *v);

  // SPDZ
  struct spdz_ctx;
  typedef struct spdz_ctx spdz_ctx_t;

  spdz_ctx_t *spdz_create(int party, int nparties);
  void spdz_destroy(spdz_ctx_t *p);

  struct spdz_int64;
  typedef struct spdz_int64 spdz_int64_t;
  spdz_int64_t *spdz_int64_create(spdz_ctx_t *p, int party, int64_t v);
  spdz_int64_t *spdz_int64_add(spdz_ctx_t *p, spdz_int64_t *lhs, spdz_int64_t *rhs);
  spdz_int64_t *spdz_int64_mult(spdz_ctx_t *p, spdz_int64_t *lhs, spdz_int64_t *rhs);
  int64_t spdz_int64_reveal(spdz_ctx_t *p, int party, spdz_int64_t *sh);
  void spdz_int64_destroy(spdz_int64_t *sh);

#ifdef __cplusplus
}
#endif
