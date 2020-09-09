# hist.py

import sys
from collections import Counter

def ascii_histogram(seq) -> None:
    if(len(list(seq))<1):
        return
    counted = Counter(seq)
    max_counted = next(iter(counted.values()))
    for k in counted:
        max_counted = max(max_counted, counted[k])
    for k in sorted(counted):
        print('{0:5d} {1}'.format(k, '+' * round(1000 * counted[k] / max_counted)))

in_filename = sys.argv[1]
indicator = sys.argv[2]
with open(in_filename, 'r') as file:
    lines = file.readlines()
    ascii_histogram(
        map(int,#convert to int
            map(lambda s: s.split()[-1],#extract data point
                filter(lambda s: s.startswith(indicator), lines)
                )
            )
        )