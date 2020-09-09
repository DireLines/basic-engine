# hist.py

import sys
from collections import Counter

def ascii_histogram(seq) -> None:
    counted = Counter(seq)
    max_counted = max(counted.values())
    max_line_length = 200
    relative_display = False
    if(max_counted>max_line_length):
        relative_display = True
    for k in sorted(counted):
        display_num = counted[k]
        if(relative_display):
            display_num = round(max_line_length * counted[k] / max_counted)
        print('{0:5d} {1}'.format(k, '+' * display_num))

in_filename = sys.argv[1]
indicator = sys.argv[2]
with open(in_filename, 'r') as file:
    lines = file.readlines()
    ascii_histogram(
        map(int,#convert to int
            map(lambda s: s.split()[-1],#extract data point
                filter(lambda s: indicator in s, lines)
                )
            )
        )