import json
import math

def are_parallel_or_perpendicular(v1, v2):
    """Check if edge is parallel to one of the main axes (cube edge)"""
    dx = abs(v1[0] - v2[0])
    dy = abs(v1[1] - v2[1])
    dz = abs(v1[2] - v2[2])
    
    # Count how many axes have non-zero difference
    non_zero = sum([1 if d > 0.001 else 0 for d in [dx, dy, dz]])
    
    # Cube edges only differ in exactly ONE axis
    # Diagonals differ in 2 or 3 axes
    return non_zero == 1

def generate_cube_edges_only(vertices, faces):
    """Generate only edges parallel to axes (no diagonals)"""
    edge_set = set()
    
    # Collect all edges from faces
    for face in faces:
        # Handle both list of indices and dict with 'vertices' key
        if isinstance(face, dict):
            face_verts = face['vertices']
        else:
            face_verts = face
            
        for i in range(len(face_verts)):
            v1_idx = face_verts[i]
            v2_idx = face_verts[(i + 1) % len(face_verts)]
            
            v1 = vertices[v1_idx]
            v2 = vertices[v2_idx]
            
            # Only add if it's a cube edge (parallel to axis)
            if are_parallel_or_perpendicular(v1, v2):
                edge = tuple(sorted([v1_idx, v2_idx]))
                edge_set.add(edge)
    
    # Convert to flat list
    edges = []
    for v1, v2 in sorted(edge_set):
        edges.extend([v1, v2])
    
    return edges

def round_coord(val, threshold=1e-5):
    """Round coordinates close to integers"""
    if abs(val) < threshold:
        return 0
    rounded = round(val)
    if abs(val - rounded) < threshold:
        return int(rounded)
    return val

# Read the JSON from stdin or file
with open('pentacubes.json', 'r') as f:
    data = json.load(f)

# Process each pentacube
pentacubes_ordered = ['A', 'B', 'E', 'E\'', 'F', 'G', 'G\'', 'H', 'H\'', 'I', 'J', 'J\'', 'K', 'L', 'M', 'N', 'P', 'Q', 'R', 'R\'', 'S', 'S\'', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z']

pentacubes_processed = {}

for name in pentacubes_ordered:
    obj = data[name]
    origin = obj['origin']
    vertices = obj['vertices']
    faces = obj['faces']
    
    # Subtract origin and round
    adjusted_verts = []
    for v in vertices:
        adjusted_verts.append([
            round_coord(v[0] - origin[0]),
            round_coord(v[1] - origin[1]),
            round_coord(v[2] - origin[2])
        ])
    
    # Find min coords to shift everything positive
    min_x = min(v[0] for v in adjusted_verts)
    min_y = min(v[1] for v in adjusted_verts)
    min_z = min(v[2] for v in adjusted_verts)
    
    positive_verts = []
    for v in adjusted_verts:
        positive_verts.append([
            int(v[0] - min_x),
            int(v[1] - min_y),
            int(v[2] - min_z)
        ])
    
    pentacubes_processed[name] = {
        'vertices': positive_verts,
        'faces': faces
    }

# Generate C code
print("// Auto-generated pentacube data from Blender export")
print("#include \"pentacubes.h\"")
print()

# Generate vertex arrays
for name in pentacubes_ordered:
    enum_name = name.replace("'", "_p")
    verts = pentacubes_processed[name]['vertices']
    print(f"static const float vertices_{enum_name}[][3] = {{")
    for v in verts:
        print(f"    {{{v[0]:.1f}f, {v[1]:.1f}f, {v[2]:.1f}f}},")
    print("};")
    print()

# Generate edge arrays (FILTERED)
for name in pentacubes_ordered:
    enum_name = name.replace("'", "_p")
    vertices = pentacubes_processed[name]['vertices']
    faces = pentacubes_processed[name]['faces']
    
    edges = generate_cube_edges_only(vertices, faces)
    
    print(f"static const int edges_{enum_name}[] = {{")
    for i in range(0, len(edges), 16):
        line = ', '.join(str(e) for e in edges[i:i+16])
        print(f"    {line},")
    print("};")
    print()

# Generate face vertex arrays
for name in pentacubes_ordered:
    enum_name = name.replace("'", "_p")
    faces = pentacubes_processed[name]['faces']
    print(f"static const int face_vertices_{enum_name}[] = {{")
    for face in faces:
        if isinstance(face, dict):
            verts = face['vertices']
        else:
            verts = face
        line = ', '.join(str(v) for v in verts)
        print(f"    {line},")
    print("};")
    print()

# Generate pentacube_data array
print("const pentacube_data_t pentacube_data[PENTACUBE_COUNT] = {")

for name in pentacubes_ordered:
    enum_name = name.replace("'", "_p")
    verts = pentacubes_processed[name]['vertices']
    vertices = pentacubes_processed[name]['vertices']
    faces = pentacubes_processed[name]['faces']
    
    edges = generate_cube_edges_only(vertices, faces)
    edge_count = len(edges) // 2
    face_count = len(faces)
    
    print(f"    {{")
    print(f"        .vertices = (const float *)vertices_{enum_name},")
    print(f"        .vertex_count = {len(verts)},")
    print(f"        .edges = edges_{enum_name},")
    print(f"        .edge_count = {edge_count},")
    print(f"        .face_vertices = face_vertices_{enum_name},")
    print(f"        .face_count = {face_count},")
    print(f"        .name = \"{name}\"")
    print(f"    }},")

print("};")

# Print stats
print("\n// Statistics:", file=__import__('sys').stderr)
for name in pentacubes_ordered:
    vertices = pentacubes_processed[name]['vertices']
    faces = pentacubes_processed[name]['faces']
    edges = generate_cube_edges_only(vertices, faces)
    print(f"//   {name:8s}: {len(edges)//2:2d} edges (filtered)", file=__import__('sys').stderr)