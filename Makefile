run: jeffkopra.cpp
	g++ jeffkopra.cpp -lglut -lGLU -lGL -lGLEW -g
	./a.out
	
clean:
	rm -f *.out *~run
