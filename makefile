all:
		cd build && qmake && make
clean:
		cd build && make clean
notify: all
run:
		build/logger
