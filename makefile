CXX = g++
CXXFLAGS = -Wall -std=c++11
TARGET = "TFT Board Solver"
EMBEDED_FOLDER = Embeded_text

OBJ_FILES = $(EMBEDED_FOLDER)/Prompts.o \
			Database.o \
			Board.o \
			Solver.o \
			Hasher.o \
			Interface.o \
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
