CXXFLAGS += -I../include -fPIC\
			-I/usr/include/php5/main -I/usr/include/php5/Zend \
			-I/usr/include/php5/TSRM -I/usr/include/php5 \
			-O2 -DLINK_PHP_EXT
			
CFLAGS += -I../include -fPIC\
			-I/usr/include/php5/main -I/usr/include/php5/Zend \
			-I/usr/include/php5/TSRM -I/usr/include/php5 \
			-O2 -DLINK_PHP_EXT
			
OBJECTS = export_entry.o				\
					iflytek_funcs_ita.o	 \
					iflytek_funcs_tts.o

./iflytek_funcs.so: $(OBJECTS)
	g++ -fPIC -shared -rdynamic $(OBJECTS) -L../bin/x64/ -lmsc -ldl -lpthread -lrt -o ./iflytek_funcs.so
	
.PHONY: clean
clean:
	-rm $(OBJECTS)
	
.PHONY: all
all:
	make clean
	make
