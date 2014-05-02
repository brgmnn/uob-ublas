#!/usr/bin/env python
import os, random, subprocess, threading
from decimal import *
import numpy as np

libs = ["atlas", "cublas", "mkl", "plasma"]

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
def make_train_file(testpoints, vals):
	with open("training.data", "w") as fp:
		fp.write(str(len(testpoints))+" 3 4\n\n")
		for tp,point in zip(testpoints,vals):
			out = ["-1" for x in range(4)]
			out[point.index(min(point))] = "1"

			fp.write(" ".join(map(str, tp))+"\n")
			fp.write(" ".join(out)+"\n")

#		Simulated Functions
#------------------------------------------------------------------------------#
def simfunc(lib, m, k, n):
	if lib == "atlas":
		return m*k*n
	elif lib == "cublas":
		return 1000000+0.1*m*k*n
	elif lib == "mkl":
		return 200+0.9*m*k*n
	elif lib == "plasma":
		return 2000+0.7*m*k*n

#		Analysis
#------------------------------------------------------------------------------#
def test_random(bound, samples):
		print "  Random Sampling"
		print "    range:  ",bound
		print "    samples:",samples

		# testpoints = [[random.randint(2,bound),random.randint(2,bound),
		# 	random.randint(2,bound)] for i in range(samples)]

		# testpoints = [[x,x,x] for x in range(8,500,2)]

		testpoints = [[m,k,n] for m in range(50,500,50) for k in range(50,500,50) for n in range(50,500,50)]

		print "num testpoints",len(testpoints)

		# with open("run-in", "w") as fp:
		# 	fp.write("8 550 550 550\n");
		# 	for tp in testpoints:
		# 		for l in libs:
		# 			fp.write(l+" "+" ".join(map(str, tp))+"\n")

		with open("run-out", "w") as fp:
			for m,k,n in testpoints:
				fp.write(str(simfunc("atlas",m,k,n))+" ")
				fp.write(str(simfunc("cublas",m,k,n))+" ")
				fp.write(str(simfunc("mkl",m,k,n))+" ")
				fp.write(str(simfunc("plasma",m,k,n))+"\n")

		with open("query-in", "w") as fp:
			for tp in testpoints:
				fp.write(" ".join(map(str, tp))+"\n")

		print "=== running commands ==="
		# os.system("./build/run < run-in > run-out")
		os.system("./build/query < query-in > query-out")
		print "========================"

		with open("query-out") as fp:
			selected = fp.read().split()

		with open("run-out") as fp:
			vals = map(lambda x: Decimal(x), fp.read().split())
			vals = zip(*[iter(vals)]*len(libs))
			vals = [[v for v in val] for val in vals]

		# make_train_file(testpoints, vals)

		# bads = [vals.index(p) for p in vals if selected[vals.index(p)] != libs.index(p.index(min(p)))]
		bads = [[tp,p,l] for tp,p,l in zip(testpoints,vals,selected) if l != libs[p.index(min(p))]]
		bads = [[tp, (libs[p.index(min(p))], min(p)), (l,p[libs.index(l)])] for tp,p,l in bads]

		for b in bads:
			print " ",b

		print len(bads),"/",len(selected),"wrong."




#		Sample UBLAS selection
#------------------------------------------------------------------------------#


if __name__ == "__main__":
	# with UblasRun() as run:
	# 	print "time for atlas",run.get_time("atlas", 800, 800, 800)

	# with UblasQuery() as query:
	# 	print "lib is",query.get_lib(400,400,400)

	test_random(500, 100)