#include "empc.h"

#include "Machines/Semi.hpp"
#include "Protocols/RepRingOnlyEdabitPrep.hpp"
#include "Protocols/Semi2kShare.h"
#include "Protocols/SemiPrep2k.h"
#include "Protocols/ProtocolSet.h"

struct spdz {
  void *names;
  void *player;
  void *set;
};

spdz_t *spdz_create(int party, int nparties) {
  spdz_t *p;

  p = (spdz_t *) malloc(sizeof(spdz_t));
  Names *N = new Names(party, nparties, "localhost", 9999);
  PlainPlayer *P = new PlainPlayer(*N);
  ProtocolSetup<Semi2kShare<64>> setup(*P);
  ProtocolSet<Semi2kShare<64>> *S = new ProtocolSet<Semi2kShare<64>>(*P, setup);
  p->names = N;
  p->player = P;
  p->set = S;

  return p;
}
