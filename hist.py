# hist.py

import sys
from matplotlib import pyplot as plt

indicator = sys.argv[1]

plt.hist(
    list(
        map(float,#convert to number
            map(lambda s: s.split()[-1],#extract data point
                filter(lambda s: indicator in s, sys.stdin)
            )
        )
    )
)
plt.show()