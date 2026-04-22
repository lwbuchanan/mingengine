CXX=g++
LDFLAGS = -lX11 -fopenmp
INCLUDES = -I./src -I./src/gen
FLAGS = -Wall -Wextra -Wswitch-enum -pedantic -O3 -ggdb -fopenmp

SRCDIR=src
ODIR=o

TARGET=game

HEADERS = $(shell find $(SRCDIR) -regextype egrep -regex "$(SRCDIR)/(.+\.h)|(.+\.hpp)")
CXXSRC = $(shell find $(SRCDIR) -regextype egrep -regex "$(SRCDIR)/(.+\.cpp)")
OBJS = $(patsubst $(SRCDIR)/%.c, $(ODIR)/%.o, $(CSRC)) $(patsubst $(SRCDIR)/%.cpp, $(ODIR)/%.o, $(CXXSRC))

# Main build task
default: all

# Compile objects
$(ODIR)/%.o: $(SRCDIR)/%.cpp $(HEADERS)
	@mkdir -p $(@D)
	$(CXX) -c -o $@ $< $(INCLUDES) $(FLAGS)

# Link target
$(TARGET): $(OBJS)
	@mkdir -p $(@D)
	$(CXX) -o $@ $^ $(LDFLAGS)

# Phony rules
.PHONY: all
all: $(TARGET)

.PHONY: run
run: all
	@./$(TARGET)

.PHONY: debug
debug: all
	@gdb $(TARGET)

.PHONY: clean
clean:
	@rm -rf $(ODIR) $(GENSRCDIR) $(TARGET)

.PHONY: list
list:
	@echo "C++ Sources: $(CXXSRC)"
	@echo "Headers: $(HEADERS)"
	@echo "Objects: $(OBJS)"

.PHONY: clangd
clangd:
	@bear -- make all -B
