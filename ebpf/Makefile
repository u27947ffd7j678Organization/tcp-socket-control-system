APP := tcp_monitor

USER_SRC := tcp_monitor.c
BPF_SRC  := tcp_monitor.bpf.c

OUTPUT := .output

BPF_OBJ  := $(OUTPUT)/tcp_monitor.bpf.o
SKEL_HDR := $(OUTPUT)/tcp_monitor.skel.h
USER_OBJ := $(OUTPUT)/tcp_monitor.o

BOOTSTRAP_ROOT := $(HOME)/libbpf-bootstrap
BOOTSTRAP_OUT  := $(BOOTSTRAP_ROOT)/examples/c/.output

LIBBPF_DIR := $(BOOTSTRAP_OUT)/libbpf
LIBBPF_A   := $(LIBBPF_DIR)/libbpf.a
BPFTOOL    := $(BOOTSTRAP_OUT)/bpftool/bootstrap/bpftool

VMLINUX_DIR := $(BOOTSTRAP_ROOT)/vmlinux.h/include/x86

CLANG := clang
CC    := gcc

BPF_ARCH := x86

BPF_CFLAGS := -g -O2 \
	-target bpf \
	-D__TARGET_ARCH_$(BPF_ARCH) \
	-I$(VMLINUX_DIR) \
	-I$(BOOTSTRAP_OUT) \
	-I.

USER_CFLAGS := -g -O2 -Wall -Wextra \
	-I$(OUTPUT) \
	-I$(BOOTSTRAP_OUT) \
	-I.

LDLIBS := $(LIBBPF_A) -lelf -lz

.PHONY: all clean

all: $(APP)

$(BPF_OBJ): $(BPF_SRC) tcp_monitor.h
	@mkdir -p $(OUTPUT)
	$(CLANG) $(BPF_CFLAGS) -c $< -o $@

$(SKEL_HDR): $(BPF_OBJ)
	$(BPFTOOL) gen skeleton $< > $@

$(USER_OBJ): $(USER_SRC) tcp_monitor.h $(SKEL_HDR)
	$(CC) $(USER_CFLAGS) -c $(USER_SRC) -o $@

$(APP): $(USER_OBJ) $(LIBBPF_A)
	$(CC) $(USER_OBJ) $(LDLIBS) -o $@

clean:
	rm -rf $(OUTPUT) $(APP)
