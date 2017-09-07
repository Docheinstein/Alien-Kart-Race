compile_result=$(./compile.sh)
if [ $compile_result -eq 0 ]; then
	echo "Compiled succesfully.";
	echo "Killing previous instances..."
	./kill.sh
	echo "Running executable..."
	../game &
else
	echo "Compilation failed. Not running executable."
fi
