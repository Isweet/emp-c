#include "empc.h"
#include "emp-sh2pc/emp-sh2pc.h"
using namespace emp;

struct netio {
  void *obj;
};

netio_t *netio_create(const char *address, int port) {
  netio_t *io;
  NetIO *obj;

  io     = (netio_t *) malloc(sizeof(netio_t));
  obj    = new NetIO(address, port);
  io->obj = obj;

  return io;
}

void netio_destroy(netio_t *io) {
  if (io == NULL) {
    return;
  }

  delete static_cast<NetIO *>(io->obj);
  free(io);
}

void setup_semi_honest_c(netio_t *io, int party) {
  if (io == NULL) {
    return;
  }

  NetIO *obj = static_cast<NetIO *>(io->obj);
  setup_semi_honest(obj, party);
}

struct bit {
  void *obj;
};

bit_t *bit_create(bool b, int party) {
  bit_t *v;
  Bit *obj;

  v      = (bit_t *) malloc(sizeof(bit_t));
  obj    = new Bit(b, party);
  v->obj = obj;

  return v;
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

integer_t *integer_create(int length, int64_t init, int party) {
  integer_t *v;
  Integer *obj;

  v      = (integer_t *) malloc(sizeof(integer_t));
  obj    = new Integer(length, init, party);
  v->obj = obj;

  return v;
}

integer_t *integer_add(integer_t *lhsc, integer_t *rhsc) {
  Integer *lhs = static_cast<Integer *>(lhsc->obj);
  Integer *rhs = static_cast<Integer *>(rhsc->obj);

  integer_t *v;
  Integer *obj;
  v      = (integer_t *) malloc(sizeof(integer_t));
  obj    = new Integer(*lhs + *rhs);
  v->obj = obj;
  return v;
}

int64_t integer_reveal(integer_t *vc, int party) {
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
