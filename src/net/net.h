#ifndef SYMPHONY_NET_H__
#define SYMPHONY_NET_H__

#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

  struct netio;
  typedef struct netio netio_t;
  netio_t *netio_create(const char *addr, int port, bool quiet);
  void netio_destroy(netio_t *io);

  void netio_send(netio_t *io, void *data, size_t bytes);
  void netio_recv(netio_t *io, void *data, size_t bytes);
  void netio_flush(netio_t *io);

#ifdef __cplusplus
}
#endif

#endif
