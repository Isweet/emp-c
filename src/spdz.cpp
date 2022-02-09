#include "empc.h"

#include "Machines/Semi.hpp"
#include "Protocols/RepRingOnlyEdabitPrep.hpp"
#include "Protocols/Semi2kShare.h"
#include "Protocols/SemiPrep2k.h"
#include "Protocols/ProtocolSet.h"

struct spdz_ctx {
  void *names;
  void *player;
  void *set;
};

spdz_ctx_t *spdz_create(int party, int nparties) {
  spdz_ctx_t *p;

  p = (spdz_ctx_t *) malloc(sizeof(spdz_ctx_t));
  Names *N = new Names(party, nparties, "localhost", 9999);
  PlainPlayer *P = new PlainPlayer(*N);
  ProtocolSetup<Semi2kShare<64>> setup(*P);
  ProtocolSet<Semi2kShare<64>> *S = new ProtocolSet<Semi2kShare<64>>(*P, setup);
  p->names = N;
  p->player = P;
  p->set = S;

  return p;
}

void spdz_destroy(spdz_ctx_t *p) {
  Names *N = static_cast<Names *>(p->names);
  PlainPlayer *P = static_cast<PlainPlayer *>(p->player);
  ProtocolSet<Semi2kShare<64>> *S = static_cast<ProtocolSet<Semi2kShare<64>> *>(p->set);

  delete N;
  delete P;
  delete S;
  free(p);
}

struct spdz_int64 {
  void *obj;
};

spdz_int64_t *spdz_int64_create(spdz_ctx_t *p, int party, int64_t v) {
  assert(sizeof(long) == 8); // MP-SPDZ assumption

  PlainPlayer *P = static_cast<PlainPlayer *>(p->player);
  ProtocolSet<Semi2kShare<64>> *set = static_cast<ProtocolSet<Semi2kShare<64>> *>(p->set);
  auto& input = set->input;

  if (P->my_num() == party) {
    input.add_from_all(static_cast<long>(v));
  }
  input.exchange();

  spdz_int64_t *ret = (spdz_int64_t *) malloc(sizeof(spdz_int64_t));
  ret->obj = new Semi2kShare<64>(input.finalize(party));

  return ret;
}

spdz_int64_t *spdz_int64_add(spdz_ctx_t *p, spdz_int64_t *lhs, spdz_int64_t *rhs) {
  Semi2kShare<64> *a = static_cast<Semi2kShare<64> *>(lhs->obj);
  Semi2kShare<64> *b = static_cast<Semi2kShare<64> *>(rhs->obj);

  spdz_int64_t *ret = (spdz_int64_t *) malloc(sizeof(spdz_int64_t));
  ret->obj = new Semi2kShare<64>(*a + *b);

  return ret;
}

spdz_int64_t *spdz_int64_mult(spdz_ctx_t *p, spdz_int64_t *lhs, spdz_int64_t *rhs) {
  ProtocolSet<Semi2kShare<64>> *set = static_cast<ProtocolSet<Semi2kShare<64>> *>(p->set);
  auto& protocol = set->protocol;

  Semi2kShare<64> *a = static_cast<Semi2kShare<64> *>(lhs->obj);
  Semi2kShare<64> *b = static_cast<Semi2kShare<64> *>(rhs->obj);

  spdz_int64_t *ret = (spdz_int64_t *) malloc(sizeof(spdz_int64_t));
  ret->obj = new Semi2kShare<64>(protocol.mul(*a, *b));

  return ret;
}

int64_t spdz_int64_reveal(spdz_ctx_t *p, int party, spdz_int64_t *v) {
  assert(sizeof(long) == 8); // MP-SPDZ assumption

  PlainPlayer *P = static_cast<PlainPlayer *>(p->player);
  ProtocolSet<Semi2kShare<64>> *set = static_cast<ProtocolSet<Semi2kShare<64>> *>(p->set);
  auto& output = set->output;

  Semi2kShare<64> *x = static_cast<Semi2kShare<64> *>(v->obj);

  output.init_open(*P);
  output.prepare_open(x);
  output.exchange(*P);
  SignedZ2<64> r = output.finalize_open();
  bigint r_bi;
  r.to(r_bi);

  return r_bi.get_si();
}

void spdz_int64_destroy(spdz_int64_t *sh) {
  Semi2kShare<64> *x = static_cast<Semi2kShare<64> *>(sh->obj);

  delete x;
  free(sh);
}
