all:
	@echo "test, lib, clean"

test: lib 
	build/test_phobos2390_IDLP
	
valgrind: lib 
	valgrind build/test_phobos2390_IDLP

artifact: lib 
	mkdir -p artifact/lib artifact/test artifact/apps
	cp -rf build/include artifact/
	cp -rf build/test_phobos2390_IDLP artifact/test
	cp -rf build/*.so artifact/lib
	cp -rf build/apps artifact/
	mkdir -p artifact/phobos2390_IDLP_device
	mv artifact/lib artifact/phobos2390_IDLP_device
	mv artifact/test artifact/phobos2390_IDLP_device
	mv artifact/apps artifact/phobos2390_IDLP_device
	mv artifact/include artifact/phobos2390_IDLP_device
	tar -czvf artifact/phobos2390_IDLP_device.tar.gz artifact/phobos2390_IDLP_device
	rm -rf artifact/phobos2390_IDLP_device

lib:
	@mkdir -p build
	@cd build; cmake ..
	@cd build; make

clean:
	@rm -rf build artifact
