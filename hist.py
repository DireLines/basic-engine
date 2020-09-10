# hist.py

import sys
from collections import Counter
from matplotlib import pyplot as plt

in_filename = sys.argv[1]
indicator = sys.argv[2]
with open(in_filename, 'r') as file:
    lines = file.readlines()
    plt.hist(
        list(
            map(float,#convert to number
                map(lambda s: s.split()[-1],#extract data point
                    filter(lambda s: indicator in s, lines)
                )
            )
        )
    )
    plt.show()