all:
	g++ -Wall -std=c++0x -lboost_system -lboost_thread -lpthread uiComponent.cpp regComponent.cpp secComponent.cpp msgComponent.cpp main.cpp -o tauNet

test:
	g++ -Wall -std=c++0x secComponent.cpp secComponent.test.cpp -o test-secComponent
	./test-secComponent
    
testcs1:
	g++ -std=c++0x extras/cs1.cpp -o cs1
	./cs1
    
testcs2:
	gcc -Wall extras/cs2.cpp -o cs2



