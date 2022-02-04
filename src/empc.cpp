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

struct protocol {
  int tag;
  void *circ;
  void *prot;
};

protocol_t *sh_create(netio_t *io, int party) {
  NetIO *net = static_cast<NetIO *>(io->obj);

  protocol_t *p;

  p = (protocol_t *) malloc(sizeof(protocol_t));
  p->tag  = 0;
  setup_semi_honest(net, party);
  p->circ = CircuitExecution::circ_exec;
  p->prot = ProtocolExecution::prot_exec;

  return p;
}

protocol_t *plain_create() {
  protocol_t *p;

  p = (protocol_t *) malloc(sizeof(protocol_t));
  p->tag = 1;
  setup_plain_prot(false, "/dev/null");
  p->circ = CircuitExecution::circ_exec;
  p->prot = ProtocolExecution::prot_exec;

  return p;
}

void protocol_install(protocol_t *p) {
  CircuitExecution::circ_exec = static_cast<CircuitExecution *>(p->circ);
  ProtocolExecution::prot_exec = static_cast<ProtocolExecution *>(p->prot);
  return;
}

void protocol_destroy(protocol_t *p) {
  protocol_install(p);

  if (p->tag == 0) {
    finalize_semi_honest();
  } else if (p->tag == 1) {
    finalize_plain_prot();
  } else {
    assert(false);
  }

  free(p);
}

bit_t bit_create_s(protocol_t *p, bool b, int party) {
  protocol_install(p);

  Bit tmp(b, party);

  bit_t v;
  _mm_storeu_si128((__m128i *) v.obj, tmp.bit);

  return v;
}

bit_t bit_not_s(protocol_t *p, bit_t *bc) {
  protocol_install(p);

  Bit b(_mm_loadu_si128((__m128i *) bc->obj));

  bit_t v;
  _mm_storeu_si128((__m128i *) v.obj, (!b).bit);

  return v;
}

bit_t bit_and_s(protocol_t *p, bit_t *lhsc, bit_t *rhsc) {
  protocol_install(p);

  Bit lhs(_mm_loadu_si128((__m128i *) lhsc->obj));
  Bit rhs(_mm_loadu_si128((__m128i *) rhsc->obj));

  bit_t v;
  _mm_storeu_si128((__m128i *) v.obj, (lhs & rhs).bit);

  return v;
}

bit_t bit_or_s(protocol_t *p, bit_t *lhsc, bit_t *rhsc) {
  protocol_install(p);

  Bit lhs(_mm_loadu_si128((__m128i *) lhsc->obj));
  Bit rhs(_mm_loadu_si128((__m128i *) rhsc->obj));

  bit_t v;
  _mm_storeu_si128((__m128i *) v.obj, (lhs | rhs).bit);

  return v;
}

bit_t bit_xor_s(protocol_t *p, bit_t *lhsc, bit_t *rhsc) {
  protocol_install(p);

  Bit lhs(_mm_loadu_si128((__m128i *) lhsc->obj));
  Bit rhs(_mm_loadu_si128((__m128i *) rhsc->obj));

  bit_t v;
  _mm_storeu_si128((__m128i *) v.obj, (lhs ^ rhs).bit);

  return v;
}

bit_t bit_cond_s(protocol_t *p, bit_t *guardc, bit_t *lhsc, bit_t *rhsc) {
  protocol_install(p);

  Bit guard(_mm_loadu_si128((__m128i *) guardc->obj));
  Bit lhs(_mm_loadu_si128((__m128i *) lhsc->obj));
  Bit rhs(_mm_loadu_si128((__m128i *) rhsc->obj));

  bit_t v;
  _mm_storeu_si128((__m128i *) v.obj, If(guard, lhs, rhs).bit);

  return v;
}

bit_t *bit_create(protocol_t *p, bool b, int party) {
  bit_t *v = (bit_t *) malloc(sizeof(bit_t));
  *v = bit_create_s(p, b, party);

  return v;
}

bit_t *bit_not(protocol_t *p, bit_t *bc) {
  bit_t *v = (bit_t *) malloc(sizeof(bit_t));
  *v = bit_not_s(p, bc);

  return v;
}

bit_t *bit_and(protocol_t *p, bit_t *lhsc, bit_t *rhsc) {
  bit_t *v = (bit_t *) malloc(sizeof(bit_t));
  *v = bit_and_s(p, lhsc, rhsc);

  return v;
}

bit_t *bit_or(protocol_t *p, bit_t *lhsc, bit_t *rhsc) {
  bit_t *v = (bit_t *) malloc(sizeof(bit_t));
  *v = bit_or_s(p, lhsc, rhsc);

  return v;
}

bit_t *bit_xor(protocol_t *p, bit_t *lhsc, bit_t *rhsc) {
  bit_t *v = (bit_t *) malloc(sizeof(bit_t));
  *v = bit_xor_s(p, lhsc, rhsc);

  return v;
}

bit_t *bit_cond(protocol_t *p, bit_t *guardc, bit_t *lhsc, bit_t *rhsc) {
  bit_t *v = (bit_t *) malloc(sizeof(bit_t));
  *v = bit_cond_s(p, guardc, lhsc, rhsc);

  return v;
}

bool bit_reveal(protocol_t *p, bit_t *vc, int party) {
  protocol_install(p);

  Bit v(_mm_loadu_si128((__m128i *) vc->obj));
  bool ret = v.reveal<bool>(party);

  return ret;
}

void bit_destroy(bit_t *vc) {
  if (vc == NULL) {
    return;
  }

  free(vc);
}

struct integer {
  void *obj;
};

integer_t *integer_create(protocol_t *p, int length, int64_t init, int party) {
  protocol_install(p);

  integer_t *v;

  v      = (integer_t *) malloc(sizeof(integer_t));
  v->obj = new Integer(length, init, party);

  return v;
}

integer_t *integer_add(protocol_t *p, integer_t *lhsc, integer_t *rhsc) {
  protocol_install(p);

  Integer *lhs = static_cast<Integer *>(lhsc->obj);
  Integer *rhs = static_cast<Integer *>(rhsc->obj);

  integer_t *v;
  v      = (integer_t *) malloc(sizeof(integer_t));
  v->obj = new Integer(*lhs + *rhs);

  return v;
}

integer_t *integer_sub(protocol_t *p, integer_t *lhsc, integer_t *rhsc) {
  protocol_install(p);

  Integer *lhs = static_cast<Integer *>(lhsc->obj);
  Integer *rhs = static_cast<Integer *>(rhsc->obj);

  integer_t *v;
  v      = (integer_t *) malloc(sizeof(integer_t));
  v->obj = new Integer(*lhs - *rhs);

  return v;
}

integer_t *integer_mult(protocol_t *p, integer_t *lhsc, integer_t *rhsc) {
  protocol_install(p);

  Integer *lhs = static_cast<Integer *>(lhsc->obj);
  Integer *rhs = static_cast<Integer *>(rhsc->obj);

  integer_t *v;
  v      = (integer_t *) malloc(sizeof(integer_t));
  v->obj = new Integer(*lhs * *rhs);

  return v;
}

integer_t *integer_div(protocol_t *p, integer_t *lhsc, integer_t *rhsc) {
  protocol_install(p);

  Integer *lhs = static_cast<Integer *>(lhsc->obj);
  Integer *rhs = static_cast<Integer *>(rhsc->obj);

  integer_t *v;
  v      = (integer_t *) malloc(sizeof(integer_t));
  v->obj = new Integer(*lhs / *rhs);

  return v;
}

integer_t *integer_mod(protocol_t *p, integer_t *lhsc, integer_t *rhsc) {
  protocol_install(p);

  Integer *lhs = static_cast<Integer *>(lhsc->obj);
  Integer *rhs = static_cast<Integer *>(rhsc->obj);

  integer_t *v;
  v      = (integer_t *) malloc(sizeof(integer_t));
  v->obj = new Integer(*lhs % *rhs);

  return v;
}

bit_t *integer_eq(protocol_t *p, integer_t *lhsc, integer_t *rhsc) {
  protocol_install(p);

  Integer *lhs = static_cast<Integer *>(lhsc->obj);
  Integer *rhs = static_cast<Integer *>(rhsc->obj);


  bit_t *v;
  v      = (bit_t *) malloc(sizeof(bit_t));
  _mm_storeu_si128((__m128i *) v->obj, (*lhs == *rhs).bit);

  return v;
}

bit_t *integer_lt(protocol_t *p, integer_t *lhsc, integer_t *rhsc) {
  protocol_install(p);

  Integer *lhs = static_cast<Integer *>(lhsc->obj);
  Integer *rhs = static_cast<Integer *>(rhsc->obj);

  bit_t *v;
  v      = (bit_t *) malloc(sizeof(bit_t));
  _mm_storeu_si128((__m128i *) v->obj, (*lhs < *rhs).bit);

  return v;
}

bit_t *integer_lte(protocol_t *p, integer_t *lhsc, integer_t *rhsc) {
  protocol_install(p);

  Integer *lhs = static_cast<Integer *>(lhsc->obj);
  Integer *rhs = static_cast<Integer *>(rhsc->obj);

  bit_t *v;
  v      = (bit_t *) malloc(sizeof(bit_t));
  _mm_storeu_si128((__m128i *) v->obj, (*lhs <= *rhs).bit);

  return v;
}

bit_t *integer_gt(protocol_t *p, integer_t *lhsc, integer_t *rhsc) {
  protocol_install(p);

  Integer *lhs = static_cast<Integer *>(lhsc->obj);
  Integer *rhs = static_cast<Integer *>(rhsc->obj);

  bit_t *v;
  v      = (bit_t *) malloc(sizeof(bit_t));
  _mm_storeu_si128((__m128i *) v->obj, (*lhs > *rhs).bit);

  return v;
}

integer_t *integer_cond(protocol_t *p, bit_t *guardc, integer_t *lhsc, integer_t *rhsc) {
  protocol_install(p);

  Bit guard(_mm_loadu_si128((__m128i *) guardc->obj));
  Integer *lhs = static_cast<Integer *>(lhsc->obj);
  Integer *rhs = static_cast<Integer *>(rhsc->obj);

  integer_t *v;
  v      = (integer_t *) malloc(sizeof(integer_t));
  v->obj= new Integer(If(guard, *lhs, *rhs));

  return v;
}

int64_t integer_reveal(protocol_t *p, integer_t *vc, int party) {
  protocol_install(p);

  Integer *v = static_cast<Integer *>(vc->obj);
  int64_t ret = (*v).reveal<int64_t>(party);

  return ret;
}

void integer_destroy(integer_t *vc) {
  if (vc == NULL) {
    return;
  }

  delete static_cast<Integer *>(vc->obj);
  free(vc);
}
