#!/bin/bash

#check for correct argument count
if [[ "${#}" -ne 4 ]]
then
	echo "Error : incorrect argument count!"
	exit 1
fi

logfile="${1}"
eggfile="${2}"
larfile="${3}"
outdir="${4}"
PERIODLIMIT="0.2"
timezone=''

if [[ !(-e "${logfile}" ) ]]
then
	echo "Error : Log file does not exist!"
	exit 2
fi

if [[ !(-e "${eggfile}" ) ]]
then
	echo "Error : EEG file does not exist!"
	exit 3
fi

if [[ !(-e "${larfile}" ) ]]
then
	echo "Error : Larynx file file does not exist!"	
	exit 4
fi

#check for correct directory
if [[ !(-d "${outdir}") ]]
then
	echo "Error : No correct directory provided!"
	exit 5
fi

#checking for stimulus apearing more than once
while read count
do
	if [[ "${count}" -gt 1 ]]
	then 
		echo "Error : Dublicate stimulus found!"
		exit 6
	fi
done < <(cat "${logfile}" | cut -d ' ' -f 1 | sort | uniq -c | awk '{print $1}')

#date in xml files is in format dd.mm.yyyy
startdate=$(grep "StartRecordingDate" "${eggfile}" | cut -d '<' -f 2 | cut -d '>' -f 2)
starthour=$(grep "StartRecordingTime" "${eggfile}" | cut -d '<' -f 2  | cut -d '>' -f 2)
samplingrate=$(grep "SamplingRate" "${eggfile}" | cut -d '<' -f 2 | cut -d '>' -f 2 | cut -d ' ' -f 1 )
wavsecsduration=$(grep "RecordLength" "${eggfile}" | cut -d '<' -f 2 | cut -d '>' -f 2 | awk -F ':' '{print $1 * 3600 +  $2 * 60 + $3}')
wavstart=$(grep "beep" "${logfile}" | cut -d ' ' -f 3 | cut -d '.' -f 1)

#modifying the date to the format mm/dd/yyyy
startdate=$(echo "${startdate}" | awk -F '.' '{print $2"/"$1"/"$3}')
wholedate=$(echo "${startdate}" "${starthour}")

#taking into consideration the time zone when calculating the hours
env | grep -q "EEG_TZ" && timezone="${EEG_TZ}" || timezone="UTC"

#another way to determine the timezone
# if [[ -n "${EEG_TZ}" ]]
# then
# 	timezone=$EEG_TZ
# else
# 	timezone="UTC"
# fi

#the samples in the given files are using a timezone Europe/Sofia, so for testing we set the timezone to that
#timezone="Europe/Sofia"

eegstart=$(TZ="${timezone}" date --date="${wholedate}" +%s)

tocutrows=0
rowcounter=0
soundcounter=0
tickrowsnum=$(grep "tick" "${eggfile}" | wc -l)

#extracting data from the logfile and creating the xml and wav parts for each stimulus
cat "${logfile}" | while read stimulus begtime endtime
do

	#check for data correctness
	#when comparing bc returns 1 for true and 0 for false
	if [[ $(echo "${endtime} < ${begtime}" | bc -l) -eq 1 ]]
	then
		echo "Error : endtime is before beginning time!"
		exit 7
	fi

	if [[ $(echo "${begtime} < ${eegstart}" | bc -l) -eq 1 ]]
	then
		echo "not reached eeg start yet!"
		continue
	fi

	if [[ $(echo "${begtime} < ${wavstart}" |  bc -l) -eq 1 ]]
	then
		echo "not reached wav file start yet!"
		continue
	fi

	delta=$(echo "${endtime} - ${begtime}" | bc -l)

	if [[ $(echo "$delta < $PERIODLIMIT" | bc -l) -eq 1  ]]
	then
		echo "Stimulus duration is too short!"
		continue
	fi

	######################cut ticks from xml file######################

	rowcounter=$(echo "((${begtime} - ${eegstart})/1)*${samplingrate}" | bc)

	tocutrows=$(echo "(${delta} * ${samplingrate})/1" | bc)

	if [[ $(echo "(${tocutrows} + ${rowcounter}) > ${tickrowsnum}" | bc -l ) -eq 1 ]]
	then
		echo "Error : tick rows number exceeded!"
		exit 8
	fi

	grep "tick" eeg.xml | cut -d '<' -f 2 | cut -d '>' -f 2 | tail -n  +"${rowcounter}" | head -n "${tocutrows}" > "${outdir}"/"${stimulus}""_eeg.xml"

	######################cut the audio######################

	soundcounter=$(echo "((${begtime} - ${wavstart})/1)" | bc)

	if [[ $(echo "((${soundcounter} + ${delta})/1) > ${wavsecsduration}" | bc -l) -eq 1 ]]
	then
		echo "Error : Length of stimulus exceeds the length of the wav file!"
		exit 9
	fi

	sox "${larfile}" "${outdir}"/"${stimulus}""_lar.wav" trim "${soundcounter}" "${delta}"

	######some checks for debugging######

	#echo "${stimulus}" "${begtime}" "${endtime}"

	# echo "tocutrows : ${tocutrows}"
	# echo "rowcounter : ${rowcounter}"
	# echo "${tocutrows} + ${rowcounter}" | bc -l

	# echo "delta : ${delta}"
	# echo "soundcounter : ${soundcounter}"
	# echo "(${soundcounter} + ${delta})/1" | bc

done

