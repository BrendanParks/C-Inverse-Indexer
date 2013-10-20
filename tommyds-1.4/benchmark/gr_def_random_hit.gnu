load "gr_common.gnu"

tdir = "def/"
tsub = "\nCore i5 650 3.20 GHz, 4 MB L3 cache, 2400 Uncore Speed\nLinux, gcc 4.7.1, 32 bit"

set output bdir.tdir."img_random_hit".bext
set title "Random Hit".tsub
data = bdir.tdir.'dat_random_hit.lst'

plot data using 1:2 title columnheader(2), \
	for [i=3:16] '' using 1:i title columnheader(i)


