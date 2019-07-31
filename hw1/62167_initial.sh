cat text.txt | \
	tr -d [:digit:] | \
	sed 's/ - /-/g' | \
	tr '-' ' '  | \
	tr '/' ' ' | \
	sed -r 's/[[:punct:]]//g' | \
	tr ' '  '\n'  | \
	gawk '{print tolower($0);}' | \
	sort | \
	uniq | \
	xargs -I'{}' sh -c 'printf "{}:\n"; tre-agrep -B -s -w {} dic.txt | head -n 1' | \
	xargs -d '\n' -n 2 | \
	tr -d ' ' | \
	grep -v ":0:"