# ------------------------------------------------------------------------------
# Copywrite Luke Titley 2015
# ------------------------------------------------------------------------------
all: render

#CC=clang++
#CC_LINK=clang++
CC=g++
CC_LINK=g++

# ------------------------------------------------------------------------------
# Build Configuration
# ------------------------------------------------------------------------------
include configuration.mk

# ------------------------------------------------------------------------------
# Intermediate directories
# ------------------------------------------------------------------------------
# We create the intermediate directories needed when building but we also create
# an empty 'stub' file which we use to know if the directory exists or not.
# We can't specify the directory directly as the modification time of the
# directory changes with the modification of its contents. The stub file remains
# unmodified after the directory is created.
# 
objects/stub:
	mkdir ./objects
	touch objects/stub

lib/stub:
	mkdir ./lib
	touch lib/stub

images/stub:
	mkdir ./images
	touch images/stub

docs/stub:
	mkdir ./docs
	touch docs/stub

# ------------------------------------------------------------------------------
# Header files dependencies
# ------------------------------------------------------------------------------
include/trace/vector.h: include/trace/log.h\
						include/trace/test.h\
						include/trace/vector_impl.h\
						include/trace/vector_impl_sse.h
include/trace/array.h: include/trace/assert.h\
				   	   include/trace/log.h\
					   include/trace/test.h
include/trace/bounds.h: include/trace/assert.h\
					    include/trace/log.h\
					    include/trace/ray.h\
					    include/trace/test.h\
					    include/trace/vector.h
include/trace/constvector.h: include/trace/assert.h\
							 include/trace/log.h
include/trace/shadestack.h: include/trace/constvector.h\
							include/trace/traceResult.h
include/trace/supersampleiterator.h: include/trace/vector.h
include/trace/geoAPI.h: include/trace/traceResult.h\
					 include/trace/triangleCache.h
include/trace/solidangle.h: include/trace/log.h\
									 include/trace/matrix.h\
									 include/trace/random.h\
									 include/trace/test.h\
									 include/trace/vector.h
include/trace/intersect.h: include/trace/vector.h
include/trace/radiance.h: include/trace/sampledspectrum.h\
						  include/trace/traceResult.h\
						  include/trace/vector.h
include/trace/sampledspectrum.h: include/trace/vector.h
include/trace/kdtree.h: include/trace/bounds.h\
						include/trace/constvector.h\
						include/trace/intersect.h\
						include/trace/ray.h\
						include/trace/test.h\
						include/trace/tree.h\
						include/trace/vector.h
include/trace/linearPixelIterator.h: include/trace/pixelIterator.h
include/trace/matrix.h: include/trace/vector.h
include/trace/objiterator.h: include/trace/triangleIterator.h\
							 include/trace/vector.h
include/trace/lsditerator.h: lsd/include/lsd/lsd.h\
							 include/trace/bounds.h\
							 include/trace/objectiterator.h\
							 include/trace/triangleIterator.h
include/trace/pixelIterator.h: include/trace/vector.h
include/trace/pngwriter.h: include/trace/array.h
include/trace/ray.h: include/trace/vector.h
include/trace/recursivePixelIterator.h: include/trace/pixelIterator.h\
										include/trace/recursiveSequenceIterator.h
include/trace/recursiveSequenceIterator.h: include/trace/constvector.h\
										   include/trace/vector.h
include/trace/renderer.h: include/trace/array.h\
						  include/trace/radiance.h\
						  include/trace/kdtree.h\
						  include/trace/log.h\
						  include/trace/renderThreads.h\
						  include/trace/time.h\
						  include/trace/triangle.h
include/trace/renderThreads.h: include/trace/array.h\
						       include/trace/image.h\
							   include/trace/thread.h\
							   include/trace/vector.h
include/trace/shade.h: include/trace/vector.h
include/trace/shader.h: include/trace/vector.h
include/trace/shadersDiffuse.h: include/trace/shader.h\
								include/trace/sampledspectrum.h
include/trace/shadersWhiteLight.h: include/trace/shader.h\
								include/trace/sampledspectrum.h
include/trace/simpleScene.h: include/trace/constvector.h\
							 include/trace/geoAPI.h\
							 include/trace/traceResult.h\
							 include/trace/triangle.h\
							 include/trace/triangleCache.h\
							 include/trace/triangleIterator.h
include/trace/sphere.h: include/trace/bounds.h\
						include/trace/vector.h
include/trace/traceResult.h: include/trace/vector.h
include/trace/tree.h: include/trace/int.h
include/trace/triangleCache.h: include/trace/kdtree.h
include/trace/triangle.h: include/trace/bounds.h\
					      include/trace/constvector.h\
						  include/trace/vector.h
include/trace/triangleIterator.h: include/trace/triangle.h
include/trace/vector.h: include/trace/log.h\
						include/trace/test.h

# ------------------------------------------------------------------------------
# Source files
# ------------------------------------------------------------------------------
objects/intersect.o: src/intersect.cpp\
					 include/trace/intersect.h\
					 include/trace/bounds.h\
					 include/trace/ray.h\
					 include/trace/triangle.h\
					 objects/stub
	$(CC) $(CONFIGURATION) -c -fPIC -I./include/ src/intersect.cpp\
				   -o objects/intersect.o

objects/kdtree.o: src/kdtree.cpp\
			   include/trace/kdtree.h\
			   objects/stub
	$(CC) $(CONFIGURATION) -c -fPIC -I./include/ src/kdtree.cpp -o\
					objects/kdtree.o

objects/linearPixelIterator.o: src/linearPixelIterator.cpp\
							   include/trace/linearPixelIterator.h\
							   objects/stub
	$(CC) $(CONFIGURATION) -c -fPIC -I./include/ src/linearPixelIterator.cpp\
				   -o objects/linearPixelIterator.o

objects/log.o: src/log.cpp\
			   include/trace/log.h\
			   objects/stub
	$(CC) $(CONFIGURATION) -c -fPIC -I./include/ src/log.cpp -o objects/log.o

objects/objiterator.o: src/objiterator.cpp\
					   include/trace/objiterator.h\
					   include/pystring.h\
					   objects/stub
	$(CC) $(CONFIGURATION) -c -fPIC -I./include/ src/objiterator.cpp -o objects/objiterator.o

#objects/lsditerator.o: src/lsditerator.cpp\
#					   include/trace/assert.h\
#					   include/trace/lsditerator.h\
#					   lsd/include/lsd/lsdParser.h\
#					   objects/stub
#	$(CC) $(CONFIGURATION) -c -fPIC -I./include/ -I./lsd/include src/lsditerator.cpp -o objects/lsditerator.o

objects/pngwriter.o: src/pngwriter.cpp\
					 include/trace/pngwriter.h\
					 objects/stub
	$(CC) $(CONFIGURATION) -c -fPIC -I./libpng -I./include/ src/pngwriter.cpp -o objects/pngwriter.o

objects/pystring.o: include/pystring.h\
					src/pystring.cpp
	$(CC) $(CONFIGURATION) -c -fPIC -I./include/ src/pystring.cpp -o objects/pystring.o

objects/random.o: src/random.cpp\
					 include/trace/random.h\
					 objects/stub
	$(CC) $(CONFIGURATION) -c -fPIC -I./include/ src/random.cpp -o objects/random.o

objects/recursivePixelIterator.o: src/recursivePixelIterator.cpp\
							   include/trace/recursivePixelIterator.h\
							   objects/stub
	$(CC) $(CONFIGURATION) -c -fPIC -I./include/ src/recursivePixelIterator.cpp\
				   -o objects/recursivePixelIterator.o

objects/renderer.o: src/renderer.cpp\
				 include/trace/renderer.h\
				 objects/stub
	$(CC) $(CONFIGURATION) -c -fPIC -I./include/ src/renderer.cpp -o objects/renderer.o

objects/renderThreads.o: src/renderThreads.cpp\
			 include/trace/renderThreads.h\
		     include/trace/triangleCache.h\
			 include/trace/shadestack.h\
			 include/trace/pixelIterator.h\
			 include/trace/shade.h\
			 include/trace/supersampleiterator.h\
			 objects/stub
	$(CC) $(CONFIGURATION) -c -fPIC -I./include/ src/renderThreads.cpp -o objects/renderThreads.o

objects/shade.o: src/shade.cpp\
			 include/trace/geoAPI.h\
			 include/trace/geoid.h\
			 include/trace/radiance.h\
			 include/trace/matrix.h\
			 include/trace/random.h\
			 include/trace/shader.h\
			 include/trace/shade.h\
			 include/trace/shadeAPI.h\
			 include/trace/shadestack.h\
			 include/trace/surfaceframe.h\
			 include/trace/solidangle.h\
			 objects/stub
	$(CC) $(CONFIGURATION) -c -fPIC -I./include/ src/shade.cpp -o objects/shade.o

objects/shadersDiffuse.o: src/shadersDiffuse.cpp\
						  include/trace/shadersDiffuse.h\
						  include/trace/geoAPI.h\
						  include/trace/shadeAPI.h\
						  include/trace/geoid.h\
						  include/trace/radiance.h\
						  include/trace/sampledspectrum.h\
					 objects/stub
	$(CC) $(CONFIGURATION) -c -fPIC -I./include/ src/shadersDiffuse.cpp\
				   -o objects/shadersDiffuse.o

objects/shadersWhiteLight.o: src/shadersWhiteLight.cpp\
						  include/trace/shadersWhiteLight.h\
						  include/trace/geoAPI.h\
						  include/trace/shadeAPI.h\
						  include/trace/geoid.h\
						  include/trace/radiance.h\
						  include/trace/sampledspectrum.h\
					 objects/stub
	$(CC) $(CONFIGURATION) -c -fPIC -I./include/ src/shadersWhiteLight.cpp\
				   -o objects/shadersWhiteLight.o

objects/simpleScene.o: src/simpleScene.cpp\
					 include/trace/simpleScene.h\
					 include/trace/assert.h\
					 include/trace/geoid.h\
					 include/trace/intersect.h\
					 include/trace/vector.h\
					 include/trace/shadersDiffuse.h\
					 include/trace/shadersWhiteLight.h\
					 objects/stub
	$(CC) $(CONFIGURATION) -c -fPIC -I./include/ src/simpleScene.cpp -o objects/simpleScene.o

objects/solidangle.o: src/solidangle.cpp\
							   include/trace/solidangle.h\
							   objects/stub
	$(CC) $(CONFIGURATION) -c -fPIC -I./include/ src/solidangle.cpp\
				   -o objects/solidangle.o


objects/thread.o: src/thread.cpp include/trace/thread.h objects/stub
	$(CC) $(CONFIGURATION) -c -fPIC -I./include/ src/thread.cpp -o\
												 objects/thread.o

# ------------------------------------------------------------------------------
# Test Files
# ------------------------------------------------------------------------------
objects/test.o: src/test.cpp\
				include/trace/array.h\
				include/trace/bounds.h\
				include/trace/kdtree.h\
				include/trace/log.h\
				include/trace/solidangle.h\
				include/trace/tree.h\
				include/trace/test.h\
				include/trace/vector.h\
				objects/stub
	$(CC) $(CONFIGURATION) -c -fPIC -I./include/ src/test.cpp -o objects/test.o

objects/test_array.o: src/test/test_array.cpp\
						include/trace/log.h\
						include/trace/array.h\
						include/trace/test.h\
						objects/stub
	$(CC) $(CONFIGURATION) -c -fPIC -I./include/ src/test/test_array.cpp\
				  -o objects/test_array.o

objects/test_bounds.o: src/test/test_bounds.cpp\
						include/trace/log.h\
						include/trace/bounds.h\
						include/trace/test.h\
						objects/stub
	$(CC) $(CONFIGURATION) -c -fPIC -I./include/ src/test/test_bounds.cpp\
				  -o objects/test_bounds.o

objects/test_constvector.o: src/test/test_constvector.cpp\
						include/trace/log.h\
						include/trace/constvector.h\
						include/trace/test.h\
						objects/stub
	$(CC) $(CONFIGURATION) -c -fPIC -I./include/ src/test/test_constvector.cpp\
				  -o objects/test_constvector.o

objects/test_intersect.o: src/test/test_intersect.cpp\
						include/trace/bounds.h\
						include/trace/log.h\
						include/trace/intersect.h\
						include/trace/ray.h\
						include/trace/test.h\
						include/trace/triangle.h\
						objects/stub
	$(CC) $(CONFIGURATION) -c -fPIC -I./include/ src/test/test_intersect.cpp\
				  -o objects/test_intersect.o

objects/test_kdtree.o: src/test/test_kdtree.cpp\
						include/trace/log.h\
						include/trace/kdtree.h\
						include/trace/test.h\
						objects/stub
	$(CC) $(CONFIGURATION) -c -fPIC -I./include/ src/test/test_kdtree.cpp\
				  -o objects/test_kdtree.o

objects/test_solidangle.o: src/test/test_solidangle.cpp\
						include/trace/log.h\
						include/trace/solidangle.h\
						include/trace/test.h\
						objects/stub
	$(CC) $(CONFIGURATION) -c -fPIC -I./include/ src/test/test_solidangle.cpp\
				  -o objects/test_solidangle.o

objects/test_tree.o: src/test/test_tree.cpp\
						include/trace/log.h\
						include/trace/tree.h\
						include/trace/test.h\
						objects/stub
	$(CC) $(CONFIGURATION) -c -fPIC -I./include/ src/test/test_tree.cpp\
				  -o objects/test_tree.o

objects/test_vector.o: src/test/test_vector.cpp\
						include/trace/log.h\
						include/trace/vector.h\
						include/trace/test.h\
						objects/stub
	$(CC) $(CONFIGURATION) -c -fPIC -I./include/ src/test/test_vector.cpp\
				  -o objects/test_vector.o

# ------------------------------------------------------------------------------
# Qt Project for QTrace
# ------------------------------------------------------------------------------
qtproject:
	find . -name "*.cpp" > trace.files
	find . -name "*.cc" >> trace.files
	find . -name "*.cpp" >> trace.files
	find . -name "*.h" >> trace.files
	find . -name "*.h" > trace.includes

# ------------------------------------------------------------------------------
# Docs
# ------------------------------------------------------------------------------
docs/html/index.html: docs/stub doxygen/Doxyfile\
					  src/test/*.cpp\
					  include/trace/*.h
	make -C ./doxygen

docs: docs/html/index.html

# ------------------------------------------------------------------------------
# Shared libraries
# ------------------------------------------------------------------------------

lib/libpng.so: libpng/*.c
	make --directory=./libpng

#lib/liblsd.so: lsd/src/*.cc lsd/include/lsd/*.h lsd/grammar
#	make --directory=./lsd
#	cp lsd/lib/liblsd.so lib/liblsd.so

# libtraceshaders
lib/libtraceshaders.so: objects/shadersDiffuse.o\
						objects/shadersWhiteLight.o\
						lib/stub
	$(CC_LINK) $(CONFIGURATION) -shared\
						objects/shadersDiffuse.o\
						objects/shadersWhiteLight.o\
						-o lib/libtraceshaders.so 

# libthread
lib/libthread.so: objects/thread.o lib/stub
	$(CC_LINK) $(CONFIGURATION) -shared objects/thread.o -o\
										lib/libthread.so 

# libtracetest
lib/libtracetest.so: objects/test.o\
				     objects/test_array.o\
				     objects/test_bounds.o\
				     objects/test_constvector.o\
				     objects/test_intersect.o\
				     objects/test_kdtree.o\
					 objects/test_solidangle.o\
					 objects/test_tree.o\
					 objects/test_vector.o\
					 lib/stub
	$(CC_LINK) $(CONFIGURATION) -shared\
						objects/test.o\
						objects/test_array.o\
						objects/test_bounds.o\
						objects/test_constvector.o\
						objects/test_intersect.o\
						objects/test_kdtree.o\
						objects/test_solidangle.o\
					 	objects/test_tree.o\
						objects/test_vector.o\
						-o lib/libtracetest.so

#  libtrace
lib/libtrace.so: objects/intersect.o\
				 objects/kdtree.o\
				 objects/linearPixelIterator.o\
				 objects/log.o\
				 objects/objiterator.o\
				 objects/pngwriter.o\
				 objects/pystring.o\
				 objects/random.o\
				 objects/recursivePixelIterator.o\
				 objects/renderer.o\
				 objects/renderThreads.o\
				 objects/shade.o\
				 objects/solidangle.o\
				 objects/simpleScene.o\
				 Makefile\
				 lib/stub
	$(CC_LINK) $(CONFIGURATION) -shared\
					objects/intersect.o\
					objects/kdtree.o\
					objects/linearPixelIterator.o\
					objects/log.o\
					objects/objiterator.o\
					objects/pngwriter.o\
					objects/pystring.o\
					objects/random.o\
					objects/recursivePixelIterator.o\
					objects/renderer.o\
					objects/renderThreads.o\
					objects/shade.o\
					objects/solidangle.o\
					objects/simpleScene.o\
				   -o lib/libtrace.so 

# ------------------------------------------------------------------------------
# main
# ------------------------------------------------------------------------------
trace_includes=-I./include -I./lsd/include 
./trace: lib/libtrace.so\
	     lib/libtracetest.so\
		 lib/libthread.so\
		 lib/libtraceshaders.so\
		 lib/libpng.so\
		 main.cpp\
		 include/trace/image.h\
		 include/trace/objiterator.h\
		 include/trace/lsditerator.h
	$(CC) $(CONFIGURATION) -L./ -L./lib $(trace_includes) main.cpp\
		-ltrace\
		-lthread\
		-ltraceshaders\
		-ltracetest\
		-lpng\
		-lpthread\
		-lz\
		-Wl,-rpath='$$ORIGIN/lib' -o trace

# ------------------------------------------------------------------------------
# analyze
# ------------------------------------------------------------------------------
analyze:
	clang-check -analyze main.cpp src/*.cpp -- $(trace_includes) 
	rm *.plist

# ------------------------------------------------------------------------------
# detect_memory_leaks
# ------------------------------------------------------------------------------
maxRayDepth=2
qualityLevel=1
samplesPerPixel=1
detect_memory_leaks: ./trace
	valgrind ./trace --render\
		--inputFilename "in.obj"\
		--outputFilename "out.png"\
		--maxRayDepth ${maxRayDepth}\
		--samplesPerPixel ${samplesPerPixel}\
		--qualityLevel ${qualityLevel}\
	    --width 256 \
		--height 256 \
		--reportProgress

# ------------------------------------------------------------------------------
# profile
# ------------------------------------------------------------------------------
maxRayDepth=2
qualityLevel=1
samplesPerPixel=1
profile: ./trace
	valgrind --tool=callgrind\
		./trace --render\
		--inputFilename "in.obj"\
		--outputFilename "out.png"\
		--maxRayDepth ${maxRayDepth}\
		--samplesPerPixel ${samplesPerPixel}\
		--qualityLevel ${qualityLevel}\
	    --width 256 \
		--height 256 \
		--reportProgress

# ------------------------------------------------------------------------------
# Tests
# ------------------------------------------------------------------------------
tests: ./trace
	./trace --runUnitTests

# ------------------------------------------------------------------------------
# Render
# ------------------------------------------------------------------------------
maxRayDepth=2
qualityLevel=1
samplesPerPixel=1
out.png: ./trace images/stub
	./trace --render\
		--inputFilename "in.obj"\
		--outputFilename "out.png"\
		--maxRayDepth ${maxRayDepth}\
		--samplesPerPixel ${samplesPerPixel}\
		--qualityLevel ${qualityLevel}\
	    --width 256 \
		--height 256 \
		--reportProgress

render: out.png tests

# ------------------------------------------------------------------------------
# Performance
# ------------------------------------------------------------------------------
notes/performance_results.txt: ./trace tools/performanceTest.py
	tools/performanceTest.py >> notes/performance_results.txt
	cat notes/performance_results.txt

performance: notes/performance_results.txt

# ------------------------------------------------------------------------------
# Cleanup 
# ------------------------------------------------------------------------------
clean:
	rm -rf objects
	rm -rf lib
	rm -rf images
	rm -rf docs
	rm trace
	make --directory=./lsd clean

# ------------------------------------------------------------------------------
#  Licenses - pystring
# ------------------------------------------------------------------------------
#######################################/
# Copyright (c) 2008-2010, Sony Pictures Imageworks Inc
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
# Redistributions of source code must retain the above copyright notice,
# this list of conditions and the following disclaimer.
# Redistributions in binary form must reproduce the above copyright
# notice, this list of conditions and the following disclaimer in the
# documentation and/or other materials provided with the distribution.
# Neither the name of the organization Sony Pictures Imageworks nor the
# names of its contributors
# may be used to endorse or promote products derived from this software
# without specific prior written permission.
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS
# IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
# TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
# PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER
# OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
# PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
# LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#######################################/

