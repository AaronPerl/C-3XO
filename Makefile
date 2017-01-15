BINDIR    := bin
SRCDIR    := src
SOURCES   := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS   := $(addprefix $(BINDIR)/,$(notdir $(SOURCES:.cpp=.o)))
BINARY    := c-3xo

CXXFLAGS := 
CPPFLAGS := -std=c++11
LDLIBS   := -lwiringPi -lwiringPiDev -lm -lwiringPiPca9685 -lrt
LDFLAGS  := 
MKDIR    := mkdir
CXX      := g++

all: $(BINARY)

clean:
	rm -f  ./$(BINARY)
	rm -rf ./$(BINDIR)

test:
	echo $(SOURCES)
	echo $(OBJECTS)

$(BINARY): $(OBJECTS)
	$(CXX) $(LDFLAGS) $^ -o $@ $(LDLIBS)

$(BINDIR): 
	$(MKDIR) $@

$(BINDIR)/%.o : $(SRCDIR)/%.cpp | $(BINDIR)
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $< -o $@

.PHONY: all clean test
