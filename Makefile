#
#    Makefile for yasuna
#

TARGET	= ../example
MAKE	:= make
CC	:= cc
RM	:= rm
CFLAGS	:= -O2 -g -Wall
LDFLAGS	:=
CMDLINE	:= 0
export

all clean:
	@$(MAKE) -C ./src	$@

.PHONY:	all			\
	clean
