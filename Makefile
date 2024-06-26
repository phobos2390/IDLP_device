all:
	@echo "test, lib, clean"

.PHONY: valgrind
.PHONY: test
.PHONY: coverage

test: lib 
	cd build && ./test_phobos2390_IDLP
	
valgrind: lib 
	cd build && valgrind ./test_phobos2390_IDLP

coverage: valgrind
	mkdir -p build/gcov
	cd build && find -name '*.gcda' -exec gcov -p {} \;
	cd build && find -name '*.gcov' -exec mv {} gcov \; &> /dev/null
	find -name '*.gcov' | sed -e 's/\.\/build\/gcov\///g; s/#/\//g' | grep -v $(shell pwd) | sed -e 's/\//#/g; s/^/.\/build\/gcov\//g' | xargs -n 1 rm -rf 	
	find -name '*.gcov' | grep catch2 | xargs -n 1 rm -rf 	
	cd build/gcov && find -name '*.gcov' | xargs -n 1 -I@ echo @ '####@' | sed -e 's/####.*src#//g' | xargs -n 2 mv

artifact: coverage
	mkdir -p artifact/lib artifact/test artifact/app
	cp -rf build/include artifact/
	cp -rf build/test_phobos2390_IDLP artifact/test
	cp -rf build/*.so artifact/lib
	cp -rf build/app artifact/
	cp -rf build/gcov artifact/gcov
	mkdir -p artifact/phobos2390_IDLP_device
	mv artifact/lib artifact/phobos2390_IDLP_device
	mv artifact/test artifact/phobos2390_IDLP_device
	mv artifact/app artifact/phobos2390_IDLP_device
	mv artifact/include artifact/phobos2390_IDLP_device
	tar -czvf artifact/phobos2390_IDLP_device.tar.gz artifact/phobos2390_IDLP_device
	tar -czvf artifact/phobos2390_IDLP_device_gcov.tar.gz artifact/gcov
	rm -rf artifact/phobos2390_IDLP_device artifact/gcov

lib:
	@mkdir -p build
	@cd build; cmake ..
	@cd build; make

clean:
	@rm -rf build artifact
