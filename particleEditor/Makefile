CMAKE_BUILD_TYPE=Debug

LINUX:
	mkdir -p build/Linux && cd build/Linux && \
	cmake -DCMAKE_BUILD_TYPE=$(CMAKE_BUILD_TYPE) ../../ && \
	make -j 32 && \
	cd ../../

CLEAN:
	rm -rf ./build

URHO:
	cd ../urho/script && ./cmake_generic.sh ../build && cd ../build && make && cd ../../
