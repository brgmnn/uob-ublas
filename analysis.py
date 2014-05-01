#!/usr/bin/env python
import numpy as np
import subprocess

#		Process manipulation
#------------------------------------------------------------------------------#
p = None

def start_ublas():
	global p
	p = subprocess.Popen("./build/run", bufsize=1024,
			stdout=subprocess.PIPE, stdin=subprocess.PIPE)
	p.stdin.write("10 1000 1000 1000\n")
	p.stdin.flush()

def close_ublas():
	global p
	p.stdin.write("none 0 0 0\n")
	p.stdin.flush()

def get_lib_time(lib, m, k, n):
	global p
	p.stdin.write(str(lib)+" "+str(m)+" "+str(k)+" "+str(n)+"\n")
	p.stdin.flush()
	return float(p.stdout.readline().strip())



if __name__ == "__main__":
	start_ublas()


	print "time for atlas",get_lib_time("atlas", 800, 800, 800)


	close_ublas()