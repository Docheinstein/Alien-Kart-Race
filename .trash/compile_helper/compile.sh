OK_CODE=0
FAIL_CODE=1
retval=""
g++ -c ../main.cpp -I/usr/include/SFML/
if [ $? -eq 0 ]; then
	g++ ./main.o -o ../game -L/usr/include/SFML/ -lsfml-graphics -lsfml-window -lsfml-system
	rm main.o
	if [ $? -eq 0 ]; then
		retval=$OK_CODE
	else
		retval=$FAIL_CODE
	fi
else
	retval=$FAIL_CODE
fi
echo "$retval"
