-- 04-a-5000
Намерете командите на 10-те най-стари процеси в системата.
	ps --no-headers -o "comm" --sort=-etimes | head -n 10
	
-- 04-a-6000
Намерете PID и командата на процеса, който заема най-много виртуална памет в системата.
	ps --no-headers -o "pid comm" --sort=-vsz | head -n 1
	 
-- 04-b-5000
Намерете колко физическа памет заемат всички процеси на потребителската група SI.
	ps -G 504 -o "rss" | awk '{sum += 1;} END {print sum;}'
	
-- 04-b-7000
Намерете колко физическа памет заемат осреднено всички процеси на потребителската група SI. Внимавайте, когато групата няма нито един процес.
	ps -G 504 -o "rss" | awk '{sum += $1; n += 1;} END {if (n > 0) print sum/n;}'
	
-- 04-b-8000
Намерете всички PID и техните команди (без аргументите), които нямат tty, което ги управлява. Изведете списък само с командите без повторения.
	ps -t '-' -o "pid comm"
	ps -t '-' -o "pid comm" | awk -F ' ' '{print $2;}' | sort | uniq

-- 04-b-9000
Да се отпечатат PID на всички процеси, които имат повече деца от родителския си процес.
	#gets the number of children by a given pid, i.e. we get with the --ppid all the processes that have the function argument as parent pid
	function GetChildrenCount 
	{
		ps --no-headers --ppid "$1" | wc -l
	}

	ps --no-headers -e -o pid,ppid | while read varPID varPPID
	do
		if [[ $(children "${varPID}") -gt $(children "${varPPID}")
		then
			echo "${varPID}"
		fi
	done
