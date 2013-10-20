load "gr_common.gnu"

set output bdir.tdir."img_random_change".bext
set title "Random Change (Remove + Insert)".tsub
data = bdir.tdir.'dat_random_change.lst'

plot data using 1:2 title columnheader(2), \
	for [i=3:16] '' using 1:i title columnheader(i)
