CC      = gcc
CFLAGS  = -Wall -Wextra -pthread -fPIC -I. -I/usr/include/tirpc -D_REENTRANT
LDFLAGS = -ltirpc

RPC_STUBS = claves_rpc.h claves_rpc_xdr.c claves_rpc_clnt.c claves_rpc_svc.c

all: $(RPC_STUBS) libclaves.so servidor cliente

$(RPC_STUBS): claves_rpc.x
	rpcgen -M -C -h -o claves_rpc.h        $<
	rpcgen -M -C -c -o claves_rpc_xdr.c    $<
	rpcgen -M -C -l -o claves_rpc_clnt.c   $<
	rpcgen -M -C -s tcp -o claves_rpc_svc.c $<

libclaves.so: proxy-rpc.o claves_rpc_clnt.o claves_rpc_xdr.o
	$(CC) -shared -o $@ $^ $(CFLAGS) $(LDFLAGS)

servidor: servidor-rpc.o claves.o claves_rpc_svc.o claves_rpc_xdr.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

cliente: app-cliente.o proxy-rpc.o claves_rpc_clnt.o claves_rpc_xdr.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o servidor cliente libclaves.so $(RPC_STUBS)



