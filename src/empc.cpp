#include "empc.h"
#include "emp-sh2pc/emp-sh2pc.h"
using namespace emp;

struct netio {
  void *obj;
};

netio_t *netio_create(const char *address, int port) {
  netio_t *io;

  io      = (netio_t *) malloc(sizeof(netio_t));
  io->obj = new NetIO(address, port, false);

  return io;
}

void netio_send_data(netio_t *io, void *data, int nbyte) {
  NetIO *net = static_cast<NetIO *>(io->obj);
  net->send_data(data, nbyte);
}

void netio_recv_data(netio_t *io, void *data, int nbyte) {
  NetIO *net = static_cast<NetIO *>(io->obj);
  net->recv_data(data, nbyte);
}

void netio_destroy(netio_t *io) {
  if (io == NULL) {
    return;
  }

  delete static_cast<NetIO *>(io->obj);
  free(io);
}

struct semihonest {
  void *circ;
  void *prot;
};

semihonest_t *setup_semi_honest_c(netio_t *io, int party) {
  if (io == NULL) {
    return NULL;
  }

  NetIO *net = static_cast<NetIO *>(io->obj);

  semihonest_t *sh;

  sh = (semihonest_t *) malloc(sizeof(semihonest_t));
  setup_semi_honest(net, party);
  sh->circ = CircuitExecution::circ_exec;
  sh->prot = ProtocolExecution::prot_exec;

  return sh;
}

void install_sh(semihonest_t *sh) {
  CircuitExecution::circ_exec = static_cast<CircuitExecution *>(sh->circ);
  ProtocolExecution::prot_exec = static_cast<ProtocolExecution *>(sh->prot);
  return;
}

void finalize_semi_honest_c(semihonest_t *sh) {
  install_sh(sh);
  finalize_semi_honest();
  free(sh);
}

struct bit {
  void *obj;
};

bit_t *bit_create(semihonest_t *sh, bool b, int party) {
  install_sh(sh);

  bit_t *v;
  v      = (bit_t *) malloc(sizeof(bit_t));
  v->obj = new Bit(b, party);

  return v;
}

bit_t *bit_not(semihonest_t *sh, bit_t *bc) {
  install_sh(sh);

  Bit *b = static_cast<Bit *>(bc->obj);

  bit_t *v;
  v      = (bit_t *) malloc(sizeof(bit_t));
  v->obj = new Bit(!*b);

  return v;
}

bit_t *bit_and(semihonest_t *sh, bit_t *lhsc, bit_t *rhsc) {
  install_sh(sh);

  Bit *lhs = static_cast<Bit *>(lhsc->obj);
  Bit *rhs = static_cast<Bit *>(rhsc->obj);

  bit_t *v;
  v      = (bit_t *) malloc(sizeof(bit_t));
  v->obj = new Bit(*lhs & *rhs);

  return v;
}

bit_t *bit_or(semihonest_t *sh, bit_t *lhsc, bit_t *rhsc) {
  install_sh(sh);

  Bit *lhs = static_cast<Bit *>(lhsc->obj);
  Bit *rhs = static_cast<Bit *>(rhsc->obj);

  bit_t *v;
  v      = (bit_t *) malloc(sizeof(bit_t));
  v->obj = new Bit(*lhs | *rhs);

  return v;
}

bit_t *bit_xor(semihonest_t *sh, bit_t *lhsc, bit_t *rhsc) {
  install_sh(sh);

  Bit *lhs = static_cast<Bit *>(lhsc->obj);
  Bit *rhs = static_cast<Bit *>(rhsc->obj);

  bit_t *v;
  v      = (bit_t *) malloc(sizeof(bit_t));
  v->obj = new Bit(*lhs ^ *rhs);

  return v;
}

bit_t *bit_cond(semihonest_t *sh, bit_t *guardc, bit_t *lhsc, bit_t *rhsc) {
  install_sh(sh);

  Bit *guard = static_cast<Bit *>(guardc->obj);
  Bit *lhs   = static_cast<Bit *>(lhsc->obj);
  Bit *rhs   = static_cast<Bit *>(rhsc->obj);

  bit_t *v;
  v      = (bit_t *) malloc(sizeof(bit_t));
  v->obj = new Bit(If(*guard, *lhs, *rhs));

  return v;
}

bool bit_reveal(semihonest_t *sh, bit_t *vc, int party) {
  install_sh(sh);

  Bit *v = static_cast<Bit *>(vc->obj);

  return (*v).reveal<bool>(party);
}

void bit_destroy(bit_t *vc) {
  if (vc == NULL) {
    return;
  }

  delete static_cast<Bit *>(vc->obj);
  free(vc);
}

struct integer {
  void *obj;
};

integer_t *integer_create(semihonest_t *sh, int length, int64_t init, int party) {
  install_sh(sh);

  integer_t *v;

  v      = (integer_t *) malloc(sizeof(integer_t));
  v->obj = new Integer(length, init, party);

  return v;
}

integer_t *integer_add(semihonest_t *sh, integer_t *lhsc, integer_t *rhsc) {
  install_sh(sh);

  Integer *lhs = static_cast<Integer *>(lhsc->obj);
  Integer *rhs = static_cast<Integer *>(rhsc->obj);

  integer_t *v;
  v      = (integer_t *) malloc(sizeof(integer_t));
  v->obj = new Integer(*lhs + *rhs);

  return v;
}

integer_t *integer_sub(semihonest_t *sh, integer_t *lhsc, integer_t *rhsc) {
  install_sh(sh);

  Integer *lhs = static_cast<Integer *>(lhsc->obj);
  Integer *rhs = static_cast<Integer *>(rhsc->obj);

  integer_t *v;
  v      = (integer_t *) malloc(sizeof(integer_t));
  v->obj = new Integer(*lhs - *rhs);

  return v;
}

integer_t *integer_mult(semihonest_t *sh, integer_t *lhsc, integer_t *rhsc) {
  install_sh(sh);

  Integer *lhs = static_cast<Integer *>(lhsc->obj);
  Integer *rhs = static_cast<Integer *>(rhsc->obj);

  integer_t *v;
  v      = (integer_t *) malloc(sizeof(integer_t));
  v->obj = new Integer(*lhs * *rhs);

  return v;
}

integer_t *integer_div(semihonest_t *sh, integer_t *lhsc, integer_t *rhsc) {
  install_sh(sh);

  Integer *lhs = static_cast<Integer *>(lhsc->obj);
  Integer *rhs = static_cast<Integer *>(rhsc->obj);

  integer_t *v;
  v      = (integer_t *) malloc(sizeof(integer_t));
  v->obj = new Integer(*lhs / *rhs);

  return v;
}

integer_t *integer_mod(semihonest_t *sh, integer_t *lhsc, integer_t *rhsc) {
  install_sh(sh);

  Integer *lhs = static_cast<Integer *>(lhsc->obj);
  Integer *rhs = static_cast<Integer *>(rhsc->obj);

  integer_t *v;
  v      = (integer_t *) malloc(sizeof(integer_t));
  v->obj = new Integer(*lhs % *rhs);

  return v;
}

bit_t *integer_eq(semihonest_t *sh, integer_t *lhsc, integer_t *rhsc) {
  install_sh(sh);

  Integer *lhs = static_cast<Integer *>(lhsc->obj);
  Integer *rhs = static_cast<Integer *>(rhsc->obj);

  bit_t *v;
  v      = (bit_t *) malloc(sizeof(bit_t));
  v->obj = new Bit(*lhs == *rhs);

  return v;
}

bit_t *integer_lt(semihonest_t *sh, integer_t *lhsc, integer_t *rhsc) {
  install_sh(sh);

  Integer *lhs = static_cast<Integer *>(lhsc->obj);
  Integer *rhs = static_cast<Integer *>(rhsc->obj);

  bit_t *v;
  v      = (bit_t *) malloc(sizeof(bit_t));
  v->obj = new Bit(*lhs < *rhs);

  return v;
}

bit_t *integer_gt(semihonest_t *sh, integer_t *lhsc, integer_t *rhsc) {
  install_sh(sh);

  Integer *lhs = static_cast<Integer *>(lhsc->obj);
  Integer *rhs = static_cast<Integer *>(rhsc->obj);

  bit_t *v;
  v      = (bit_t *) malloc(sizeof(bit_t));
  v->obj = new Bit(*lhs > *rhs);

  return v;
}

integer_t *integer_cond(semihonest_t *sh, bit_t *guardc, integer_t *lhsc, integer_t *rhsc) {
  install_sh(sh);

  Bit *guard   = static_cast<Bit *>(guardc->obj);
  Integer *lhs = static_cast<Integer *>(lhsc->obj);
  Integer *rhs = static_cast<Integer *>(rhsc->obj);

  integer_t *v;
  v      = (integer_t *) malloc(sizeof(integer_t));
  v->obj= new Integer(If(*guard, *lhs, *rhs));

  return v;
}

int64_t integer_reveal(semihonest_t *sh, integer_t *vc, int party) {
  install_sh(sh);

  Integer *v = static_cast<Integer *>(vc->obj);

  return (*v).reveal<int64_t>(party);
}

void integer_destroy(integer_t *vc) {
  if (vc == NULL) {
    return;
  }

  delete static_cast<Integer *>(vc->obj);
  free(vc);
}
