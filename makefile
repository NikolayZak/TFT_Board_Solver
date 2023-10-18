CXX = g++
CXXFLAGS = -Wall -std=c++11 -static -static-libgcc -static-libstdc++
TARGET = "TFT Board Solver"
EMBEDED_FOLDER = Embeded_text

OBJ_FILES = $(EMBEDED_FOLDER)/Champions.o \
            $(EMBEDED_FOLDER)/Traits.o \
            $(EMBEDED_FOLDER)/Prompts.o \
            TFT_Algorithm.o \
            main.o \
            $(EMBEDED_FOLDER)/TFT_Icon.o

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CXX) $(LDFLAGS) -o $(TARGET) $(OBJ_FILES)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(EMBEDED_FOLDER)/TFT_Icon.o: $(EMBEDED_FOLDER)/TFT_Icon.rc
	windres -i $(EMBEDED_FOLDER)/TFT_Icon.rc -o $(EMBEDED_FOLDER)/TFT_Icon.o

run: $(TARGET)
	./$(TARGET)

clean:
	del /Q $(TARGET).exe *.o
	del /Q $(EMBEDED_FOLDER)\*.o