#include "empc.h"
#include "emp-sh2pc/emp-sh2pc.h"
#include <immintrin.h>

using namespace emp;

struct netio {
  void *obj;
};

netio_t *netio_create(const char *address, int port, bool quiet) {
  netio_t *io;

  io      = (netio_t *) malloc(sizeof(netio_t));
  io->obj = new NetIO(address, port, quiet);

  return io;
}

void netio_send(netio_t *io, void *data, int nbyte) {
  NetIO *net = static_cast<NetIO *>(io->obj);
  net->send_data_internal(data, nbyte);
}

void netio_recv(netio_t *io, void *data, int nbyte) {
  NetIO *net = static_cast<NetIO *>(io->obj);
  net->recv_data_internal(data, nbyte);
}

void netio_flush(netio_t *io) {
  NetIO *net = static_cast<NetIO *>(io->obj);
  net->flush();
}

void netio_destroy(netio_t *io) {
  if (io == NULL) {
    return;
  }

  delete static_cast<NetIO *>(io->obj);
  free(io);
}

struct emp_ctx {
  int tag;
  void *circ;
  void *prot;
};

emp_ctx_t *emp_sh_create(netio_t *io, int party) {
  NetIO *net = static_cast<NetIO *>(io->obj);

  emp_ctx_t *p;

  p = (emp_ctx_t *) malloc(sizeof(emp_ctx_t));
  p->tag  = 0;
  setup_semi_honest(net, party);
  p->circ = CircuitExecution::circ_exec;
  p->prot = ProtocolExecution::prot_exec;

  return p;
}

emp_ctx_t *emp_plain_create() {
  emp_ctx_t *p;

  p = (emp_ctx_t *) malloc(sizeof(emp_ctx_t));
  p->tag = 1;
  setup_plain_prot(false, "/dev/null");
  p->circ = CircuitExecution::circ_exec;
  p->prot = ProtocolExecution::prot_exec;

  return p;
}

void emp_install(emp_ctx_t *p) {
  CircuitExecution::circ_exec = static_cast<CircuitExecution *>(p->circ);
  ProtocolExecution::prot_exec = static_cast<ProtocolExecution *>(p->prot);
  return;
}

void emp_destroy(emp_ctx_t *p) {
  emp_install(p);

  if (p->tag == 0) {
    finalize_semi_honest();
  } else if (p->tag == 1) {
    finalize_plain_prot();
  } else {
    assert(false);
  }

  free(p);
}

emp_bit_t emp_bit_create_s(emp_ctx_t *p, bool b, int party) {
  emp_install(p);

  Bit tmp(b, party);

  emp_bit_t v;
  _mm_storeu_si128((__m128i *) v.obj, tmp.bit);

  return v;
}

emp_bit_t emp_bit_not_s(emp_ctx_t *p, emp_bit_t *bc) {
  emp_install(p);

  Bit b(_mm_loadu_si128((__m128i *) bc->obj));

  emp_bit_t v;
  _mm_storeu_si128((__m128i *) v.obj, (!b).bit);

  return v;
}

emp_bit_t emp_bit_and_s(emp_ctx_t *p, emp_bit_t *lhsc, emp_bit_t *rhsc) {
  emp_install(p);

  Bit lhs(_mm_loadu_si128((__m128i *) lhsc->obj));
  Bit rhs(_mm_loadu_si128((__m128i *) rhsc->obj));

  emp_bit_t v;
  _mm_storeu_si128((__m128i *) v.obj, (lhs & rhs).bit);

  return v;
}

emp_bit_t emp_bit_or_s(emp_ctx_t *p, emp_bit_t *lhsc, emp_bit_t *rhsc) {
  emp_install(p);

  Bit lhs(_mm_loadu_si128((__m128i *) lhsc->obj));
  Bit rhs(_mm_loadu_si128((__m128i *) rhsc->obj));

  emp_bit_t v;
  _mm_storeu_si128((__m128i *) v.obj, (lhs | rhs).bit);

  return v;
}

emp_bit_t emp_bit_xor_s(emp_ctx_t *p, emp_bit_t *lhsc, emp_bit_t *rhsc) {
  emp_install(p);

  Bit lhs(_mm_loadu_si128((__m128i *) lhsc->obj));
  Bit rhs(_mm_loadu_si128((__m128i *) rhsc->obj));

  emp_bit_t v;
  _mm_storeu_si128((__m128i *) v.obj, (lhs ^ rhs).bit);

  return v;
}

emp_bit_t emp_bit_cond_s(emp_ctx_t *p, emp_bit_t *guardc, emp_bit_t *lhsc, emp_bit_t *rhsc) {
  emp_install(p);

  Bit guard(_mm_loadu_si128((__m128i *) guardc->obj));
  Bit lhs(_mm_loadu_si128((__m128i *) lhsc->obj));
  Bit rhs(_mm_loadu_si128((__m128i *) rhsc->obj));

  emp_bit_t v;
  _mm_storeu_si128((__m128i *) v.obj, If(guard, lhs, rhs).bit);

  return v;
}

emp_bit_t *emp_bit_create(emp_ctx_t *p, bool b, int party) {
  emp_bit_t *v = (emp_bit_t *) malloc(sizeof(emp_bit_t));
  *v = emp_bit_create_s(p, b, party);

  return v;
}

emp_bit_t *emp_bit_not(emp_ctx_t *p, emp_bit_t *bc) {
  emp_bit_t *v = (emp_bit_t *) malloc(sizeof(emp_bit_t));
  *v = emp_bit_not_s(p, bc);

  return v;
}

emp_bit_t *emp_bit_and(emp_ctx_t *p, emp_bit_t *lhsc, emp_bit_t *rhsc) {
  emp_bit_t *v = (emp_bit_t *) malloc(sizeof(emp_bit_t));
  *v = emp_bit_and_s(p, lhsc, rhsc);

  return v;
}

emp_bit_t *emp_bit_or(emp_ctx_t *p, emp_bit_t *lhsc, emp_bit_t *rhsc) {
  emp_bit_t *v = (emp_bit_t *) malloc(sizeof(emp_bit_t));
  *v = emp_bit_or_s(p, lhsc, rhsc);

  return v;
}

emp_bit_t *emp_bit_xor(emp_ctx_t *p, emp_bit_t *lhsc, emp_bit_t *rhsc) {
  emp_bit_t *v = (emp_bit_t *) malloc(sizeof(emp_bit_t));
  *v = emp_bit_xor_s(p, lhsc, rhsc);

  return v;
}

emp_bit_t *emp_bit_cond(emp_ctx_t *p, emp_bit_t *guardc, emp_bit_t *lhsc, emp_bit_t *rhsc) {
  emp_bit_t *v = (emp_bit_t *) malloc(sizeof(emp_bit_t));
  *v = emp_bit_cond_s(p, guardc, lhsc, rhsc);

  return v;
}

bool emp_bit_reveal(emp_ctx_t *p, emp_bit_t *vc, int party) {
  emp_install(p);

  Bit v(_mm_loadu_si128((__m128i *) vc->obj));
  bool ret = v.reveal<bool>(party);

  return ret;
}

void emp_bit_destroy(emp_bit_t *vc) {
  if (vc == NULL) {
    return;
  }

  free(vc);
}

struct emp_integer {
  void *obj;
};

emp_integer_t *emp_integer_create(emp_ctx_t *p, int length, int64_t init, int party) {
  emp_install(p);

  emp_integer_t *v;

  v      = (emp_integer_t *) malloc(sizeof(emp_integer_t));
  v->obj = new Integer(length, init, party);

  return v;
}

emp_integer_t *emp_integer_add(emp_ctx_t *p, emp_integer_t *lhsc, emp_integer_t *rhsc) {
  emp_install(p);

  Integer *lhs = static_cast<Integer *>(lhsc->obj);
  Integer *rhs = static_cast<Integer *>(rhsc->obj);

  emp_integer_t *v;
  v      = (emp_integer_t *) malloc(sizeof(emp_integer_t));
  v->obj = new Integer(*lhs + *rhs);

  return v;
}

emp_integer_t *emp_integer_sub(emp_ctx_t *p, emp_integer_t *lhsc, emp_integer_t *rhsc) {
  emp_install(p);

  Integer *lhs = static_cast<Integer *>(lhsc->obj);
  Integer *rhs = static_cast<Integer *>(rhsc->obj);

  emp_integer_t *v;
  v      = (emp_integer_t *) malloc(sizeof(emp_integer_t));
  v->obj = new Integer(*lhs - *rhs);

  return v;
}

emp_integer_t *emp_integer_mult(emp_ctx_t *p, emp_integer_t *lhsc, emp_integer_t *rhsc) {
  emp_install(p);

  Integer *lhs = static_cast<Integer *>(lhsc->obj);
  Integer *rhs = static_cast<Integer *>(rhsc->obj);

  emp_integer_t *v;
  v      = (emp_integer_t *) malloc(sizeof(emp_integer_t));
  v->obj = new Integer(*lhs * *rhs);

  return v;
}

emp_integer_t *emp_integer_div(emp_ctx_t *p, emp_integer_t *lhsc, emp_integer_t *rhsc) {
  emp_install(p);

  Integer *lhs = static_cast<Integer *>(lhsc->obj);
  Integer *rhs = static_cast<Integer *>(rhsc->obj);

  emp_integer_t *v;
  v      = (emp_integer_t *) malloc(sizeof(emp_integer_t));
  v->obj = new Integer(*lhs / *rhs);

  return v;
}

emp_integer_t *emp_integer_mod(emp_ctx_t *p, emp_integer_t *lhsc, emp_integer_t *rhsc) {
  emp_install(p);

  Integer *lhs = static_cast<Integer *>(lhsc->obj);
  Integer *rhs = static_cast<Integer *>(rhsc->obj);

  emp_integer_t *v;
  v      = (emp_integer_t *) malloc(sizeof(emp_integer_t));
  v->obj = new Integer(*lhs % *rhs);

  return v;
}

emp_bit_t *emp_integer_eq(emp_ctx_t *p, emp_integer_t *lhsc, emp_integer_t *rhsc) {
  emp_install(p);

  Integer *lhs = static_cast<Integer *>(lhsc->obj);
  Integer *rhs = static_cast<Integer *>(rhsc->obj);


  emp_bit_t *v;
  v      = (emp_bit_t *) malloc(sizeof(emp_bit_t));
  _mm_storeu_si128((__m128i *) v->obj, (*lhs == *rhs).bit);

  return v;
}

emp_bit_t *emp_integer_lt(emp_ctx_t *p, emp_integer_t *lhsc, emp_integer_t *rhsc) {
  emp_install(p);

  Integer *lhs = static_cast<Integer *>(lhsc->obj);
  Integer *rhs = static_cast<Integer *>(rhsc->obj);

  emp_bit_t *v;
  v      = (emp_bit_t *) malloc(sizeof(emp_bit_t));
  _mm_storeu_si128((__m128i *) v->obj, (*lhs < *rhs).bit);

  return v;
}

emp_bit_t *emp_integer_lte(emp_ctx_t *p, emp_integer_t *lhsc, emp_integer_t *rhsc) {
  emp_install(p);

  Integer *lhs = static_cast<Integer *>(lhsc->obj);
  Integer *rhs = static_cast<Integer *>(rhsc->obj);

  emp_bit_t *v;
  v      = (emp_bit_t *) malloc(sizeof(emp_bit_t));
  _mm_storeu_si128((__m128i *) v->obj, (*lhs <= *rhs).bit);

  return v;
}

emp_bit_t *emp_integer_gt(emp_ctx_t *p, emp_integer_t *lhsc, emp_integer_t *rhsc) {
  emp_install(p);

  Integer *lhs = static_cast<Integer *>(lhsc->obj);
  Integer *rhs = static_cast<Integer *>(rhsc->obj);

  emp_bit_t *v;
  v      = (emp_bit_t *) malloc(sizeof(emp_bit_t));
  _mm_storeu_si128((__m128i *) v->obj, (*lhs > *rhs).bit);

  return v;
}

emp_integer_t *emp_integer_cond(emp_ctx_t *p, emp_bit_t *guardc, emp_integer_t *lhsc, emp_integer_t *rhsc) {
  emp_install(p);

  Bit guard(_mm_loadu_si128((__m128i *) guardc->obj));
  Integer *lhs = static_cast<Integer *>(lhsc->obj);
  Integer *rhs = static_cast<Integer *>(rhsc->obj);

  emp_integer_t *v;
  v      = (emp_integer_t *) malloc(sizeof(emp_integer_t));
  v->obj= new Integer(If(guard, *lhs, *rhs));

  return v;
}

int64_t emp_integer_reveal(emp_ctx_t *p, emp_integer_t *vc, int party) {
  emp_install(p);

  Integer *v = static_cast<Integer *>(vc->obj);
  int64_t ret = (*v).reveal<int64_t>(party);

  return ret;
}

void emp_integer_destroy(emp_integer_t *vc) {
  if (vc == NULL) {
    return;
  }

  delete static_cast<Integer *>(vc->obj);
  free(vc);
}
