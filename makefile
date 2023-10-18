CXX = g++
CXXFLAGS = -Wall -std=c++11
TARGET = "TFT Board Solver"
OBJ_FILES = main.o \
            TFT_Algorithm.o \
            Embeded_text/Champions.o \
            Embeded_text/Traits.o \
			Embeded_text/Prompts.o \
            #TFT_Icon.o

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CXX) $(LDFLAGS) -o $(TARGET) $(OBJ_FILES)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

TFT_Icon.o: TFT_Icon.rc
	windres -i TFT_Icon.rc -o TFT_Icon.o

clean:
	del /Q "TFT Board Solver".exe *.o
	del /Q Embeded_text\*.o
