CC := g++
SRCEXT := cpp
SRCDIR := src
BUILDDIR := build
TARGET := bin/astro

CFLAGS := -g -Wall
LIB := -lswe -pthread -L lib -ldl -lm 
INC := -I include

SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))

$(TARGET) : $(OBJECTS)
	@echo " $(CC) $^ -o $(TARGET) $(LIB)"; $(CC) $^ -o $(TARGET) $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)

.PHONY: clean