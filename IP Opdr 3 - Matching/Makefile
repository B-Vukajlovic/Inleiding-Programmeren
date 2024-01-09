# Change this to change the compiler used
# CC = clang
CC = gcc

# Use C11, debugging and warnings flags
define CFLAGS
-std=gnu11 \
-g3 \
-O3 \
-fsanitize=address \
-Wpedantic \
-Wall \
-Wextra \
-Wformat=2 \
-Wsign-conversion \
-Wswitch-enum \
-Wno-switch-default \
-Wunused \
-Wfloat-equal \
-Wconversion \
-Wpointer-arith \
-Wtype-limits \
-Wcast-qual \
-Wenum-compare \
-Wsizeof-pointer-memaccess \
-Wstrict-prototypes
endef

# Turn on the address sanitizer
LDFLAGS = -fsanitize=address

# Flags needed for the check library
CHECK_LDFLAGS = $(LDFLAGS) `pkg-config --libs check`

TARGETS=altijd_steen random teller matching competitie

.PHONY: all clean tarball check

all: speel $(TARGETS)

valgrind: LDFLAGS=
valgrind: CFLAGS=-Wall
valgrind: $(PROG)

speel: speel.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(TARGETS): %: %.c

clean:
	rm -f *~ *.o speel $(TARGETS)

tarball: opdracht3_submit.tar.gz

opdracht3_submit.tar.gz: altijd_steen.c random.c teller.c matching.c speel.c second_guess.c competitie.c Makefile
	tar -czf $@ $^
