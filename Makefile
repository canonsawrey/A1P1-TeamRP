#Testing utilities

build:
	g++ -std=c++11 -w -O3 -o sorer main.cpp

test:
	#building our unit tests executable
	g++ -std=c++11 -w -O3 -o tests tests.cpp

	#command line input tests
	# the '-' preceding tests is for ignoring errors/failed assert statements for testing.
	./sorer -f "data.sor"
	./sorer -f "data.sor" -print_col_type 0
	./sorer -f "data.sor" -print_col_type 1
	./sorer -f "data.sor" -print_col_type 2
	./sorer -f "data.sor" -print_col_type 4
	-./sorer -f "data.sor" -print_col_type 7
	./sorer -f "data.sor" -from 0 -len 1000 -print_col_type 0
	-./sorer -f "data.sor" -from 0 -len 1000 -print_col_idx 0
	./sorer -f "data.sor" -from 0 -len 1000 -print_col_idx 0 1
	./sorer -f "data.sor" -from 0 -len 1000 -print_col_idx 0 2
	./sorer -f "data.sor" -from 0 -len 1000 -print_col_idx 1 2
	-./sorer -f "data.sor" -from 0 -len 1000 -print_col_idx 0 8
	-./sorer -f "data.sor" -from 0 -len 1000 -print_col_idx 8 0
	./sorer -f "data.sor" -from 0 -len 1000 -print_col_idx 3 2
	-./sorer -f "data.sor" -from 0 -len 1000 -is_missing_idx 0
	-./sorer -f "data.sor" -from 0 -len 1000 -is_missing_idx 8 0
	-./sorer -f "data.sor" -from 0 -len 1000 -is_missing_idx 0 9
	./sorer -f "data.sor" -from 0 -len 1000 -is_missing_idx 0 7
	./sorer -f "data.sor" -from 0 -len 1000 -is_missing_idx 0 5
	./sorer -f "data.sor" -from 0 -len 1000 -is_missing_idx 2 4

	#unit tests
	./tests

clean:
	rm ./sorer
	rm ./tests

time:
	### TIMING TESTS ###
	time ./sorer -f assignment1ex1.txt
	time ./sorer -f assignment1ex2.txt
	### END TIMING TESTS ###