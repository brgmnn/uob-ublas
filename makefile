cc		= gcc
fc		= gfortran

inc		= -I./src -I./../include/simple-profiler
clibs	= -L./../lib/simple-profiler -lsimpleprofiler
cflags	= $(inc) -std=c99
# cflags	= $(inc) $(ubopt) -std=c99 -g -Wfatal-errors -Wall -Wextra

# clibs	= -L../lib/plasma -lm -lpthread -lrefblas -lcblas -lplasma -lcoreblas -lcoreblasqw -ltmg -llapack -llapacke -lquark 
# clibs	= -L../lib/plasma -lplasma -lcoreblasqw -lcoreblas -lplasma -lquark -lcblas -llapacke -ltmg -llapack -lrefblas -lpthread -lm

#---------------------------------------------------------------------------------------------------
# ATLAS configuration
atlas_flags	= -D WITH_ATLAS
# atlas_flags	= 
atlas_libs	= -lcblas
cflags += $(atlas_flags)
clibs += $(atlas_libs)


# PLASMA configuration
plasma_flags	= -D WITH_PLASMA -I../include/plasma
# plasma_flags	= 
plasma_libs		= -L../lib/plasma -lplasma -lcoreblasqw -lcoreblas -lplasma -lquark -lcblas \
	-llapacke -ltmg -llapack -lrefblas -lpthread -lm
cflags += $(plasma_flags)
clibs += $(plasma_libs)

#---------------------------------------------------------------------------------------------------

# cflags	= $(inc) -std=c99
# clibs	= $(clibs_atlas) $(clibs_plasma)

src		= ublas.c ublas_lib_cblas.c ublas_lib_plasma.c
src_t	= accuracy.c measure.c
src_ut	= unittests.c

src_dir		= src
src_t_dir	= src/tests
src_ut_dir	= src/unittests
obj_dir		= obj
obj			= $(addprefix obj/, $(addsuffix .o,$(basename $(src))))
obj_t 		= $(addprefix obj/tests/, $(addsuffix .o,$(basename $(src_t))))
obj_ut 		= $(addprefix obj/unittests/, $(addsuffix .o,$(basename $(src_ut))))
obj_all		= $(obj) $(obj_t) $(obj_ut)


all: build-source link-tests-accuracy link-unittests pack-library
# all: build-source

build-source: $(obj_all)

link-tests-accuracy: $(obj_t) $(obj)
	$(fc) $(obj_t) $(obj) -o accuracy $(clibs)

link-unittests: $(obj_ut)
	$(fc) $(obj_ut) -o unittests $(clibs)

pack-library:
	ar -r libublas.a $(obj)

$(obj_dir)/%.o: $(src_dir)/%.c
	$(cc) -c $(cflags) -o $@ $<

clean:
	rm -f $(obj_all)