export MAKEFLAGS=--no-print-directory

.DEFAULT_GOAL:=all
.PHONY:all clean

RHINOURL:=https://github.com/requirejs/r.js/blob/master/lib/rhino/js.jar?raw=true
CLOSUREURL:=https://github.com/requirejs/r.js/blob/master/lib/closure/compiler.jar?raw=true

all: clean
	curl -L $(RHINOURL) --output js.jar
	curl -L $(CLOSUREURL) --output compiler.jar

clean:
	@-rm js.jar compiler.jar
