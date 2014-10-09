all:
	edje_cc -v audiolist.edc && edje_cc -v default.edc && edje_cc -v main.edc && edje_cc -v settings.edc && edje_cc -v videolist.edc `pkg-config --libs --cflags Ecore Ecore_Evas Edje.h`
