all:    test

clean:
	@printf "\n\e[32m--------| \e[1;32mCLEANING\e[0m\e[32m |--------\e[0m\n\n"
	rm out.bin *.gch
	@printf "\e[32m\tDONE\e[0m\n"

compiler:
	@printf "\e[32m--------| \e[1;32mCompilation of all your .cpp\e[0m\e[32m |--------\e[0m\n\n"
	g++ -std=c++17  -O3 src/MacMahonGame/MacMahonGame.cpp  main.cpp -o out.bin
	@printf "\e[32m\tDONE\e[0m\n"

test:   compiler
	@printf "\e[32m--------| \e[1mTest of the binary file\e[0m\e[32m |--------\e[0m\n\n"
	./out.bin -f grid/4x4.txt
	@printf "\e[32m\tDONE\e[0m\n"

stat: 
	@printf "\e[32m--------| \e[1;32mCompilation of all your .cpp\e[0m\e[32m |--------\e[0m\n\n"
	g++ -std=c++17 -O3 get_stat.cpp src/MacMahonGame/MacMahonGame.cpp -o out.bin
	@printf "\e[32m\tDONE\e[0m\n"
.PHONY = all clean
