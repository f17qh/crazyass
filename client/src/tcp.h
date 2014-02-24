#ifndef	__DOGRUN2_TCP_H__
#define	__DOGRUN2_TCP_H__

#include <inttypes.h>

#ifdef	__cplusplus
extern "C" {
#endif

struct tcp_client {
	char dest_ip[32];
	uint16_t dest_port;
	// int (*connection_made)(struct tcp_client *)
	void (*disconnect)(struct tcp_client *, int err);
	int (*restbytes)(void *data, size_t size);
	uint32_t (*getseq)(void *data, size_t size);
};

typedef struct tcp_client tcp_client_t;

struct tcp_request {
	uint32_t seq;
	void *data;
	size_t len;
	// int (*sent)(struct tcp_request *, ssize_t sentsize, int errno);
};

typedef struct tcp_request tcp_request_t;

struct tcp_response {
	tcp_request_t *req;
	void *client;
	int (*proc)(struct tcp_response *, char *data, size_t len);
};

typedef struct tcp_response tcp_response_t;

extern tcp_client_t * tcp_client_new(const char *destip, uint16_t destport);
extern uint32_t tcp_client_getseq(tcp_client_t *);
extern int tcp_client_send(tcp_client_t *, tcp_request_t *, tcp_response_t *t);
extern void tcp_client_release(tcp_client_t *);

#ifdef	__cplusplus
}
#endif

#endif
