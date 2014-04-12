CFLAGS   += -O0 -g
CXXFLAGS += -O0 -g

$(TARGET): $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS)

include $(DEPS)
