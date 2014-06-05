#!/usr/bin/env python
import os, random, subprocess, sys, threading
from decimal import *
import numpy as np

libs = ["atlas", "cublas", "mkl", "plasma"]
# libs = ["cublas", "plasma", "ublas"]
prefix = 1000**3

#		Process manipulation
#------------------------------------------------------------------------------#
class UblasRun:
	def __init__(self):
		self.p = subprocess.Popen("./build/run", bufsize=102400,
				stdout=subprocess.PIPE, stdin=subprocess.PIPE)
		self.p.stdin.write("10 1000 1000 1000\n")
		self.p.stdin.flush()

	def __enter__(self):
		return self

	def __exit__(self, type, value, traceback):
		self.p.stdin.write("none 0 0 0\n")
		self.p.stdin.flush()

	def get_time(self, lib, m, k, n):
		self.p.stdin.write(str(lib)+" "+str(m)+" "+str(k)+" "+str(n)+"\n")
		self.p.stdin.flush()
		# return float(self.p.stdout.readline().strip())
		return self.p.stdout.readline().strip()

class UblasQuery:
	def __init__(self):
		self.p = subprocess.Popen("./build/query", bufsize=102400,
				stdout=subprocess.PIPE, stdin=subprocess.PIPE)

	def __enter__(self):
		return self

	def __exit__(self, type, value, traceback):
		self.p.stdin.write("0 0 0\n")
		self.p.stdin.flush()

	def get_lib(self, m, k, n):
		self.p.stdin.write(str(m)+" "+str(k)+" "+str(n)+"\n")
		self.p.stdin.flush()
		return self.p.stdout.readline().strip()


#		Training
#------------------------------------------------------------------------------#
def make_train_file(testpoints, vals, path="training.data"):
	with open(path, "w") as fp:
		fp.write(str(len(testpoints))+" 3 4\n\n")
		for tp,point in zip(testpoints,vals):
			out = ["-1" for x in range(4)]
			out[point.index(min(point))] = "1"

			fp.write(" ".join(map(str, tp))+"\n")
			fp.write(" ".join(out)+"\n")


#		Analysis
#------------------------------------------------------------------------------#
def test_random(bound, samples):
	print "  Random Sampling"
	print "    range:  ",bound
	print "    samples:",samples

	if len(sys.argv) > 1:
		logpoints = int(sys.argv[1])
	else:
		logpoints = 10

	testpoints = [[random.randint(2,bound),random.randint(2,bound),
		random.randint(2,bound)] for i in range(samples)]

	# testpoints = [[x,x,x] for x in range(8,512,2)]

	# dimpoints = range(100,1000,200)
	# dimpoints = [80]+[int(dp) for dp in np.logspace(2, 2.71, num=12)]
	# dimpoints = [int(dp) for dp in np.logspace(1.78, 2.71, num=logpoints)]
	# dimpoints = [int(dp) for dp in np.logspace(1.78, 3.0, num=logpoints)]
	# print dimpoints

	# testpoints = [[m,k,n] for m in dimpoints for k in dimpoints for n in dimpoints]

	numm = len(testpoints)
	print "num testpoints",numm
	# sys.exit(0)

	with open("run-in", "w") as fp:
		fp.write("8 "+str(bound)+" "+str(bound)+" "+str(bound)+"\n");
		for tp in testpoints:
			for l in libs:
				fp.write(l+" "+" ".join(map(str, tp))+"\n")

	with open("query-in", "w") as fp:
		for tp in testpoints:
			fp.write(" ".join(map(str, tp))+"\n")

	print "=== running commands ==="
	os.system("./build/run < run-in > run-out")
	os.system("./build/query < query-in > query-out")
	print "========================"

	with open("query-out") as fp:
		selected = fp.read().split()

	with open("run-out") as fp:
		vals = map(lambda x: Decimal(x), fp.read().split())
		vals = zip(*[iter(vals)]*len(libs))
		vals = [[v for v in val] for val in vals]

	# make_train_file(testpoints, vals, str(len(testpoints))+"-training.data")
	# sys.exit(0)

	optimals = [libs[p.index(min(p))] for p in vals]

	return sum([1 for i,j in zip(selected,optimals) if i==j])

	# ublas  = [v[libs.index(i)] for i,v in zip(selected,vals)]
	# atlas  = [i for i in optimals if i == "atlas"]
	# cublas = [i for i in optimals if i == "cublas"]
	# mkl    = [i for i in optimals if i == "mkl"]
	# plasma = [i for i in optimals if i == "plasma"]

	# print optimals
	# print ublas

	# print vals

	ctatlas  = sum([v[libs.index("atlas")]  for v in vals])
	ctcublas = sum([v[libs.index("cublas")] for v in vals])
	ctmkl    = sum([v[libs.index("mkl")]    for v in vals])
	ctplasma = sum([v[libs.index("plasma")] for v in vals])
	ctublas  = sum([v[libs.index("ublas")]  for v in vals])
	# ctublas  = sum(ublas)

	# print "c. time atlas  =",ctatlas
	# print "c. time cublas =",ctcublas
	# print "c. time mkl    =",ctmkl
	# print "c. time plasma =",ctplasma
	# print
	# print "c. time ublas  =",ctublas

	# print
	# print
	# print " atlas saving =",(ctatlas-ctublas)/ctatlas
	# print "cublas saving =",(ctcublas-ctublas)/ctcublas
	# print "   mkl saving =",(ctmkl-ctublas)/ctmkl
	# print "plasma saving =",(ctplasma-ctublas)/ctplasma

	# return (ctatlas-ctublas)/ctatlas, (ctcublas-ctublas)/ctcublas,\
	# 	(ctmkl-ctublas)/ctmkl, (ctplasma-ctublas)/ctplasma

	# print "ublas =",float(len(ublas))/samples
	# print "atlas =",float(len(atlas))/samples
	# print "cublas =",float(len(cublas))/samples
	# print "mkl =",float(len(mkl))/samples
	# print "plasma =",float(len(plasma))/samples

	# bads = [[tp,p,l] for tp,p,l in zip(testpoints,vals,selected) if l != libs[p.index(min(p))]]
	# bads = [[tp, (libs[p.index(min(p))], min(p)), (l,p[libs.index(l)])] for tp,p,l in bads]

	# for b in bads:
	# 	print " ",b

	# print len(bads),"/",len(selected),"wrong."


def test_speedup(trials=100):

	atlas, cublas, mkl, plasma = [], [], [], []

	for i in range(trials):
		print "="*50,"trial",(i+1),"/",trials
		a, c, m, p = test_random(512, 10)
		atlas.append(a)
		cublas.append(c)
		mkl.append(m)
		plasma.append(p)

	atlas = map(float, atlas)
	cublas = map(float, cublas)
	mkl = map(float, mkl)
	plasma = map(float, plasma)

	print " atlas saving =",np.mean(atlas),	"~",np.std(atlas)
	print "cublas saving =",np.mean(cublas),"~",np.std(cublas)
	print "   mkl saving =",np.mean(mkl),	"~",np.std(mkl)
	print "plasma saving =",np.mean(plasma),"~",np.std(plasma)

	with open("speedup", "w") as fp:
		fp.write("ATLAS,CuBLAS,MKL,PLASMA\n")
		for a,c,m,p in zip(atlas,cublas,mkl,plasma):
			fp.write(",".join(map(str, [a,c,m,p]))+"\n")


def test_training_sampling(trials=100):
	ss = 100
	accuarcy = []

	for i in range(trials):
		print "="*50,i+1,"/",trials
		accuarcy.append(float(test_random(512,ss))/ss)

	with open("training-samples-accuracy", "w") as fp:
		fp.write("\n".join(map(str, accuarcy)))

#		Sample UBLAS selection
#------------------------------------------------------------------------------#


if __name__ == "__main__":
	# with UblasRun() as run:
	# 	print "time for atlas",run.get_time("atlas", 800, 800, 800)

	# with UblasQuery() as query:
	# 	print "lib is",query.get_lib(400,400,400)o

	# atlas, cublas, mkl, plasma = test_random(512, 10)

	# print " atlas saving =",atlas
	# print "cublas saving =",cublas
	# print "   mkl saving =",mkl
	# print "plasma saving =",plasma

	print float(test_random(512, 100))/1
	# test_speedup()
	# test_training_sampling()