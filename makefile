RPI_LED_LIB = rgbmatrix
RPI_HTTP_LIB = httprequest

CXXFLAGS = -Wall -O3 -g -I../rpi-led-matrix-panel -I../rpi-http-request
LDFLAGS = -L../rpi-led-matrix-panel -L../rpi-http-request
LIBS = -lpthread -l$(RPI_LED_LIB) -l$(RPI_HTTP_LIB)
TARGET = buoyViz

SRCS = BuoyInfoDataViz.cpp Thread.cpp
OBJS = $(SRCS:.cpp=.o)


all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(TARGET) $(OBJS) $(LDFLAGS) $(LIBS)

clean:
	rm -f $(OBJS) $(TARGET)

