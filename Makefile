# Compilador
CXX = g++

# Opciones de compilación
CXXFLAGS = -std=c++11 -Wall -Wextra

# Directorios de inclusión
INCLUDES = -I./dependencias -I./dependencias/imgui

# Bibliotecas
LIBS = -lglfw -lGLEW -lGL

# Nombre del ejecutable
TARGET = pso_prog ae_prog

# Archivos fuente
AE_SRCS = ae.cpp

PSO_SRC = pso.cpp

SRCS = dependencias/shaders/Shader.cpp

# Archivos fuente de ImGui y sus backends

IMGUI_SRCS = dependencias/imgui/imgui.cpp \
			 dependencias/imgui/imgui_demo.cpp \
			 dependencias/imgui/imgui_draw.cpp \
			 dependencias/imgui/imgui_tables.cpp \
			 dependencias/imgui/imgui_widgets.cpp  \
             dependencias/imgui/backends/imgui_impl_glfw.cpp \
             dependencias/imgui/backends/imgui_impl_opengl3.cpp 




# Todos los archivos fuente
ALL_SRCS = $(SRCS) $(IMGUI_SRCS)

# Archivos objeto
PSO_OBJS = pso.o
AE_OBJS = ae.o
OBJS = $(ALL_SRCS:.cpp=.o)

########################################################################
#                             REGLAS
########################################################################
# Regla por defecto
all: $(TARGET)

# Regla para compilar el ejecutable
pso_prog: $(PSO_OBJS) $(OBJS)
	$(CXX) $(CXXFLAGS) -o pso_prog $(PSO_OBJS) $(OBJS) $(LIBS)

ae_prog: $(AE_OBJS) $(OBJS)
	$(CXX) $(CXXFLAGS) -o ae_prog $(AE_OBJS) $(OBJS) $(LIBS)

# Regla para compilar los archivos objeto
%.o: %.cpp
	$(CXX) -g $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Limpiar archivos generados
clean:
	rm -f $(OBJS) $(PSO_OBJS) $(AE_OBJS) $(TARGET)

# Regla para instalar dependencias en Ubuntu o Arch Linux
install-deps:
	@echo "Detectando sistema operativo..."
	@if [ -f /etc/arch-release ]; then \
		echo "Sistema detectado: Arch Linux"; \
		sudo pacman -S --needed glfw glew mesa; \
	elif [ -f /etc/debian_version ]; then \
		echo "Sistema detectado: Ubuntu/Debian"; \
		sudo apt update && sudo apt install -y libglfw3-dev libglew-dev libgl1-mesa-dev; \
	else \
		echo "Sistema no reconocido. Instala las dependencias manualmente."; \
	fi

# Para evitar conflictos con archivos que se llamen 'clean'
.PHONY: clean