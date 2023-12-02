#!/bin/bash
#!/bin/sh


COMMAND="clean_odu"

show_help(){
	printf " -d                Build the project\n"
	printf " -c                Clean the project\n"
	printf " -h                Show help\n"
	printf "Unkown Parameter   Will clean the project\n"
	exit 0
}

main(){
	if [[ $1 = "-b" ]];then
		echo "Build with arg $1"
		COMMAND=odu
	elif [[ $1 = "-c" ]];then
		echo "Clean with arg $1"
		COMMAND=clean_odu
	elif [[ $1 = "-h" || $1 = "" ]];then
		show_help
	else
		echo "Default Cleaning"
		COMMAND=clean_odu
	fi

	ARG="MACHINE=BIT64 MODE=FDD VNF_ENABLE=YES"

	make $COMMAND $ARG 
}


main $1
