#---------------------------------------------------------------------------------
# Borrar cosas implicitas
#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------
# TARGET es el nombre de el ejecutable (o salida)
# BUILD es la carpeta donde van a llegar los archivos compilados y otros
# SOURCES es una lista de directorios con el source
# INCLUDES es una lista de directorios con archivos cabecera extra
#---------------------------------------------------------------------------------
TARGET		:=	$(notdir $(CURDIR))_sec
BUILD		:=	build
SOURCES		:=	source
DATA		:=	data
INCLUDES	:=

#---------------------------------------------------------------------------------
# Opciones de compilacion
#---------------------------------------------------------------------------------

CFLAGS	= -Wall #-g -O2 -mrvl  $(MACHDEP) $(INCLUDE)
CXXFLAGS	=#	$(CFLAGS)

#---------------------------------------------------------------------------------
# librerias para linkear? (lm = math.h)
#---------------------------------------------------------------------------------
LIBS	:=	-lm 

#---------------------------------------------------------------------------------
# directorios con librerias adicionales
#---------------------------------------------------------------------------------
LIBDIRS	:= 

#---------------------------------------------------------------------------------
# desde aqui para abajo son definiciones del makefile para que funcione todo bien
#---------------------------------------------------------------------------------
# Definir el compilador y rutinas de compilacion
#---------------------------------------------------------------------------------
PREFIX	:=	mpi

export CC		:=	$(PREFIX)cc
export CXX		:=	$(PREFIX)cxx

#---------------------------------------------------------------------------------
%.o: %.cpp
	@echo compilando $(notdir $<)
	@$(CXX) -MMD -MP -MF $(DEPSDIR)/$*.d $(CXXFLAGS) -c $< -o $@
	
#---------------------------------------------------------------------------------
%.o: %.c
	@echo compilando $(notdir $<)
	@$(CC) -MMD -MP -MF $(DEPSDIR)/$*.d $(CFLAGS) -c $< -o $@

#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
#---------------------------------------------------------------------------------

export OUTPUT	:=	$(CURDIR)/$(TARGET)

export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
					$(foreach dir,$(DATA),$(CURDIR)/$(dir))

export DEPSDIR	:=	$(CURDIR)/$(BUILD)

#---------------------------------------------------------------------------------
# construir automaticamente listas de archivos para nuestro proyecto
#---------------------------------------------------------------------------------
CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))

#---------------------------------------------------------------------------------
# usar CXX para linkear proyectos en C++ , CC para los de C
#---------------------------------------------------------------------------------
ifeq ($(strip $(CPPFILES)),)
	export LD	:=	$(CC)
else
	export LD	:=	$(CXX)
endif

export OFILES	:=	$(addsuffix .o,$(BINFILES)) \
					$(CPPFILES:.cpp=.o) $(CFILES:.c=.o)

#---------------------------------------------------------------------------------
# hace una lista de direcciones donde hay cosas para incluir
#---------------------------------------------------------------------------------
export INCLUDE	:=	$(foreach dir,$(INCLUDES), -iquote $(CURDIR)/$(dir)) \
					$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
					-I$(CURDIR)/$(BUILD) \

#---------------------------------------------------------------------------------
# hace una lista de direcciones donde estan las librerias
#---------------------------------------------------------------------------------
export LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib) \

export OUTPUT	:=	$(CURDIR)/$(TARGET)
.PHONY: $(BUILD) clean

#---------------------------------------------------------------------------------
$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@make --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

#---------------------------------------------------------------------------------
clean:
	@echo limpiando ...
	@rm -fr $(BUILD) $(OUTPUT).elf $(OUTPUT).dol

#---------------------------------------------------------------------------------
run:
	mpiexec $(TARGET)

#---------------------------------------------------------------------------------
else

DEPENDS	:=	$(OFILES:.o=.d)

#---------------------------------------------------------------------------------
# linkear los archivos compilados
#---------------------------------------------------------------------------------
$(OUTPUT): $(OFILES)
	@echo linkeando ... $(notdir $@)
	@$(LD)  $^ $(LDFLAGS) $(LIBPATHS) $(LIBS) -o $@
	@echo listoko
#---------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------
