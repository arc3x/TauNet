all:
	g++ -Wall -lncurses -lboost_system -lboost_thread -lpthread uiComponent.cpp regComponent.cpp secComponent.cpp msgComponent.cpp main.cpp -o tauNet
