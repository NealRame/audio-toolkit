CFLAGS   += -O0 -g -DDEBUG
CXXFLAGS += -O0 -g -DDEBUG

$(TARGET): $(OBJECTS)
	$(CXX) $(LFLAGS) -o $@ $(OBJECTS)

include $(DEPS)