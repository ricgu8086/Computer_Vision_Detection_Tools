# change paths if necessary
INCLUDES = -I/usr/local/include/opencv2
LIBS =   -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_video -lopencv_legacy
LIBDIRS = -L/usr/local/lib

OPT = -g -O0 -Wno-deprecated -ggdb `pkg-config --cflags opencv` `pkg-config --libs opencv`

CC= g++

.PHONY: all clean

OBJS =  main.o functions.o

clean:
		rm -f *.o detection_functions
			
all:	detection_functions
		echo all: make complete
%.o:%.cpp
	$(CC) -c $(INCLUDES) $+ $(OPT)
	
detection_functions: $(OBJS)
		$(CC) $(LIBDIRS) $(LIBS) -o $@ $+ $(OPT)




