#include "net/net.h"
#include "emp-sh2pc/emp-sh2pc.h"

#include <stddef.h>
#include <stdbool.h>

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

void netio_destroy(netio_t *io) {
  if (io == NULL) {
    return;
  }

  delete static_cast<NetIO *>(io->obj);
  free(io);
}

void netio_send(netio_t *io, void *data, size_t bytes) {
  NetIO *net = static_cast<NetIO *>(io->obj);
  net->send_data_internal(data, bytes);
}

void netio_recv(netio_t *io, void *data, size_t bytes) {
  NetIO *net = static_cast<NetIO *>(io->obj);
  net->recv_data_internal(data, bytes);
}

void netio_flush(netio_t *io) {
  NetIO *net = static_cast<NetIO *>(io->obj);
  net->flush();
}
