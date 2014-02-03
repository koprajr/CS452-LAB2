run: example.cpp
	g++ example.cpp -lglut -lGLU -lGL -lGLEW -g
	./a.out
	
clean:
	rm -f *.out *~ run
