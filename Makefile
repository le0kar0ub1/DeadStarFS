 #
# Simple Makefile to compile our mkfs sources
 #

BUILD		:=	build

CCEXT		:=	gcc

OBJEXT		:=	.o

CCEXT		:=	.c

BINARY		:=	mkfs

LNKFLAGS	=	-o $(BINARY)				\

SRCDIR		:=	src									\
				$(addprefix src/formats/,			\
								ext2				\
				)

INCDIR		=	inc

CCFLAGS		=	-I $(INCDIR)						\
				-Wall								\
				-Wextra				 				\
				-Wnested-externs					\
				-Winline							\
				-Wpragmas							\
				-Wuninitialized						\
				-Wno-missing-braces					\
				-Wcast-align						\
				-Wwrite-strings						\
				-Wparentheses						\
				-Wunreachable-code					\
				-Wunused							\
				-Wmissing-field-initializers		\
				-Wswitch-enum						\
				-Wshadow				 			\
				-Wuninitialized				 		\
				-Wmissing-declarations				\
				-Wmissing-prototypes				\
				-Wstrict-prototypes					\
				-Wpointer-arith						\
				-Wno-override-init					\

SOURCES		:=	$(wildcard $(addsuffix /*$(CCEXT), $(SRCDIR)))

OBJECTS		:=	$(patsubst %$(CCEXT), $(BUILD)/%$(OBJEXT), $(SOURCES))

all:	$(BINARY)

clean:
	@rm -rf	$(BUILD)

fclean:	clean
	@rm -f	$(BINARY)

re:	fclean	all

$(BINARY):	$(OBJECTS)
	@$(CC) $(LNKFLAGS) $(OBJECTS)
	@-echo "   LNK    $(BINARY)"

$(BUILD)/%$(OBJEXT): %$(CCEXT)
	@mkdir -p $(shell dirname $@)
	@$(CC) $(CCFLAGS) -c $< -o $@
	@-echo "    CC    $@"