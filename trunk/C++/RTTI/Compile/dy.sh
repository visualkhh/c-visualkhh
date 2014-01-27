
CC_SO_FLAG=" -fPIC -g -c -Wall  "
CC_SL_FLAG=" -shared -Wl -lc -o "

g++ $CC_SO_FLAG ./InkPrinter.cpp

g++ $CC_SL_FLAG libmystuff.so.1.0.1 InkPrinter.o 
# g++ -shared -Wl,-soname,libmystuff.so.1 -o libmystuff.so.1.0.1 InkPrinter.o -lc
 mv  libmystuff.so.1.0.1  ./Debug/src/InkPrinter.o
 rm -rf ./InkPrinter.o ./libmystuff.so.1.0.1

 g++ -o main main.cpp Class.cpp -ldl

