-- SI-2016-z1
Задача 1.
Напишете shell скрипт, който по подаден един позиционен параметър, ако този параметър е директория, намира 
всички symlink-ове в нея и под-директориите ѝ с несъществуващ destination.
	#!/bin/bash
	if [[ "$#" -ne 1 ]]
	then
		exit 1
	fi

	dir="${$1}"
	if [[ !(-d "${dir}" ]]
	then
		exit 2
	fi
	
	#i.e. we search for broken links
	find -L "${dir}" -mindepth 1 -type l -printf "%f\n"
	
-- SI-2016-z2
Задача 2.
Напишете shell скрипт, който приема един позиционен параметър - число. Ако скриптът се изпълнява като root, да извежда обобщена информация
за общото количество активна памет (RSS - resident set size, non-swapped physical memory that a task has used) на процесите на всеки потребител.
Ако за някой потребител обобщеното число надвишава подадения параметър, да изпраща подходящи сигнали за прекратяване на процеса 
с най-много активна памет на потребителя.
Забележка: Приемаме, че изхода в колоната RSS е число в същата мерна единица, като числото, подадено като аргумент. Примерен формат:
USER       PID %CPU %MEM    VSZ   RSS TTY      STAT START   TIME COMMAND
root         1  0.0  0.0  15816  1884 ?        Ss   May12   0:03 init [2]
root         2  0.0  0.0      0     0 ?        S    May12   0:00 [kthreadd]
root         3  0.0  0.0      0     0 ?        S    May12   0:02 [ksoftirqd/0]
Алтернативно може да ползвате изхода от ps -e -o uid,pid,rss
	#!/bin/bash

	if [[ $# -ne 1 ]]
	then
		echo "1 parameter needed!"
		exit 1
	fi

	# if [[ !($1 =~ ^[0-9]+$) ]]
	# then
	# 	echo "bad"
	# 	exit 3
	# fi

	case $1 in
		''|*[^0-9]*)
			echo "number needed!"
			exit 2
		;;
		*)
			# echo "all fine"
		;;
	esac

	# if [[ $(id -u) -ne 0 ]]
	# then
	# 	echo "you must be root!" 
	# 	exit 3
	# fi

	users=$(ps --no-headers -e -o user | sort | uniq)
	echo $users

	for user in $users
	do
		totalrss=0

		echo "user : ${user}"

		while read line
		do
			currPID=$(echo $line | awk '{print $1}')
			currRSS=$(echo $line | awk '{print $2}')
			totalrss=$(( "${totalrss}" + "${currRSS}" ))

			echo "currPID : ${currPID}"
			echo "currRSS : ${currRSS}"		
			echo "totalrss : ${totalrss}"

		done < <(ps --no-headers -u "${user}" -o pid,rss | sort -n -k 2)

		if [[ $totalrss -gt $1 ]]
		then 
			
			mostRSShungryPID=$(ps --no-headers -u "${user}" -o pid,rss | sort -n -r -k 2 | head -n 1 | awk '{print $1}')
			# echo "mostRSShungryPID : ${mostRSShungryPID}"
			# echo "currPID : ${currPID}"
			# echo -e "\n"

			# kill -15 "${mostRSShungryPID}"
			# sleep 2
			# kill -9 "${mostRSShungryPID}"

		fi

	done
	
	
	
-- SI-2016-z3
Задача 3.
Напишете shell скрипт който, ако се изпълнява от root, проверява кои потребители на системата нямат homedir или не могат да пишат в него.
Примерен формат:

root:x:0:0:root:/root:/bin/bash
daemon:x:1:1:daemon:/usr/sbin:/usr/sbin/nologin
bin:x:2:2:bin:/bin:/usr/sbin/nologin

#!/bin/bash

if [[ $(id -u) -ne 0 ]]
then
	echo "you must be root!"
	exit 1
fi


cat /etc/passwd | awk -F[:,] '{print $1 " " $6}' | while read user homedir
do
	#echo "user : ${user} homedir : ${homedir}"
	
	if [[ -z "${homedir}" ]]
	then
		echo "${user} does not have a home directory"
			echo -e "\n"
		break
	fi

	dirperm=$(ls -ld "${homedir}" | awk '{print $1}')
	#echo "dirperm : ${dirperm}"

	if [[ $(echo "${dirperm}" | cut -c 3) != 'w' ]]
	then
		echo "${user} can not write in his home directory"
	fi


done

-- SI-2017-z1
зад. 1:
--------
Напишете скрипт, който получава задължителен първи позиционен параметър - директория и незадължителен втори - число. Скриптът трябва да проверява подадената директория и нейните под-директории и да извежда имената на:
- при подаден на скрипта втори параметър - всички файлове с брой hardlink-ове поне равен на параметъра
- при липса на втори параметър - всички symlink-ове с несъществуващ destination (счупени simlink-ове)

Заб.: За удобство приемаме, че ако има подаден втори параметър, то той е число.

#!/bin/bash

function CheckDir 
{
	if [[ !(-d "${1}") ]]
	then
		echo "${1} is not a directory!"
		exit 2
	fi
}

case $# in
	1)
		CheckDir "${1}"

		# all symlinks with no destination
		find -L "${1}" -type l
	;;
	2)
		CheckDir "${1}"

		# all files with hardlink# >= $2
		for i in $(find "${1}")
		do
			if [[ $(stat --printf "%h" "${i}") -ge "${2}" ]]
			then
				echo "${i}"
			fi
		done
	;;

	*)
		echo "Error-parameters needed : <directory> [<number>]"
		exit 1
	;;
esac

-- SI-2017-z2
зад. 2:
--------
Напишете скрипт, който приема три задължителни позиционни параметра - директория SRC, директория DST (която не трябва
да съдържа файлове) и низ (АBC). Ако скриптът се изпълнява от root потребителя, то той трябва да намира всички файлове
в директорията SRC и нейните под-директории, които имат в името си като под-низ ABC, и да ги мести в директорията DST, 
запазвайки директорийната структура.
Заб.: За удобство приемаме, че не искаме да запазваме метаданните owner, group и permission на директорийната структура,
т.е., не ни интересуват тези параметри на новите директории, които скриптът би генерирал в DST.
Пример:
- в SRC (/src) има следните файлове:
        /src/foof.txt
        /src/1/bar.txt
        /src/1/foo.txt
        /src/2/1/foobar.txt
        /src/2/3/barf.txt
- DST (/dst) е празна директория
- зададения низ е foo

Резултат:
- SRC:
        /src/1/bar.txt
        /dst/2/1/
        /src/2/3/barf.txt
- DST:
        /dst/foof.txt
        /dst/1/foo.txt
        /dst/2/1/foobar.txt

#!/bin/bash

if [[ $# -ne 3 ]]
then
	echo "incorrect argument count!"
	exit 1
fi

if [[ $(id -u) -ne 0 ]]
then
	echo "You must be root!"
	exit 2
fi

#directories check
function CheckDir
{
	if [[  !(-d "${1}") ]]
	then
		echo "no such directory!"
		exit 3
	fi
}

src="${1}"
dest="${2}"
str="${3}"

CheckDir $src
CheckDir $dest

if [[ -z "${str}" ]]
then
	exit 4
fi

SRC=$(dirname "${1}")"/"$(basename "${1}")
DEST=$(dirname "${2}")"/"$(basename "${2}")

for tocopy in $(find "${src}" -type f -name "*${str}*")
do
	echo "tocopy : ${tocopy}"
	DSTFILE=$(echo "${tocopy}" | tr $SRC $DEST)

	echo "DSTFILE : ${DSTFILE}"
	mkdir -p $(dirname "${DSTFILE}")
	cp "${tocopy}" "${DSTFILE}"

done

-- SI-2017-z3
зад. 3:
--------
Напишете скрипт, който ако се изпълнява от root потребителя:
- извежда обобщена информация за броя и общото количество активна
 памет (RSS - resident set size, non-swaped physical memory that a 
 task has used) на текущите процеси на всеки потребител
- ако процесът с най-голяма активна памет на даден потребител използва два пъти
повече памет от средното за потребителя, то скриптът да прекратява изпълнението
му по подходящ начин.

#!/bin/bash

# if [[ $(id -u) -ne 0 ]]
# then
# 	echo "you must be root!"
# 	exit 1
# fi

users=$(ps --no-headers -e -o user | sort | uniq)

for user in $users
do
	totalrss=0
	proccount=0
	maxpid=0
	maxrss=0

	while read PID RSS
	do
		maxpid="${PID}"
		maxrss="${RSS}"
		totalrss=$(( "${totalrss}" + "${RSS}" ))
		proccount=$(( "${proccount}" + 1 ))
	done < <(ps --no-headers -u "${user}" -o "pid rss" --sort +rss)

	#info printing
	echo "user : ${user}, totalrss : ${totalrss}, proccount : ${proccount}"

	if [[ "${proccount}" -eq 0 ]]
	then
		continue
	fi
	
	avgrss=$(( "${totalrss}" / "${proccount}" ))

	if [[ $(( "${avgrss}" * 2 )) -lt "${maxrss}" ]]
	then
		echo "procces ${maxpid} is consuming too much memory!"

		# kill -15 "${maxpid}"
		# sleep 3
		# kill -9 "${maxpid}"
	fi
done


-- SI-2017-z4
Задача 1:
Напишете shell script, който получава задължителен първи позиционен параметър - директория и незадължителен втори - име на файл.
Скриптът трябва да намира в подадената директория:
	- броя на symlink-овете, чийто destination не съществува
	- Името на симлинка "->" името на destination, ако съществува
Ако е подаден като аргумент файл - да се добавят в него, а ако не е - да се извеждат на екрана.

Примерен изход:
lbaz -> /foo/bar/baz
lqux -> ../../../qux
...
lquux -> /foo/quux
Broken symlinks: 34

#!/bin/bash

function CheckDir
{
	if [[ ! (-d "${1}") ]]
	then
		echo "${1} is not a dir!"
		exit 1
	fi
}


dir="${1}"

case $# in
	1)
		CheckDir "${dir}"
		#brokencount=$(find -L "${dir}" -maxdepth 1 -type l | wc -l)
		brokencount=$(find "${dir}" -maxdepth 1 -type l -exec test ! -e {} \; -print | wc -l)
		echo "broken links num : ${brokencount}"

		for f in $(ls "${dir}")
		do
			if [[ -h $f ]]
			then

				# if [[ $(file $f | grep -q -v "broken") -eq 0 ]]
				# then
				# 	ls -l  $f | tr -s ' ' | cut -d ' ' -f 9-
				# fi

				#or use the exit status of grep
				file $f | grep -q -v "broken" && ls -l  $f | tr -s ' ' | cut -d ' ' -f 9-	

			fi
		done

	;;
	2)
		CheckDir "${dir}"
		brokencount=$(find "${dir}" -maxdepth 1 -type l -exec test ! -e {} \; -print | wc -l)
		echo "broken links num : ${brokencount}" >> $2

		for f in $(ls "${dir}")
		do
			if [[ -h $f ]]
			then
				file $f | grep -q -v "broken" && ls -l  $f | tr -s ' ' | cut -d ' ' -f 9- >> $2
			fi
		done
		
	;;

	*)
		echo "bad argument count!"
		exit 2
	;;
esac



-- SI-2017-z5
Задача 2:
Напишете скрипт, който приема задължителен позиционен аргумент - име на потребител (FOO). Ако скриптът се изпълнява от root:
a) да извежда имената на потребителите, които имат повече на брой процеси от FOO, ако има такива;
б) да извежда средното време (в секунди), за което са работили процесите на всички потребители на системата (TIME);
в) ако съществуват процеси на FOO, които са работили над два пъти от средното време, скриптът да прекратява изпълнението им по подходящ начин

#!/bin/bash

if [[ $# -ne 1 ]]
then
	echo "need 1 argument!"
	exit 1
fi

# if [[ $(id -u) -ne 0 ]]
# then
# 	echo "must be root!"
# 	exit 2
# fi

foo=$1
foocount=$(ps --no-headers -u "${foo}" | wc -l)
users=$(ps --no-headers -e -o user | sort | uniq | grep -v "${foo}")

# echo $users
# echo "foocount : ${foocount}"

alltime=0
allcounter=0

#ps --no-headers -u "${users}" -o "etimes"
#exit 100

for usr in $users
do
	counter=0
	while read curretime 
	do
		alltime=$(( "${alltime}" + "${curretime}" ))
		allcounter=$(( "${allcounter}" + 1 ))
		count=$(( "${count}" + 1 ))

	done < <(ps --no-headers -u "${usr}" -o "etimes")

	if [[ "${count}" -gt "${foocount}" ]]
	then
		echo "user ${usr} has more proc than ${foo} ${count} > ${foocount}"
	fi
done

if [[ "${allcounter}" -gt 0 ]]
then
	avgtime=$(( "${alltime}" / "${allcounter}" ))
	echo "alltime is  : ${alltime}"
	echo "allcounter is : ${allcounter}"
	echo "avgtime is : ${avgtime}"
else
	echo "no processes found"
	exit 3
fi
ps --no-headers -u "${foo}" -o "pid etimes" | while read foopid fooetime
do
	if [[ "${fooetime}" -gt $(( 2 * "${avgtime}" )) ]]
	then
		echo "${fooetime} > 2 * ${avgtime}, killing process ${foopid}"

		# kill -15 "${foopid}"
		# sleep 2
		# kill -9 "${foopid}"

	fi
done

-- SI-2017-z6
Задача 3:
Напишете скрипт, който извежда името на потребителския акаунт, в чиято home директория има най-скоро променен обикновен файл и кой е този файл.
Напишете скрипта с подходящите проверки, така че да бъде валиден инструмент.




-- SI-2018-z3
Нека съществува програма за моментна комуникация (Instant messaging), която записва логове на разговорите
в следния формат:
	- има определена директория за логове (LOGDIR)
	- в нея има директорийна структура от следния вид:

		LOGDIR/протокол/акаунт/приятел/

	като на всяко ниво може да има няколко екземпляра от съответния вид,
	т.е. няколко директории *протокол*, във всяка от тях може да има няколко директории
	*акаунт*, и във всяка от тях -- няколко директории *приятел*.

	- във всяка от директориите *приятел* може да има файлове с имена от вида yyyy-mm-dd-hh-mm-ss.txt (година-месец-ден и т.н.,
	спрямо това кога е започнал даден разговор

	- всеки такъв файл представлява лог на даден разговор със съответния приятел, като всяка разменена реплика между вас
	е на отделен ред

	- даден идентификатор *приятел* може да се среща няколко пъти в структурата (напр. през различни ваши акаунти
	сте водили разговори със същия приятел)

Напишете скрипт, който приема задължителен позиционен аргумент - име на лог директория (LOGDIR). Скриптът трябва
да извежда десетимата приятели, с които имате най-много редове комуникация глобално (без значение протокол и акаунт),
и колко реда имате с всеки от тях.
