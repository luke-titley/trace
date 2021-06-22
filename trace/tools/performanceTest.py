#!/usr/bin/env python
#-------------------------------------------------------------------------------
# Copywrite Luke Titley 2015
#-------------------------------------------------------------------------------
# Super naive tool to help track performance improvments.
# This tries to more accurately measure performance improvements by profiling
# multiple times and taking the average result.
#
# It outputs the results into an easily parseable format.

import os
import subprocess
import sys
import time

render_command = ["./trace",
                  "--render",
                  "--outputFilename", "out.png",
                  "--qualityLevel", "2",
                  "--samplesPerPixel", "1",
                  "--threadCount", "4",
                  "--width", "256",
                  "--height", "256"]

number_of_tests = 3

print "performance_results.append({"
print "\t'id' : %d" % time.time()
print "\t'render_command' : %s," % repr(render_command)
total_elapsed_time = 0.0
for i in range(0,number_of_tests):
    sys.stderr.write("Running test %d of %d%s" % (i,
                                                  number_of_tests,
                                                  os.linesep))
    sys.stderr.flush()
    results = subprocess.check_output(render_command)
    for i in results.split(os.linesep):
        if "elapsed_time" in i:
            elapsed_time = float(i.split()[1]) # TODO LT: Use regex
            total_elapsed_time += elapsed_time

print "\t'average_elapsed_time' : %f," % (total_elapsed_time/float(number_of_tests))
print "})"
