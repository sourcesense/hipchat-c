## A small program written in C for sending message to hipchat

Copyright (c) 2016, Eugenio Marzo <eugenio.marzo@yahoo.it>

### BUILDING

To build hipchat-c, make sure you have gcc,make tools and libcurl
installed. Then run the following:

gcc hipchat-c.c  -lcurl -o hipchat-c

### EXAMPLE 

./hipchat-c -r Devopslab -t 123l9QAwdmsTCrKJrUriowYereOlkJch4myk5R65a -m "puppet or chef?"
