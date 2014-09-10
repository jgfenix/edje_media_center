#Makefile of edje_player*
all:
	edje_cc -v default.edc && gcc -o default default.c `pkg-config --libs --cflags elementary`
