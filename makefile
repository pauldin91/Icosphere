CXX= g++
CXXFLAGS= -c -o
SOURCEDIR = include
VENDORDIR = ImGui
BUILDDIR = build

SOURCES = $(wildcard $(SOURCEDIR)/*.cpp)
VENDORSOURCES = $(wildcard $(VENDORDIR)/*.cpp)
VENDORHEADERS = $(wildcard $(VENDORDIR)/*.h)

OBJECTS = $(subst $(SOURCEDIR),$(BUILDDIR),$(SOURCES:.cpp=.o))
VENDOROBJECTS = $(subst $(VENDORDIR),$(BUILDDIR),$(VENDORSOURCES:.cpp=.o))


LDLIBS = -lGLEW -lGL -lm -lglfw
EXEEC=Source

all: $(OBJECTS) $(VENDOROBJECTS) $(VENDORHEADERS)
	$(CXX) -g -Wall $(EXEEC).cpp $(BUILDDIR)/*.o  -o $(EXEEC) $(LDLIBS)

$(OBJECTS): $(BUILDDIR)/%.o : $(SOURCEDIR)/%.cpp $(SOURCEDIR)/%.h
	$(CXX) $(CXXFLAGS) $@ $< 

$(VENDOROBJECTS): $(BUILDDIR)/%.o : $(VENDORDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $@ $< 

clean:
	rm -f $(BUILDDIR)/*.o
	rm $(EXEEC)
