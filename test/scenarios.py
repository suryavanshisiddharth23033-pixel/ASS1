from __future__ import annotations

from dataclasses import dataclass, field
from typing import Dict, Tuple


BASE_ENV: Dict[str, str] = {
    "P_LOAD": "40",
    "P_STORE": "30",
    "TEMPORAL_LOCALITY": "0.7",
    "SPATIAL_LOCALITY": "0.6",
    "MEMORY_DELAY": "40",
    "NON_BLOCKING": "1",
    "REPL_POLICY": "1",
    "WRITE_POLICY": "0",
    "DET_RNG": "1",
}


@dataclass(frozen=True)
class Scenario:
    scenario_id: str
    description: str
    env: Dict[str, str] = field(default_factory=dict)
    tags: Tuple[str, ...] = ()

    def resolved_env(self) -> Dict[str, str]:
        merged = dict(BASE_ENV)
        merged.update(self.env)
        return merged


SCENARIOS: Tuple[Scenario, ...] = (
    Scenario(
        "balanced",
        "Balanced 40% load / 30% store baseline",
        {},
        ("baseline", "performance"),
    ),
    Scenario(
        "global_rng",
        "Balanced workload with the legacy global RNG",
        {"DET_RNG": "0"},
        ("rng",),
    ),
    Scenario(
        "read_heavy",
        "Read-heavy 70% load / 10% store",
        {"P_LOAD": "70", "P_STORE": "10"},
        ("workload",),
    ),
    Scenario(
        "read_only",
        "Load-only 70% load / 0% store",
        {"P_LOAD": "70", "P_STORE": "0"},
        ("workload",),
    ),
    Scenario(
        "store_heavy",
        "Store-heavy 10% load / 80% store",
        {"P_LOAD": "10", "P_STORE": "80"},
        ("workload",),
    ),
    Scenario(
        "load_extreme",
        "Every instruction is a load",
        {"P_LOAD": "100", "P_STORE": "0"},
        ("workload", "stress"),
    ),
    Scenario(
        "store_extreme",
        "Every instruction is a store",
        {"P_LOAD": "0", "P_STORE": "100"},
        ("workload", "stress"),
    ),
    Scenario(
        "atomic_heavy",
        "Atomic-heavy 80% load / 80% store",
        {"P_LOAD": "80", "P_STORE": "80"},
        ("atomic", "stress"),
    ),
    Scenario(
        "atomic_extreme",
        "Every instruction is an atomic",
        {"P_LOAD": "100", "P_STORE": "100"},
        ("atomic", "stress"),
    ),
    Scenario(
        "no_data_ops",
        "No load/store/atomic traffic",
        {"P_LOAD": "0", "P_STORE": "0"},
        ("icache",),
    ),
    Scenario(
        "balanced_blocking",
        "Balanced workload with blocking caches",
        {"NON_BLOCKING": "0"},
        ("blocking",),
    ),
    Scenario(
        "pingpong_t3a",
        "Store-heavy high-temporal-locality ping-pong",
        {"P_LOAD": "35", "P_STORE": "55", "TEMPORAL_LOCALITY": "0.85"},
        ("coherence",),
    ),
    Scenario(
        "pingpong_blocking",
        "Ping-pong with blocking caches",
        {
            "P_LOAD": "35",
            "P_STORE": "55",
            "TEMPORAL_LOCALITY": "0.85",
            "NON_BLOCKING": "0",
        },
        ("coherence", "blocking"),
    ),
    Scenario(
        "high_locality",
        "High temporal and spatial locality",
        {"TEMPORAL_LOCALITY": "0.9", "SPATIAL_LOCALITY": "0.9"},
        ("locality",),
    ),
    Scenario(
        "low_locality",
        "Low temporal and spatial locality (0.2)",
        {"TEMPORAL_LOCALITY": "0.2", "SPATIAL_LOCALITY": "0.2"},
        ("locality",),
    ),
    Scenario(
        "fast_memory",
        "Fast memory (MEMORY_DELAY=5)",
        {"MEMORY_DELAY": "5"},
        ("memory",),
    ),
    Scenario(
        "slow_memory",
        "Slow memory (MEMORY_DELAY=80)",
        {"MEMORY_DELAY": "80"},
        ("memory",),
    ),
    Scenario(
        "random_repl",
        "Random replacement policy",
        {"REPL_POLICY": "0"},
        ("cache-policy",),
    ),
)


def get_scenario(scenario_id: str) -> Scenario:
    for scenario in SCENARIOS:
        if scenario.scenario_id == scenario_id:
            return scenario
    raise KeyError(scenario_id)


def env_text(env: Dict[str, str]) -> str:
    return " ".join(f"{key}={value}" for key, value in env.items())
