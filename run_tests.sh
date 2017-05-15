#!/bin/bash

make --quiet || exit 1

hash colordiff 2>/dev/null && DIFF=colordiff || DIFF=diff

for in in ${@:-tests/*.txt} ; do
	[[ ! -e $in ]] && echo -e "\e[31mUnexistent test file $in\e[0m" && exit 1
	tst="${in##*/}"
	out="tests/${tst}.out"
	echo -en "\e[1;38;5;215mRunning $in \e[0m > $out  "
	./project <$in >$out
	output=$(diff -q $out "tests/expected-output/$tst")
	if [[ $output ]]; then
		echo -e "[ \e[1;38;5;196mFailed\e[0m ]"
		echo -e "     \e[1mTest                                                        Expected\e[0m"
		$DIFF -y <(nl $out) <(nl "tests/expected-output/$tst")
	else
		echo -e "[ \e[1;38;5;41mSucceded\e[0m ]"
	fi

	[[ -e time.log ]] && \
		echo -e "\t\e[1mTimes:" && \
		cat time.log && \
		echo -e "\e[0m"
done

make --quiet clean
rm tests/*.out
[[ -e time.log ]] && rm time.log
