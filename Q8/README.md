# Q8: Set-Associative Cache Architecture

## Overview

This implementation extends the cache model from Q7 to support set-associative cache architecture. The cache is parameterized with three key parameters:

- **NUM_SETS**: Number of sets in the cache (must be power of 2)
- **NUM_WAYS**: Number of blocks per set, also called associativity (must be power of 2)
- **BLOCK_SIZE**: Bytes per cache block (must be power of 2)

**Total cache capacity** = NUM_SETS × NUM_WAYS × BLOCK_SIZE

## Address Decomposition

For a given memory address, the bits are interpreted as:

```
|     TAG      |   SET INDEX   |  BYTE OFFSET  |
|--------------|---------------|---------------|
|   log2(NUM_SETS × BLOCK_SIZE)  | log2(NUM_SETS) | log2(BLOCK_SIZE) |
```

- **Byte Offset** (lower log2(BLOCK_SIZE) bits): Position within a cache block
- **Set Index** (next log2(NUM_SETS) bits): Which set the address maps to
- **TAG** (remaining upper bits): Identifies which memory block

## Implementation Details

### Key Functions

```cpp
// Extract TAG from address
uint32_t get_tag(uint32_t addr) {
    return addr / (BLOCK_SIZE * NUM_SETS);
}

// Extract set index from address
uint32_t get_set_index(uint32_t addr) {
    return (addr / BLOCK_SIZE) % NUM_SETS;
}

// Extract byte offset from address
uint32_t get_offset(uint32_t addr) {
    return addr % BLOCK_SIZE;
}
```

### Cache Lookup Algorithm

1. Extract set index from address
2. Extract TAG from address
3. Check all NUM_WAYS blocks in the target set
4. If any block has VALID=1 and matching TAG → **HIT**
5. Otherwise → **MISS**

### Replacement Policy

On a miss when all ways in the target set are valid:
- **Random replacement**: Select a random way within the set to evict
- This is a simplified policy for demonstration (real caches use LRU, PLRU, etc.)

### Write Policy

- **Write-through**: Stores go directly to memory
- No write-back or dirty bits (simplified model)

## Configuration Files

### 1. Set-Associative Cache (4-way, 4 sets)
**File**: `model.sitar`

```
ICACHE: 4 sets × 4 ways × 16 bytes = 256 bytes total
DCACHE: 2 sets × 8 ways × 16 bytes = 256 bytes total
```

This is a balanced configuration offering good hit rates with moderate hardware complexity.

### 2. Direct-Mapped Cache
**File**: `model_direct_mapped.sitar`

```
NUM_SETS = 16, NUM_WAYS = 1
16 sets × 1 way × 16 bytes = 256 bytes total
```

Characteristics:
- Each address maps to exactly one cache block
- Simplest hardware implementation
- Fastest lookup (check only one location)
- **Problem**: Conflict misses occur when multiple addresses map to same set
- Lower hit rate for programs with poor locality

### 3. Fully Associative Cache
**File**: `model_fully_associative.sitar`

```
NUM_SETS = 1, NUM_WAYS = 16
1 set × 16 ways × 16 bytes = 256 bytes total
```

Characteristics:
- Any block can go anywhere in cache
- No conflict misses (only capacity misses)
- Best hit rate for any access pattern
- **Problem**: Must search all ways on every access
- Higher hardware complexity and power consumption

## Performance Comparison

| Configuration | NUM_SETS | NUM_WAYS | Lookup Complexity | Conflict Misses | Hardware Cost |
|---------------|----------|----------|-------------------|-----------------|---------------|
| Direct-Mapped | 16       | 1        | O(1) - check 1 way | High           | Lowest        |
| 4-Way Set-Assoc | 4      | 4        | O(4) - check 4 ways | Medium         | Medium        |
| 8-Way Set-Assoc | 2      | 8        | O(8) - check 8 ways | Low            | Higher        |
| Fully Associative | 1    | 16       | O(16) - check all | None           | Highest       |

### Expected Behavior

**Direct-Mapped (NUM_WAYS = 1)**:
- Fastest lookup (single comparison)
- Suffers from conflict misses
- Two addresses with same set index but different tags will evict each other
- Example: Addresses 0x00 and 0x100 both map to set 0

**Fully Associative (NUM_SETS = 1)**:
- No conflict misses - any block can hold any address
- Only capacity misses (when cache is full)
- Must compare all ways on every access
- Best hit rate but highest latency and power

**Set-Associative (middle ground)**:
- Balances hit rate and hardware complexity
- Conflict misses limited to addresses mapping to same set
- 4-way and 8-way are common in real processors
- Good trade-off for most workloads

## Running the Simulation

Each configuration can be run independently:

```bash
# Run set-associative cache
sitar model.sitar

# Run direct-mapped cache
sitar model_direct_mapped.sitar

# Run fully associative cache
sitar model_fully_associative.sitar
```

Environment variables for tuning locality:
```bash
# High temporal locality (reuse same addresses)
TEMPORAL_LOCALITY=0.9 SPATIAL_LOCALITY=0.5 sitar model.sitar

# High spatial locality (nearby addresses)
TEMPORAL_LOCALITY=0.5 SPATIAL_LOCALITY=0.9 sitar model.sitar
```

## Observed Differences

### With High Temporal Locality
- Fully associative performs best - frequently used blocks stay in cache
- Direct-mapped may still evict useful blocks due to conflicts

### With High Spatial Locality
- All configurations benefit (blocks contain nearby addresses)
- Set-associative provides good balance

### With Random Access Pattern
- Fully associative shows advantage
- Direct-mapped has more conflict misses

## Code Structure

The cache procedure is parameterized to support any configuration:

```sitar
procedure Cache
    parameter int NUM_SETS = 4    // Configurable
    parameter int NUM_WAYS = 4    // Configurable
    parameter int BLOCK_SIZE = 16 // Configurable
    parameter int CACHE_DELAY = 4
    ...
end procedure
```

The same cache procedure is instantiated with different parameters in Core module:

```sitar
// 4-way set-associative
procedure icache : Cache<4, 4, 16>

// Direct-mapped (NUM_WAYS = 1)
procedure icache : Cache<16, 1, 16>

// Fully associative (NUM_SETS = 1)
procedure icache : Cache<1, 16, 16>
```

## Validation

The implementation was validated with:

1. **Boundary cases**:
   - NUM_WAYS = 1 (direct-mapped) ✓
   - NUM_SETS = 1 (fully associative) ✓

2. **Parameter constraints**:
   - All parameters are powers of 2 ✓
   - Address decomposition works correctly ✓

3. **Behavior verification**:
   - Correct set selection from address ✓
   - Proper tag matching across all ways ✓
   - Random replacement when set is full ✓
