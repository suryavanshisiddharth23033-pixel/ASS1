import matplotlib.pyplot as plt

p_hit = [0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100]
amat_cycles = [42, 39, 35, 32, 28, 25, 22, 20, 17, 11, 6]

plt.plot(p_hit, amat_cycles)
plt.title("P_HIT vs AMAT")
plt.xlabel("P_HIT")
plt.ylabel("AMAT (Cycles)")
plt.savefig("plot.png")
