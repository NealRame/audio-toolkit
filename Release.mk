CFLAGS   += -O3
CXXFLAGS += -O3

$(TARGET): $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS)

include $(DEPS)
