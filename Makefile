prefix :=/usr/local
PROGRAM := hipchat-c

all:
	test -d $(prefix)/$(PROGRAM) || mkdir $(prefix)/$(PROGRAM)
	gcc hipchat-c.c -lcurl -o $(prefix)/$(PROGRAM)/$(PROGRAM)

install: all
	test -d $(prefix)/$(PROGRAM) || mkdir $(prefix)/$(PROGRAM)
	test -L /usr/bin/$(PROGRAM) || ln -s $(prefix)/$(PROGRAM)/$(PROGRAM)  /usr/bin/$(PROGRAM)


