CC=gcc
CFLAGS=-I./ -I/usr/include/bluetooth


LIBS=-lbluetooth

OBJ = mainloop.o gatt-client.o gatt-helpers.o gatt-db.o att.o crypto.o io-mainloop.o uuid_new.o queue.o util.o timeout-mainloop.o


%.o: %.c 
	$(CC) -c -o $@ $< $(CFLAGS)

libblhelper.a: $(OBJ)
	ar r $@ $^ 
	ranlib $@

.PHONY: clean

clean:
	rm -f *.o *~ core *.a 

