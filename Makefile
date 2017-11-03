include .deosrc

all: clean
	@-$(MAKE) run

run: build
	$(TARGET)

build: $(OBJECTS)
	$(CC) $(STD) $(CFLAGS) $(LIBS) $(HEADERS) $(MAIN) $(OBJECTS) -o $(TARGET)
	@-$(XMOD) $(TARGET)

clean:
	@-$(RM) $(TARGET) $(OBJECTS); clear

obj/deos/%.o:
	$(CC) $(STD) $(CFLAGS) $(LIBS) $(HEADERS) -c ./src/deos/$*.c -o ./obj/deos/$*.o

obj/deos/bitcoin/%.o:
	$(CC) $(STD) $(CFLAGS) $(LIBS) $(HEADERS) -c ./src/deos/bitcoin/$*.c -o ./obj/deos/bitcoin/$*.o

obj/deos/crypto/%.o:
	$(CC) $(STD) $(CFLAGS) $(LIBS) $(HEADERS) -c ./src/deos/crypto/$*.c -o ./obj/deos/crypto/$*.o

deos.atd:
	cd home/atd && $(MAKE)

deos.bower:
	bower update

deos.scripts: deos.scripts.clean
	curl -L $(CLOSUREURL) --output bin/compiler.jar
	curl -L $(RHINOURL) --output bin/js.jar

deos.scripts.clean:
	@-rm bin/js.jar bin/compiler.jar

deos.venv:
	-rm -rf venv
	virtualenv -p /usr/bin/python2.7 --always-copy --no-site-packages venv
	bash -c "source venv/bin/activate && pip install -r etc/python/requirements.txt"
