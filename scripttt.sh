#!bin/bash


if [ "$1" -eq 0 ]
then
	chmod 444 "$2"
	kate -n  "$2" 2>ERROR 
	#chmod 644 "$2"
	
else
	
	kate -n "$2" 2>ERROR 
	
fi

exit 
