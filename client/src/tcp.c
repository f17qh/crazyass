#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>

#include "tcp.h"

#define RECV_SIZE	(64 * 1024)
#define	TCPC_MAX_RSP	32
#define EV_READ	1
struct tcpc {
	struct tcp_client client;
	int sock;
	int reqid;
	int rspcount;
	size_t recvsize;
	char recvbuf[RECV_SIZE];
	tcp_response_t *resp_array[TCPC_MAX_RSP];
};

static pthread_t tcpc_thread;

static tcp_response_t * get_rsp(struct tcpc *tcpc, uint32_t seq)
{
	if (seq >= TCPC_MAX_RSP)
		return NULL;
	if (seq == 0)
		return NULL;
	return tcpc->resp_array[seq];
}

static void set_rsp(struct tcpc *tcpc, uint32_t seq, tcp_response_t *rsp)
{
	assert(seq != 0);
	assert(seq < TCPC_MAX_RSP);
	tcpc->resp_array[seq] = rsp;
}

uint32_t tcp_client_getseq(tcp_client_t *client)
{
	struct tcpc *tcpc = (struct tcpc *)client;
	uint32_t seq;

	seq = (++tcpc->reqid) & (TCPC_MAX_RSP - 1);
	if (seq == 0)
		seq = 1;

	/* full */
	if (tcpc->resp_array[seq])
		return 0;

	return seq;
}

int tcp_client_send(tcp_client_t *client, tcp_request_t *req, tcp_response_t *rsp)
{
	struct tcpc *tcpc = (struct tcpc *)client;
	int err;

	if (rsp && tcpc->rspcount >= (TCPC_MAX_RSP - 1))
		return -1;

	err = write(tcpc->sock, req->data, req->len);
	if (err < 0)
		return err;

	if (rsp == NULL)
		return 0;

	rsp->req = req;
	set_rsp(tcpc, req->seq, rsp);
	return 0;
}

static void tcpc_close(struct tcpc *tcpc, int err)
{
	if (tcpc->client.disconnect)
		tcpc->client.disconnect(&tcpc->client, err);
	close(tcpc->sock);
	free(tcpc);
}

static void tcpc_recv_event(int sock, short event_type, void *arg)
{
	struct tcpc *tcpc = (struct tcpc *)arg;
	tcp_response_t *rsp;
	int restsize, n;
	uint32_t seq;

	if (event_type != EV_READ)
		return;

	restsize = tcpc->client.restbytes(tcpc->recvbuf, tcpc->recvsize);
	assert(restsize >= 0);

	while (restsize > 0) {
		n = read(sock, tcpc->recvbuf + tcpc->recvsize, restsize);
		if (n < 0) {
			if (errno == -EAGAIN || errno == -EINPROGRESS)
				return;

			/* connection error, close it and send error event */
			tcpc_close(tcpc, errno);
			return;
		} else if (n == 0) {
			tcpc_close(tcpc, 0);
			return;
		} else {
			tcpc->recvsize += n;
			restsize = tcpc->client.restbytes(tcpc->recvbuf, tcpc->recvsize);
		}
	}

	seq = tcpc->client.getseq(tcpc->recvbuf, tcpc->recvsize);
	rsp = get_rsp(tcpc, seq);
	if (rsp == NULL) {
		tcpc_close(tcpc, -1);
		return;
	}

	n = rsp->proc(rsp, tcpc->recvbuf, tcpc->recvsize);
	if (n < 0) {
		close(sock);
		tcpc_close(tcpc, -1);
		return;
	}

	/* clear */
	tcpc->recvsize = 0;
	set_rsp(tcpc, seq, NULL);
	return;
}

static void * tcp_client_thread(void *arg)
{
	struct tcpc *tcpc = (struct tcpc *)arg;
	while (1) {
		tcpc_recv_event(tcpc->sock, EV_READ, arg);
	}
	return NULL;
}

tcp_client_t * tcp_client_new(const char *destip, uint16_t destport)
{
	struct sockaddr_in addr;
	struct tcpc *tcpc = (struct tcpc *)calloc(sizeof(struct tcpc), 1);
	if (tcpc == NULL)
		return NULL;

	strncpy(tcpc->client.dest_ip, destip, 32);
	tcpc->client.dest_port = destport;

	/* create socket, and start to connect */
	tcpc->sock = socket(AF_INET, SOCK_STREAM, 0);
	if (tcpc->sock < 0) {
		fprintf(stderr, "socket err %s\n", strerror(errno));
		goto sock_err;
	}

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(destip);
	addr.sin_port = htons(destport);

	if (connect(tcpc->sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		fprintf(stderr, "connect %s error %s\n", destip, strerror(errno));
		goto connect_err;
	}

	/* todo... */
	pthread_create(&tcpc_thread, NULL, tcp_client_thread, tcpc);
	return &tcpc->client;

connect_err:
	close(tcpc->sock);
sock_err:
	free(tcpc);
	return NULL;
}

void tcp_client_release(tcp_client_t *client)
{
	struct tcpc *tcpc = (struct tcpc *)client;
	tcpc_close(tcpc, 0);
}

#include <assert.h>

static int tcpc_test_proc(tcp_response_t *rsp, char *data, size_t len)
{
	printf("recv %s\n", data);
	return 0;
}

static int tcpc_test_restbytes(void *data, size_t size)
{
	assert(size <= 10);
	return 10 - (int)size;
}

static uint32_t tcpc_test_getseq(void *data, size_t size)
{
	return 1;
}

void tcpc_test()
{
	tcp_client_t *client;
	tcp_request_t req;
	tcp_response_t rsp;

	client = tcp_client_new("127.0.0.1", 23456);
	assert(client != NULL);
	client->restbytes = tcpc_test_restbytes;
	client->getseq = tcpc_test_getseq;

	req.data = "helloworld";
	req.len = strlen(req.data);
	req.seq = tcp_client_getseq(client);

	rsp.proc = tcpc_test_proc;
	tcp_client_send(client, &req, &rsp);
	sleep(10);
	tcp_client_release(client);
	return;
}
