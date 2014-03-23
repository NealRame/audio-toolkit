CFLAGS   += -O3
CXXFLAGS += -O3

$(TARGET): $(OBJECTS)
	$(CXX) $(LFLAGS) -o $@ $(OBJECTS)

include $(DEPS)
