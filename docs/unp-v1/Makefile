CC=cc
CFLAGS+=-Wall -Werror -Wformat=2 -g
LDFLAGS=-I./src/include

all: udpserv01 udpcli01 udpcli02 udpcli03 udpcli04\
	udpcli_multicasttest udpclibcast1 udpclibcast5

.PHONY: error.o
error.o: src/lib/error.c
	$(CC) -c $(CFLAGS) $(LDFLAGS) $< -o $@

.PHONY: sock.o
sock.o: src/lib/sock.c
	$(CC) -c $(CFLAGS) $(LDFLAGS) $< -o $@

.PHONY: stdio.o
stdio.o: src/lib/stdio.c
	$(CC) -c $(CFLAGS) $(LDFLAGS) $< -o $@

.PHONY: udpserv01
udpserv01: src/chap8/udpserv01.c error.o sock.o stdio.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

.PHONY: dg_cli.o
dg_cli.o: src/chap8/dg_cli.c
	$(CC) -c $(CFLAGS) $(LDFLAGS) $< -o $@

.PHONY: udpcli01
udpcli01: src/chap8/udpcli01.c error.o sock.o stdio.o dg_cli.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@


.PHONY: dgcliaddr.o
dgcliaddr.o: src/chap8/dgcliaddr.c
	$(CC) -c $(CFLAGS) $(LDFLAGS) $< -o $@

.PHONY: udpcli02 // 校验服务端地址版本
udpcli02: src/chap8/udpcli01.c error.o sock.o stdio.o dgcliaddr.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@


.PHONY: udpcli03 // 使用系统自带的echo服务版本（默认端口是7）
udpcli03: src/chap8/udpcli03.c error.o sock.o stdio.o dgcliaddr.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@ -DSERV_PORT=7


.PHONY: dgcliconnect.o
dgcliconnect.o: src/chap8/dgcliconnect.c
	$(CC) -c $(CFLAGS) $(LDFLAGS) $< -o $@

.PHONY: unix.o
unix.o: src/lib/unix.c
	$(CC) -c $(CFLAGS) $(LDFLAGS) $< -o $@

.PHONY: udpcli04 // 显示建立连接的UDP
udpcli04: src/chap8/udpcli01.c error.o sock.o stdio.o dgcliconnect.o unix.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

# chap20

.PHONY: udpcli_multicasttest // 测试未设置SO_BROADCAST选项时发送广播
udpcli_multicasttest: src/chap8/udpcli03.c error.o sock.o stdio.o dgcliaddr.o unix.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@ -DSERV_PORT=13

.PHONY: signal.o
signal.o: src/lib/signal.c
	$(CC) -c $(CFLAGS) $(LDFLAGS) $< -o $@

.PHONY: dgclibcast1.o
dgclibcast1.o: src/chap20/dgclibcast1.c
	$(CC) -c $(CFLAGS) $(LDFLAGS) $< -o $@

.PHONY: udpclibcast1
udpclibcast1: src/chap8/udpcli03.c error.o sock.o stdio.o dgclibcast1.o unix.o signal.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@ -DSERV_PORT=13

.PHONY: dgclibcast5.o
dgclibcast5.o: src/chap20/dgclibcast5.c
	$(CC) -c $(CFLAGS) $(LDFLAGS) $< -o $@

.PHONY: udpclibcast5
udpclibcast5: src/chap8/udpcli03.c error.o sock.o stdio.o dgclibcast5.o unix.o signal.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@ -DSERV_PORT=13

tidy:
	rm -rf *.o

clean:
	rm -f *.o
	rm -f udpserv01 udpcli01 udpcli02 udpcli03 udpcli04
	rm -rf *.dSYM
