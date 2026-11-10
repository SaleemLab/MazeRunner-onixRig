"""Generate the gain-zone trial CSVs for CorridorVR_Two_tracks_gain_zone.bonsai.

Columns 1-8 are the usual START_LOCATION columns; 9-11 are
gain_change_start_location, gain_change_length, gain_change
(start is measured from the lap start, so 70 on Track 2 = MousePos 1070).

Block design (all four files share the same block lengths):
  block 1: manipulated track, normal gain, 12 laps
  then alternating other track (normal) / manipulated track (gain zone),
  each block 8-13 laps (pseudo-random), 100 laps in total.

  START_LOCATION_gain_zone.CSV            T1 manipulated, zone 70-120 cm, gain 0.5
  START_LOCATION_gain_zone_t2.CSV         T2 manipulated, zone 70-120 cm, gain 0.5
  START_LOCATION_gain_zone_random.CSV     T1 manipulated, 50 cm zone, gain 0.5, start 10-90 cm random each lap
  START_LOCATION_gain_zone_random_t2.CSV  T2 manipulated, same as above

Run:  python make_gain_zone_csvs.py
"""
import os
import random

N_LAPS = 100
FIRST_BLOCK = 12
BLOCK_MIN, BLOCK_MAX = 8, 13
ZONE_START, ZONE_LENGTH, GAIN = 70, 50, 0.5
RANDOM_START_MIN, RANDOM_START_MAX = 10, 90
SEED = 2026

TRACK = {1: (0, 100), 2: (1000, 1100)}  # track id -> (start position, column 2)
GREY = "200,-10000,5,0,0,0,0,20000000,0,0,1"
HERE = os.path.dirname(os.path.abspath(__file__))


def block_lengths(rng):
    """First block 12 laps, then 8-13 lap blocks that add up to N_LAPS exactly."""
    remaining = N_LAPS - FIRST_BLOCK
    while True:
        blocks, total = [], 0
        while total < remaining:
            b = rng.randint(BLOCK_MIN, BLOCK_MAX)
            blocks.append(b)
            total += b
        if total == remaining:
            return [FIRST_BLOCK] + blocks


def write(name, manip_track, blocks, zone_starts=None):
    other = 2 if manip_track == 1 else 1
    rows, lap = [GREY], 0
    for i, n in enumerate(blocks):
        if i % 2 == 0:
            track = manip_track
            manipulated = i > 0  # the first block is normal gain
        else:
            track, manipulated = other, False
        for _ in range(n):
            lap += 1
            if manipulated:
                start = zone_starts[lap - 1] if zone_starts else ZONE_START
                zone = f"{start},{ZONE_LENGTH},{GAIN}"
            else:
                zone = "0,0,1"
            pos, col2 = TRACK[track]
            rows.append(f"{pos},{col2},5,{track},{lap},10,1,20000000,{zone}")
    with open(os.path.join(HERE, name), "w", newline="") as f:
        f.write("\n".join(rows) + "\n")


rng = random.Random(SEED)
blocks = block_lengths(rng)
random_starts = [rng.randint(RANDOM_START_MIN, RANDOM_START_MAX) for _ in range(N_LAPS)]

write("START_LOCATION_gain_zone.CSV", 1, blocks)
write("START_LOCATION_gain_zone_t2.CSV", 2, blocks)
write("START_LOCATION_gain_zone_random.CSV", 1, blocks, random_starts)
write("START_LOCATION_gain_zone_random_t2.CSV", 2, blocks, random_starts)
print("blocks:", blocks)
