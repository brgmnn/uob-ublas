cc		= gcc
fc		= gfortran

inc		= -I./src -I../include/plasma
# clibs	= -L../lib/plasma -lm -lpthread -lrefblas -lcblas -lplasma -lcoreblas -lcoreblasqw -ltmg -llapack -llapacke -lquark 
clibs	= -L../lib/plasma -lplasma -lcoreblasqw -lcoreblas -lplasma -lquark -lcblas -llapacke -ltmg -llapack -lrefblas -lpthread -lm
ubopt	= -D WITH_ATLAS


# cflags	= $(inc) $(ubopt) -std=c99 -g -Wfatal-errors -Wall -Wextra
cflags	= $(inc) $(ubopt) -std=c99

src = ublas.c
src_ut = unittests.c

src_dir		= src
src_ut_dir	= src/unittests
obj_dir		= obj
obj			= $(addprefix obj/, $(addsuffix .o,$(basename $(src))))
obj_ut 		= $(addprefix obj/unittests/, $(addsuffix .o,$(basename $(src_ut))))
obj_all		= $(obj) $(obj_ut)


all: build-source link-unittests pack-library
# all: build-source

build-source: $(obj_all)

link-unittests: $(obj_ut)
	$(fc) $(obj_ut) -o unittests $(clibs)

pack-library:
	ar -r libublas.a $(obj)

$(obj_dir)/%.o: $(src_dir)/%.cpp
	$(cc) -c $(cflags) -o $@ $<

clean:
	rm -f $(obj_all)