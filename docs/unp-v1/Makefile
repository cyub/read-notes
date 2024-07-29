CC=cc
CFLAGS+=-Wall -Werror -Wformat=2 -g
LDFLAGS=-I./src/include -L./src/lib

COMPILER_VERSION=$(shell $(CC) --version)
ifneq '' '$(findstring clang, $(COMPILER_VERSION))'
	CFLAGS += -Qunused-arguments
endif

lib := src/lib
bin := udpserv01 udpcli01 udpcli02 udpcli03 udpcli04\
	udpcli_multicasttest udpclibcast1 udpclibcast5
all: $(bin)

lib_objects :=$(patsubst %.c, %.o, $(wildcard $(lib)/*.c))

$(lib_objects): %.o : %.c
	$(CC) -c $(CFLAGS) $(LDFLAGS) $< -o $@

.PHONY: udpserv01
udpserv01: src/chap8/udpserv01.c $(lib_objects)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

.PHONY: dg_cli.o
dg_cli.o: src/chap8/dg_cli.c
	$(CC) -c $(CFLAGS) $(LDFLAGS) $< -o $@

.PHONY: udpcli01
udpcli01: $(lib_objects) src/chap8/udpcli01.c dg_cli.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

.PHONY: dgcliaddr.o
dgcliaddr.o: src/chap8/dgcliaddr.c
	$(CC) -c $(CFLAGS) $(LDFLAGS) $< -o $@

.PHONY: udpcli02 // 校验服务端地址版本
udpcli02: $(lib_objects) src/chap8/udpcli01.c dgcliaddr.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

.PHONY: udpcli03 // 使用系统自带的echo服务版本（默认端口是7）
udpcli03:  $(lib_objects) src/chap8/udpcli03.c dgcliaddr.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@ -DSERV_PORT=7

.PHONY: dgcliconnect.o
dgcliconnect.o: src/chap8/dgcliconnect.c
	$(CC) -c $(CFLAGS) $(LDFLAGS) $< -o $@

.PHONY: udpcli04 // 显示建立连接的UDP
udpcli04: src/chap8/udpcli01.c dgcliconnect.o $(lib_objects)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

# chap20
.PHONY: udpcli_multicasttest // 测试未设置SO_BROADCAST选项时发送广播
udpcli_multicasttest: $(lib_objects) src/chap8/udpcli03.c dgcliaddr.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@ -DSERV_PORT=13

.PHONY: dgclibcast1.o
dgclibcast1.o: src/chap20/dgclibcast1.c
	$(CC) -c $(CFLAGS) $(LDFLAGS) $< -o $@

.PHONY: udpclibcast1
udpclibcast1: $(lib_objects) src/chap8/udpcli03.c dgclibcast1.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@ -DSERV_PORT=13

.PHONY: dgclibcast5.o
dgclibcast5.o: src/chap20/dgclibcast5.c
	$(CC) -c $(CFLAGS) $(LDFLAGS) $< -o $@

.PHONY: udpclibcast5
udpclibcast5: $(lib_objects) src/chap8/udpcli03.c dgclibcast5.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@ -DSERV_PORT=13

tidy:
	rm -rf *.o

clean:
	rm -f $(lib)/*.o *.o
	rm -f $(bin)
	rm -rf *.dSYM
