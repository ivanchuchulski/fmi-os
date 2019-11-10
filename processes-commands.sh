ps (information about the processes)
	-elf
	--no-headers := remove first row of headers
	-o : user specified format, i.e. -o "comm rss vsz"
		args := the command and its arguments that invoked the process
		comm := only the command, without arguments
	
		bsdstart := time the command started; if time was <24h then format is "HH:MM"
		lstart := -//-
		start := -//-
			if (proc started < 24h)	=> "HH::MM::SS"
			else 					=> "Mmm dd"
		start_time := starting time or date of the process
			if (!started in the same year)		=> only the year will be displayed
			else if (!started in the same day)	=> "MmmDD", Mmm is a three letter month name
			else  								=> "HH:MM"
		
		etime := elapsed time since process started in format [[DD-]hh:]mm::ss
		etimes := elapsed time since process started, in seconds
		rss := comsumed RAM in kilobytes
		vsz := consumed virtual memory in Kib (1024 Byte units)	
		
		pid := process id
		pri := priority of the process, higher numbers mean lower priority
		ppid := parent process id
		
	--sort=-id,+pid			- := decsreasing order, + := increasing order
	-U userlist  := select processes by userid
	-G grouplist := select processes by groupid
	-p select by pid
	-t ttylist := select by terminal
	