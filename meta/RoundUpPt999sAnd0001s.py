import re

def round_near_int(val_str, threshold=1e-3):
    """Round float string to nearest integer if very close (within 0.001)"""
    try:
        val = float(val_str)
        rounded = round(val)
        if abs(val - rounded) < threshold:
            return str(int(rounded))
        # Otherwise return with 3 decimal places max, strip trailing zeros
        return f"{val:.3f}".rstrip('0').rstrip('.')
    except:
        return val_str

# Read OBJ file
with open('All29PentacubesMinPts.obj', 'r') as f:
    lines = f.readlines()

# Process lines
cleaned_lines = []
for line in lines:
    # Match vertex lines (v x y z), normal lines (vn x y z), texture lines (vt u v)
    if line.startswith('v ') or line.startswith('vn ') or line.startswith('vt '):
        parts = line.strip().split()
        prefix = parts[0]
        coords = [round_near_int(p, threshold=1e-3) for p in parts[1:]]
        cleaned_lines.append(f"{prefix} {' '.join(coords)}\n")
    else:
        cleaned_lines.append(line)

# Write back
with open('All29PentacubesMinPts.obj', 'w') as f:
    f.writelines(cleaned_lines)

print("✓ Rounded all coordinates in All29PentacubesMinPts.obj (threshold: 0.001)")