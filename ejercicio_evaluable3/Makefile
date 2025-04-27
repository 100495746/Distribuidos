CC = gcc
CFLAGS = -Wall -Wextra -pthread -fPIC -D_REENTRANT -I. -I/usr/include/tirpc
LDFLAGS = -ltirpc

RPC_X = claves_rpc.x
RPCGEN_FLAGS = -M -C  # -M genera servidor multihilo

# Generated files
RPC_HDR  = claves_rpc.h
RPC_XDR  = claves_rpc_xdr.c
RPC_CLNT = claves_rpc_clnt.c
RPC_SVC  = claves_rpc_svc.c

.PHONY: all rpcgen clean

all: rpcgen libclaves.so servidor cliente

rpcgen: $(RPC_X)
	rpcgen $(RPCGEN_FLAGS) -h -o $(RPC_HDR) $(RPC_X)
	rpcgen $(RPCGEN_FLAGS) -c -o $(RPC_XDR) $(RPC_X)
	rpcgen $(RPCGEN_FLAGS) -l -o $(RPC_CLNT) $(RPC_X)
	rpcgen $(RPCGEN_FLAGS) -s tcp -o $(RPC_SVC) $(RPC_X)

libclaves.so: proxy-rpc.o claves.o $(RPC_CLNT:.c=.o) $(RPC_XDR:.c=.o)
	$(CC) -shared -o $@ $^ $(CFLAGS) $(LDFLAGS)

servidor: servidor-rpc.o claves.o $(RPC_SVC:.c=.o) $(RPC_XDR:.c:.o)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) -lpthread

cliente: app-cliente.o claves.o proxy-rpc.o $(RPC_CLNT:.c:.o) $(RPC_XDR:.c:.o)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) -lpthread

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -f *.o servidor cliente libclaves.so $(RPC_HDR) $(RPC_XDR) $(RPC_CLNT) $(RPC_SVC)