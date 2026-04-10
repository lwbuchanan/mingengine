CXX=g++
LDFLAGS = -lwayland-client -lX11
INCLUDES = -I./src -I./src/gen
FLAGS = -Wall -Wextra -Wswitch-enum -pedantic
RELFLAGS = -O3
DEBFLAGS = -ggdb

SRCDIR=src
ODIR=o

OUTPUTDIR=out
TARGET=$(OUTPUTDIR)/game

HEADERS = $(shell find $(SRCDIR) -regextype egrep -regex "$(SRCDIR)/(.+\.h)|(.+\.hpp)")
CXXSRC = $(shell find $(SRCDIR) -regextype egrep -regex "$(SRCDIR)/(.+\.cpp)")
OBJS = $(patsubst $(SRCDIR)/%.c, $(ODIR)/%.o, $(CSRC)) $(patsubst $(SRCDIR)/%.cpp, $(ODIR)/%.o, $(CXXSRC))

ifeq ($(BUILD), release)
	FLAGS += $(RELFLAGS)
else
	FLAGS += $(DEBFLAGS)
endif

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

.PHONY: clean
clean:
	@rm -rf $(ODIR) $(GENSRCDIR) $(OUTPUTDIR)

.PHONY: list
list:
	@echo "C++ Sources: $(CXXSRC)"
	@echo "Headers: $(HEADERS)"
	@echo "Objects: $(OBJS)"

.PHONY: clangd
clangd:
	@bear -- make all -B
