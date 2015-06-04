MARS : MARS_Circular_Buffer.o MARS_VideoWriter.o MARS_VideoReader.o MARS_Camera.o MARS_Datasource.o MARS_Engine.o MARS_Graphic.o CustomGraphics/MARS_Graphic_FlatMarker_FillCircle.o CustomGraphics/MARS_Graphic_FlatMarker_OpenRect.o CustomGraphics/MARS_Graphic_FloatingMeter.o ./utils/timer.o ./utils/udp.o main.o
	@echo "Building MARS System..."
	g++ main.o MARS_Circular_Buffer.o MARS_VideoWriter.o MARS_Camera.o MARS_VideoReader.oMARS_Datasource.o MARS_Engine.o MARS_Graphic.o CustomGraphics/MARS_Graphic_FlatMarker_FillCircle.o CustomGraphics/MARS_Graphic_FlatMarker_OpenRect.o CustomGraphics/MARS_Graphic_FloatingMeter.o ./utils/timer.o ./utils/udp.o -o MARStest -lpthread -lGLEW -lglut -lGL -lGLU -lopencv_core -lopencv_video -lopencv_highgui -lopencv_imgproc

./main.o : ./main.cpp
	@echo "Building main..."
	g++ -g  -c ./main.cpp -o ./main.o

./utils/timer.o : ./utils/timer.cpp ./utils/timer.hpp
	@echo "Building Timer Component of MARS..."
	g++ -g -c ./utils/timer.cpp -o ./utils/timer.o
	
./utils/udp.o : ./utils/udpPort.cpp ./utils/udpPort.hpp
	@echo "Building UDP Component of MARS..."
	g++ -g -c ./utils/udpPort.cpp -o ./utils/udp.o

./CustomGraphics/MARS_Graphic_FlatMarker_OpenRect.o : ./CustomGraphics/MARS_Graphic_FlatMarker_OpenRect.cpp ./CustomGraphics/MARS_Graphic_FlatMarker_OpenRect.hpp
	@echo "Building MARS_Graphic_FlatMarker_OpenRect..."
	g++ -g -c ./CustomGraphics/MARS_Graphic_FlatMarker_OpenRect.cpp -o ./CustomGraphics/MARS_Graphic_FlatMarker_OpenRect.o

./CustomGraphics/MARS_Graphic_FloatingMeter.o : ./CustomGraphics/MARS_Graphic_FloatingMeter.cpp  ./CustomGraphics/MARS_Graphic_FloatingMeter.hpp
	@echo "Building MARS_Graphic_FloatingMeter..."
	g++ -g -c ./CustomGraphics/MARS_Graphic_FloatingMeter.cpp -o ./CustomGraphics/MARS_Graphic_FloatingMeter.o

./Custom_Graphic/MARS_Graphic_FlatMarker_FillCircle.o : ./CustomGraphics/MARS_Graphic_FlatMarker_FillCircle.cpp ./CustomGraphics/MARS_Graphic_FlatMarker_FillCircle.hpp
	@echo "Building MARS_Graphic_FlatMarker_FillCircle..."
	g++ -g -c ./CustomGraphics/MARS_Graphic_FlatMarker_FillCircle.cpp -o ./CustomGraphics/MARS_Graphic_FlatMarker_FillCircle.o

./MARS_Circular_Buffer.o : ./MARS_Circular_Buffer.cpp ./MARS_Circular_Buffer.hpp
	@echo "Building MARS Circular Buffer..."
	g++ -g -c ./MARS_Circular_Buffer.cpp -o ./MARS_Circular_Buffer.o

./MARS_Camera.o : ./MARS_Camera.cpp ./MARS_Camera.hpp
	@echo "Building MARS Camera..."
	g++ -g -c ./MARS_Camera.cpp -o ./MARS_Camera.o
	
./MARS_VideoWriter.o : ./MARS_VideoWriter.cpp ./MARS_VideoWriter.hpp
	@echo "Building MARS Video Writer..."
	g++ -g -c ./MARS_VideoWriter.cpp -o ./MARS_VideoWriter.o

./MARS_VideoReader.o : ./MARS_VideoReader.cpp ./MARS_VideoReader.hpp
	@echo "Building MARS Video Reader..."
	g++ -g -c ./MARS_VideoReader.cpp -o ./MARS_VideoReader.o

./MARS_Graphic.o : ./MARS_Graphic.cpp ./MARS_Graphic.hpp
	@echo "Building MARS Graphic..."
	g++ -g -c ./MARS_Graphic.cpp -o ./MARS_Graphic.o

./MARS_Datasource.o : ./MARS_Datasource.cpp ./MARS_Datasource.hpp
	@echo "Building MARS Datasource..."
	g++ -g -c ./MARS_Datasource.cpp -o ./MARS_Datasource.o

./MARS_Engine.o : ./MARS_Engine.cpp ./MARS_Engine.hpp
	@echo "Building MARS Engine..."
	g++ -g -c ./MARS_Engine.cpp -o ./MARS_Engine.o

./MARS_Math.o : ./MARS_Math.cpp ./MARS_Math.hpp
	@echo "Building MARS Math..."
	g++ -g -c ./MARS_Math.cpp -o ./MARS_Math.o

MARS_debug : MARS_Circular_Buffer.o MARS_Camera.o MARS_Datasource.o MARS_Engine.o MARS_Graphic.o CustomGraphics/MARS_Graphic_FlatMarker_FillCircle.o CustomGraphics/MARS_Graphic_FlatMarker_OpenRect.o ./utils/timer.o ./utils/udp.o main.o
	clear
	clear
	g++ -g main.o MARS_Circular_Buffer.o MARS_Camera.o MARS_Datasource.o MARS_Engine.o MARS_Graphic.o CustomGraphics/MARS_Graphic_FlatMarker_FillCircle.o CustomGraphics/MARS_Graphic_FlatMarker_OpenRect.o ./utils/timer.o ./utils/udp.o -o MARStest -lpthread -lGLEW -lglut -lGL -lGLU -lopencv_core -lopencv_video -lopencv_highgui -lopencv_imgproc
	
clean:	
	rm ./*.o
	rm ./utils/timer.o
	rm ./utils/udp.o
	rm MARStest

