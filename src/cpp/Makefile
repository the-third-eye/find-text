CFLAGS = -Wall -O2

COMP_OBJS = conn_comp.o
COMP_LIBS = -lopencv_core -lopencv_imgproc \
			-lopencv_imgcodecs -lopencv_highgui

PROC_OBJS = img_proc.o
PROC_LIBS = -llept -ltesseract

.PHONY: all
all: conn_comp img_proc

conn_comp: $(COMP_OBJS)
	g++ $(CFLAGS) -o conn_comp $(COMP_OBJS) $(COMP_LIBS)

img_proc: $(PROC_OBJS)
	g++ $(CFLAGS) -o img_proc $(PROC_OBJS) $(PROC_LIBS)

.PHONY: cleanall cleanconn_comp cleanimg_proc
cleanall: 
	rm conn_comp img_proc $(COMP_OBJS) $(PROC_OBJS)
cleanconn_comp: 
	rm conn_comp $(COMP_OBJS)
cleanimg_proc: 
	rm img_proc $(PROC_OBJS)
