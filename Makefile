# BINMAN MAKEFILE - LINUX
# Note: tabs not spaces. E.g. if in vim, check your tab expand:
# 	:set expandtab?		=> noexpandtab

TARGET  := binman  # Final target name

INCDIR 	:= inc
SRCDIR	:= src
OBJDIR 	:= obj
CC 		:= gcc -c
LD		:= gcc -o
CFLAGS	:= -I$(INCDIR) -Wall -g3 # Use incdir, all warnings, add debug info
LDFLAGS	:= -g3
SRCS	:= $(wildcard $(SRCDIR)/*.c)
OBJS	:= $(patsubst %.c, $(OBJDIR)/%.o, $(SRCS))

# Header dependencies
DEPS	:= $(wildcard $(INCDIR)/*.h)

# Quickref
# $^	prereq list
# S@	target
# S<	first prereq
# S?	out of date dependencies
# S+	all dependencies
# $|	'order only' prereqs

all: $(TARGET) # Default rule runs when TARGET is out of date

$(TARGET) : $(OBJS)
	$(LD) $(LDFLAGS) -o $(TARGET) $^

$(OBJDIR)/$(SRCDIR)/%.o : $(SRCDIR)/%.c $(DEPS)
	$(CC) $(CFLAGS) -o $@ $<

$(OBJS): | $(OBJDIR)/$(SRCDIR) # Make objdir first, no out-of-date check

$(OBJDIR)/$(SRCDIR): | $(OBJDIR)
	mkdir $@

$(OBJDIR):
	mkdir $@

.PHONY : clean # in case there is ever a file called 'clean'
# remove object files, keep going if error
clean:
	-rm -f $(OBJDIR)/$(SRCDIR)/*.o 
	-rm -f binman


