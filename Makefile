default:
	g++ -std=c++17 -o witchertracker src/*.cpp

grade:
	python3 test/grader.py ./witchertracker test-cases