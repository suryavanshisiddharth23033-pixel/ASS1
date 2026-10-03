#!/usr/bin/env python3
"""
Parameter sweep: Plot execution time vs P_LOAD and P_STORE.
"""

import subprocess
import re
import csv
import os
import time

# Parameters
P_LOAD_VALUES = list(range(0, 101, 20))       # 0, 20, 40, 60, 80, 100
P_STORE_VALUES = list(range(0, 101, 20))      # 0, 20, 40, 60, 80, 100
TOTAL_INSTRUCTIONS = 100
EXECUTION_DELAY = 3
MEMORY_DELAY = 2
SIM_TIME = 1500
TRIALS = 1  # average over N trials

SITAR_FILE = "proc.sitar"
CSV_FILE = "results.csv"

def read_sitar_template():
    with open(SITAR_FILE, "r") as f:
        return f.write

def write_sitar(p_load, p_store):
    with open(SITAR_FILE, "r") as f:
        content = f.read()
    content = re.sub(
        r"parameter int P_LOAD = \d+",
        f"parameter int P_LOAD = {p_load}",
        content
    )
    content = re.sub(
        r"parameter int P_STORE = \d+",
        f"parameter int P_STORE = {p_store}",
        content
    )
    with open(SITAR_FILE, "w") as f:
        f.write(content)

def run_cmd(cmd):
    result = subprocess.run(
        cmd, shell=True, capture_output=True, text=True, timeout=60
    )
    return result.stdout + result.stderr

def translate_and_compile():
    out = run_cmd("sitar translate proc.sitar")
    if "Translation Failed" in out:
        return False
    out = run_cmd("sitar compile")
    if "FAILURE" in out:
        return False
    return True

def run_simulation():
    out = run_cmd(f"./sitar_sim {SIM_TIME}")
    match = re.search(r"Simulation stopped at time \((\d+),\d+\)", out)
    if match:
        return int(match.group(1))
    return None

def main():
    results = []
    total = len(P_LOAD_VALUES) * len(P_STORE_VALUES) * TRIALS
    count = 0

    print(f"Sweep: {len(P_LOAD_VALUES)} x {len(P_STORE_VALUES)} x {TRIALS} trials = {total} runs")
    print(f"{'P_LOAD':>6} {'P_STORE':>7} {'Avg Time':>9} {'Trials'}")
    print("-" * 40)

    for p_load in P_LOAD_VALUES:
        for p_store in P_STORE_VALUES:
            times = []
            for trial in range(TRIALS):
                count += 1
                write_sitar(p_load, p_store)
                if not translate_and_compile():
                    print(f"  [{count}/{total}] Compile FAILED for P_LOAD={p_load} P_STORE={p_store}")
                    times.append(None)
                    continue
                exec_time = run_simulation()
                times.append(exec_time)
                if exec_time is None:
                    print(f"  [{count}/{total}] Simulation FAILED for P_LOAD={p_load} P_STORE={p_store}")

            valid_times = [t for t in times if t is not None]
            avg_time = sum(valid_times) / len(valid_times) if valid_times else None
            results.append((p_load, p_store, avg_time, times))

            if avg_time is not None:
                print(f"{p_load:>6} {p_store:>7} {avg_time:>9.1f} {len(valid_times)}/{TRIALS}")
            else:
                print(f"{p_load:>6} {p_store:>7} {'FAIL':>9}")

    # Save to CSV
    with open(CSV_FILE, "w", newline="") as f:
        writer = csv.writer(f)
        writer.writerow(["P_LOAD", "P_STORE", "AVG_EXECUTION_TIME"] + [f"TRIAL{i+1}" for i in range(TRIALS)])
        for p_load, p_store, avg_time, times in results:
            writer.writerow([p_load, p_store, avg_time] + times)

    print(f"\nResults saved to {CSV_FILE}")
    print("Generating plots...")

    # Plot
    try:
        import matplotlib.pyplot as plt

        # Reshape results into grid
        n_load = len(P_LOAD_VALUES)
        n_store = len(P_STORE_VALUES)
        grid = [[None]*n_store for _ in range(n_load)]
        for p_load, p_store, avg_time, _ in results:
            i = P_LOAD_VALUES.index(p_load)
            j = P_STORE_VALUES.index(p_store)
            grid[i][j] = avg_time

        # Plot 1: Execution time vs P_LOAD for different P_STORE values
        fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 5))

        store_indices = [0, n_store//4, n_store//2, 3*n_store//4, n_store-1]
        colors = ["blue", "green", "red", "orange", "purple"]

        for idx, s_idx in enumerate(store_indices):
            p_store = P_STORE_VALUES[s_idx]
            times = [grid[l][s_idx] for l in range(n_load)]
            ax1.plot(P_LOAD_VALUES, times, "o-", color=colors[idx], label=f"P_STORE={p_store}")

        ax1.set_xlabel("P_LOAD")
        ax1.set_ylabel("Execution Time (cycles)")
        ax1.set_title("Execution Time vs P_LOAD")
        ax1.legend()
        ax1.grid(True)

        # Plot 2: Execution time vs P_STORE for different P_LOAD values
        load_indices = [0, n_load//4, n_load//2, 3*n_load//4, n_load-1]

        for idx, l_idx in enumerate(load_indices):
            p_load = P_LOAD_VALUES[l_idx]
            times = [grid[l_idx][s] for s in range(n_store)]
            ax2.plot(P_STORE_VALUES, times, "o-", color=colors[idx], label=f"P_LOAD={p_load}")

        ax2.set_xlabel("P_STORE")
        ax2.set_ylabel("Execution Time (cycles)")
        ax2.set_title("Execution Time vs P_STORE")
        ax2.legend()
        ax2.grid(True)

        plt.tight_layout()
        plt.savefig("execution_time_plot.png", dpi=150)
        plt.savefig("execution_time_plot.pdf")
        print("Plots saved: execution_time_plot.png, execution_time_plot.pdf")
        plt.show()

    except ImportError:
        print("matplotlib not installed. Run: pip install matplotlib")

if __name__ == "__main__":
    main()
