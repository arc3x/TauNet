test:
	g++ -Wall -std=c++0x secComponent.cpp secComponent.test.cpp -o test-secComponent

all:
	g++ -Wall -std=c++0x -lboost_system -lboost_thread -lpthread uiComponent.cpp regComponent.cpp secComponent.cpp msgComponent.cpp main.cpp -o tauNet
