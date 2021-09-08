include ./Makefile.inc

M_SOURCES := p_master.c
S_SOURCES := p_slave.c
V_SOURCES := p_view.c

M_OBJECTS := p_master.o
S_OBJECTS := p_slave.o
V_OBJECTS := p_view.o

M_TARGET := p_master
S_TARGET := p_slave
V_TARGET := p_view

rebuild: clean all

all: $(M_TARGET) $(S_TARGET) $(V_TARGET)
	@rm -rf $(S_OBJECTS) $(M_OBJECTS) $(V_OBJECTS)

# Targets
$(M_TARGET): $(M_OBJECTS)
	$(GCC) $(GCCFLAGS) $< -o $@

$(S_TARGET): $(S_OBJECTS)
	$(GCC) $(GCCFLAGS) $< -o $@

$(V_TARGET): $(V_OBJECTS)
	$(GCC) $(GCCFLAGS) $< -o $@


# Objects
$(M_OBJECTS): $(M_SOURCES)
	$(GCC) $(GCCFLAGS) -c $< -o $@

$(S_OBJECTS): $(S_SOURCES)
	$(GCC) $(GCCFLAGS) -c $< -o $@

$(V_OBJECTS): $(V_SOURCES)
	$(GCC) $(GCCFLAGS) -c $< -o $@

clean:
	@rm -rf $(S_TARGET) $(M_TARGET) $(V_TARGET)
