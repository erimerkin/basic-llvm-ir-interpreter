all: mylang2ir 

mylang2ir: Parser.o Tokenizer.o ASTNode.o Main.o
	@g++ -o mylang2ir -std=c++14 Main.o Parser.o ASTNode.o Tokenizer.o
	@echo "mylang2ir compiled successfully"

Main.o: Main.cpp Parser.hpp
	@g++ -std=c++14 -c main.cpp

Parser.o: Tokenizer.hpp ASTNode.hpp Parser.hpp Parser.cpp
	@g++ -std=c++14 -c Parser.cpp

Tokenizer.o: Tokenizer.cpp Tokenizer.hpp 
	@g++ -std=c++14 -c Tokenizer.cpp


ASTNode.o: ASTNode.cpp ASTNode.hpp
	@g++ -std=c++14 -c ASTNode.cpp

clean:
	@rm -f *.o

















