RPI_LIB = rgbmatrix

CXXFLAGS = -Wall -O3 -g -I../rpi-led-matrix-panel
LDFLAGS = -L../rpi-led-matrix-panel
LIBS = -lpthread -l$(RPI_LIB)
TARGET = buoyViz

SRCS = LedMatrixBuoyViz.cpp Thread.cpp
OBJS = $(SRCS:.cpp=.o)


all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(TARGET) $(OBJS) $(LDFLAGS) $(LIBS)

clean:
	rm -f $(OBJS) $(TARGET)

