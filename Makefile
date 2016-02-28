tracker:shared/libblhelper.a acq.c acq.h
	gcc -std=gnu99 acq.c -o $@ -L./shared/ -lblhelper -lbluetooth

shared/libblhelper.a: 
	cd shared && $(MAKE)
clean:
	rm -rf tracker
	cd shared && $(MAKE) clean    


