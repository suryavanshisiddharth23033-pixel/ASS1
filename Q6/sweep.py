import csv
import itertools
import os
import re
import subprocess

GRID = [0, 0.2, 0.4, 0.6, 0.8, 1.0]

rows = []
for s, t in itertools.product(GRID, GRID):
    env = os.environ.copy()
    env["SPATIAL_LOCALITY"] = str(s)
    env["TEMPORAL_LOCALITY"] = str(t)
    out = subprocess.run(["./sitar_sim", "100000"], env=env,
                         capture_output=True, text=True).stdout
    req, hits, misses = map(int, re.search(
        r"requests=(\d+) hits=(\d+) misses=(\d+)", out).groups())
    p_hit = float(re.search(r"p_hit=([0-9.]+)", out).group(1))
    rows.append((s, t, req, hits, misses, p_hit))
    print(f"spatial={s} temporal={t} p_hit={p_hit:.4f} ({hits}/{req})")

with open("results.csv", "w", newline="") as f:
    w = csv.writer(f)
    w.writerow(["spatial", "temporal", "requests", "hits", "misses", "p_hit"])
    w.writerows(rows)
print("wrote results.csv")
