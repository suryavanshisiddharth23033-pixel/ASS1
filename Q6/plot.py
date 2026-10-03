import csv

import matplotlib.pyplot as plt

rows = list(csv.DictReader(open("results.csv")))


def curve(param):
    other = "temporal" if param == "spatial" else "spatial"
    sel = [r for r in rows if float(r[other]) == 0]
    sel.sort(key=lambda r: float(r[param]))
    return [float(r[param]) for r in sel], [float(r["p_hit"]) for r in sel]


fig, ax = plt.subplots(1, 2, figsize=(11, 4))

x, y = curve("spatial")
ax[0].plot(x, y, "o-")
ax[0].set_xlabel("SPATIAL_LOCALITY")
ax[0].set_ylabel("P_HIT")
ax[0].set_title("P_HIT vs SPATIAL_LOCALITY (TEMPORAL_LOCALITY = 0)")
ax[0].grid(True)

x, y = curve("temporal")
ax[1].plot(x, y, "o-")
ax[1].set_xlabel("TEMPORAL_LOCALITY")
ax[1].set_ylabel("P_HIT")
ax[1].set_title("P_HIT vs TEMPORAL_LOCALITY (SPATIAL_LOCALITY = 0)")
ax[1].grid(True)

fig.tight_layout()
fig.savefig("plot_sweep.png")
print("wrote plot_sweep.png")
