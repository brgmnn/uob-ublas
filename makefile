cc		= gcc
inc		= 
cflags	= $(inc) -std=c99 -g -Wfatal-errors -Wall -Wextra
clibs	= -lcblas

src = ublas.c

src_ut = unittests.c

src_dir		= src
src_ut_dir	= src/unittests
obj_dir		= obj
obj			= $(addprefix obj/, $(addsuffix .o,$(basename $(src))))
obj_ut 		= $(addprefix obj/unittests/, $(addsuffix .o,$(basename $(src_ut))))
obj_all		= $(obj) $(obj_ut)


all: build-source link-unittests pack-library

build-source: $(obj_all)

link-unittests: $(obj_ut)
	$(cc) $(obj_ut) -o unittests $(clibs)

pack-library:
	ar -r libublas.a $(obj)

$(obj_dir)/%.o: $(src_dir)/%.c 
	$(cc) -c $(cflags) -o $@ $<

clean:
	rm -f $(obj)