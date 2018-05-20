CFLAGS = -std=c++11

COMP_OBJS = conn_comp.o
COMP_LIBS = -lopencv_core -lopencv_imgproc \
			-lopencv_imgcodecs -lopencv_highgui

PROC_OBJS = img_proc.o
PROC_LIBS = -llept -ltesseract

TESS_OBJS = tessconn_comp.o
TESS_LIBS = -llept -ltesseract

.PHONY: all
all: conn_comp img_proc tessconn_comp

conn_comp: $(COMP_OBJS)
	g++ -o conn_comp $(COMP_OBJS) $(COMP_LIBS)

conn_comp.o: conn_comp.cpp
	g++ -c $(CFLAGS) conn_comp.cpp

img_proc: $(PROC_OBJS)
	g++ -o img_proc $(PROC_OBJS) $(PROC_LIBS)

img_proc.o: img_proc.cpp
	g++ -c $(CFLAGS) img_proc.cpp

tessconn_comp: $(TESS_OBJS)
	g++ -o tessconn_comp $(TESS_OBJS) $(TESS_LIBS)

tessconn_comp.o: tessconn_comp.cpp
	g++ -c $(CFLAGS) tessconn_comp.cpp

.PHONY: cleanall cleanconn_comp cleanimg_proc cleantessconn_comp
cleanall: 
	rm conn_comp img_proc tessconn_comp $(COMP_OBJS) $(PROC_OBJS) $(TESS_OBJS)
cleanconn_comp: 
	rm conn_comp $(COMP_OBJS)
cleanimg_proc: 
	rm img_proc $(PROC_OBJS)
cleantessconn_comp:
	rm tessconn_comp $(TESS_OBJS)
