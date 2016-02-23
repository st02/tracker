






btgatt-client:btgatt-client.c shared/libblhelper.a
	gcc btgatt-client.c -o $@ -L./shared/ -lblhelper -lbluetooth

shared/libblhelper.a: shared/*.o
	cd shared && $(MAKE)
clean:
	rm -rf btgatt-client
	cd shared && $(MAKE) clean    


