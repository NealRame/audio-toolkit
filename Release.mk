CFLAGS   += -O3
CXXFLAGS += -O3

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS)

include $(DEPS)
