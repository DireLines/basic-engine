# test_bug.py
from collections import Counter
import subprocess
import sys
num_attempts = int(sys.argv[1] if len(sys.argv) > 1 else 20)
outputs = []
for i in range(num_attempts):
    try:
        out = subprocess.check_output("odin build .".split(),stderr=subprocess.STDOUT)
        outputs.append(out.decode('utf-8').strip())
    except subprocess.CalledProcessError as e:
        outputs.append(e.output.decode('utf-8').strip())
counts = Counter(outputs)
for output in counts.keys():
    print(counts[output],"occurrences of",output)