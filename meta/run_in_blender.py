import json
import bpy
import math
from pathlib import Path

def are_parallel_or_perpendicular(v1, v2):
    """Check if edge is parallel to one of the main axes (cube edge)"""
    dx = abs(v1[0] - v2[0])
    dy = abs(v1[1] - v2[1])
    dz = abs(v1[2] - v2[2])
    
    non_zero = sum([1 if d > 0.001 else 0 for d in [dx, dy, dz]])
    return non_zero == 1

def round_coord(val, threshold=1e-5):
    """Round coordinates close to integers"""
    if abs(val) < threshold:
        return 0
    rounded = round(val)
    if abs(val - rounded) < threshold:
        return int(rounded)
    return val

def compute_face_normal(vertices, vertex_indices):
    """Compute face normal using Newell's method (works for non-planar polygons)"""
    normal = [0, 0, 0]
    n = len(vertex_indices)
    
    for i in range(n):
        v1 = vertices[vertex_indices[i]]
        v2 = vertices[vertex_indices[(i + 1) % n]]
        
        normal[0] += (v1[1] - v2[1]) * (v1[2] + v2[2])
        normal[1] += (v1[2] - v2[2]) * (v1[0] + v2[0])
        normal[2] += (v1[0] - v2[0]) * (v1[1] + v2[1])
    
    # Normalize
    length = math.sqrt(sum(n*n for n in normal))
    if length > 0.001:
        normal = [n / length for n in normal]
    
    return normal

def ensure_ccw_winding(vertices, vertex_indices, blender_normal):
    """
    Ensure face vertices are in CCW order when viewed from outside.
    Uses Blender's face normal as reference.
    """
    # Compute current winding normal using Newell's method
    computed_normal = compute_face_normal(vertices, vertex_indices)
    
    # Dot product with Blender's normal
    dot = sum(computed_normal[i] * blender_normal[i] for i in range(3))
    
    # If dot < 0, vertices are in wrong order
    if dot < -0.1:
        vertex_indices = list(reversed(vertex_indices))
    
    return vertex_indices

# ===== PASS 1: Export and Collect Metadata =====

blend_file = Path(bpy.data.filepath).absolute()
script_dir = blend_file.parent
output_file = script_dir.parent / "main" / "pentacubes.c"

print(f"Blend file: {blend_file}")
print(f"Output file: {output_file}\n")

output_file.parent.mkdir(parents=True, exist_ok=True)

# Export from Blender
blender_export = {}

for obj in bpy.data.objects:
    if obj.type != 'MESH':
        continue
    
    mesh = obj.data
    vertices = [[v.co.x, v.co.y, v.co.z] for v in mesh.vertices]
    edges = [[e.vertices[0], e.vertices[1]] for e in mesh.edges]
    
    faces = []
    for face in mesh.polygons:
        faces.append({
            "vertices": list(face.vertices),
            "normal": [face.normal.x, face.normal.y, face.normal.z]
        })
    
    origin = [obj.location.x, obj.location.y, obj.location.z]
    
    blender_export[obj.name] = {
        "origin": origin,
        "vertices": vertices,
        "edges": edges,
        "faces": faces
    }

print("✓ PASS 1: Exported from Blender")

# ===== PASS 2: Validate and Fix Winding =====

pentacubes_ordered = ['A', 'B', 'E', 'E\'', 'F', 'G', 'G\'', 'H', 'H\'', 'I', 'J', 'J\'', 'K', 'L', 'M', 'N', 'P', 'Q', 'R', 'R\'', 'S', 'S\'', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z']

pentacubes_validated = {}
winding_issues = {}

for name in pentacubes_ordered:
    if name not in blender_export:
        print(f"⚠ {name}: NOT FOUND in Blender export")
        continue
    
    obj = blender_export[name]
    origin = obj['origin']
    vertices = obj['vertices']
    faces = obj['faces']
    
    # Adjust vertices relative to origin
    adjusted_verts = []
    for v in vertices:
        adjusted_verts.append([
            round_coord(v[0] - origin[0]),
            round_coord(v[1] - origin[1]),
            round_coord(v[2] - origin[2])
        ])
    
    # Shift to positive coordinates
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
    
    # Fix winding for all faces
    fixed_faces = []
    winding_flips = 0
    
    for face in faces:
        vertex_indices = face['vertices']
        blender_normal = face['normal']
        
        # Check current winding
        computed_normal = compute_face_normal(positive_verts, vertex_indices)
        dot_before = sum(computed_normal[i] * blender_normal[i] for i in range(3))
        
        # Fix if needed
        fixed_indices = ensure_ccw_winding(positive_verts, vertex_indices, blender_normal)
        
        # Verify fix
        computed_normal = compute_face_normal(positive_verts, fixed_indices)
        dot_after = sum(computed_normal[i] * blender_normal[i] for i in range(3))
        
        if dot_before < -0.1 and dot_after > -0.1:
            winding_flips += 1
        
        fixed_faces.append({
            "vertices": fixed_indices,
            "vert_count": len(fixed_indices),
            "normal": blender_normal
        })
    
    if winding_flips > 0:
        winding_issues[name] = winding_flips
    
    pentacubes_validated[name] = {
        'vertices': positive_verts,
        'faces': fixed_faces,
        'vert_count': len(positive_verts),
        'face_count': len(fixed_faces)
    }

print(f"✓ PASS 2: Validated {len(pentacubes_validated)} pentacubes")
if winding_issues:
    print(f"  Winding fixes applied:")
    for name, count in sorted(winding_issues.items()):
        print(f"    {name}: {count} faces flipped")

# ===== PASS 3: Generate Optimized C Code =====

with open(output_file, "w") as f:
    f.write("// Auto-generated pentacube data from Blender export\n")
    f.write("// DO NOT EDIT - regenerate from run_in_blender.py\n")
    f.write("#include \"pentacubes.h\"\n\n")
    
    # Generate vertex arrays
    for name in pentacubes_ordered:
        if name not in pentacubes_validated:
            continue
        enum_name = name.replace("'", "_p")
        verts = pentacubes_validated[name]['vertices']
        f.write(f"static const float vertices_{enum_name}[][3] = {{\n")
        for v in verts:
            f.write(f"    {{{v[0]:.1f}f, {v[1]:.1f}f, {v[2]:.1f}f}},\n")
        f.write("};\n\n")
    
    # Generate edge arrays
    for name in pentacubes_ordered:
        if name not in pentacubes_validated:
            continue
        enum_name = name.replace("'", "_p")
        vertices = pentacubes_validated[name]['vertices']
        faces = pentacubes_validated[name]['faces']
        
        edge_set = set()
        for face in faces:
            face_verts = face['vertices']
            for i in range(len(face_verts)):
                v1_idx = face_verts[i]
                v2_idx = face_verts[(i + 1) % len(face_verts)]
                
                v1 = vertices[v1_idx]
                v2 = vertices[v2_idx]
                
                if are_parallel_or_perpendicular(v1, v2):
                    edge = tuple(sorted([v1_idx, v2_idx]))
                    edge_set.add(edge)
        
        edges = []
        for v1, v2 in sorted(edge_set):
            edges.extend([v1, v2])
        
        f.write(f"static const int edges_{enum_name}[] = {{\n")
        for i in range(0, len(edges), 16):
            line = ', '.join(str(e) for e in edges[i:i+16])
            f.write(f"    {line},\n")
        f.write("};\n\n")
    
    # Generate face vertex arrays
    for name in pentacubes_ordered:
        if name not in pentacubes_validated:
            continue
        enum_name = name.replace("'", "_p")
        faces = pentacubes_validated[name]['faces']
        f.write(f"static const int face_vertices_{enum_name}[] = {{\n")
        for face in faces:
            verts = face['vertices']
            line = ', '.join(str(v) for v in verts)
            f.write(f"    {line},\n")
        f.write("};\n\n")
    
    # Generate face vertex count arrays
    for name in pentacubes_ordered:
        if name not in pentacubes_validated:
            continue
        enum_name = name.replace("'", "_p")
        faces = pentacubes_validated[name]['faces']
        f.write(f"static const int face_vertex_counts_{enum_name}[] = {{\n")
        for face in faces:
            f.write(f"    {face['vert_count']},\n")
        f.write("};\n\n")
    
    # Generate pentacube_data array
    f.write("const pentacube_data_t pentacube_data[PENTACUBE_COUNT] = {\n")
    
    for name in pentacubes_ordered:
        if name not in pentacubes_validated:
            continue
        enum_name = name.replace("'", "_p")
        data = pentacubes_validated[name]
        
        # Compute edge count
        edge_set = set()
        for face in data['faces']:
            face_verts = face['vertices']
            for i in range(len(face_verts)):
                v1_idx = face_verts[i]
                v2_idx = face_verts[(i + 1) % len(face_verts)]
                
                v1 = data['vertices'][v1_idx]
                v2 = data['vertices'][v2_idx]
                
                if are_parallel_or_perpendicular(v1, v2):
                    edge = tuple(sorted([v1_idx, v2_idx]))
                    edge_set.add(edge)
        
        edge_count = len(edge_set)
        
        f.write(f"    {{\n")
        f.write(f"        .vertices = (const float *)vertices_{enum_name},\n")
        f.write(f"        .vertex_count = {data['vert_count']},\n")
        f.write(f"        .edges = edges_{enum_name},\n")
        f.write(f"        .edge_count = {edge_count},\n")
        f.write(f"        .face_vertices = face_vertices_{enum_name},\n")
        f.write(f"        .face_vertex_counts = face_vertex_counts_{enum_name},\n")
        f.write(f"        .face_count = {data['face_count']},\n")
        f.write(f"        .name = \"{name}\"\n")
        f.write(f"    }},\n")
    
    f.write("};\n")

print(f"✓ PASS 3: Generated {output_file}")

# ===== Statistics =====

print("\n=== Pentacube Statistics ===")
total_stats = {"verts": 0, "edges": 0, "faces": 0, "max_face": 0}

for name in pentacubes_ordered:
    if name not in pentacubes_validated:
        continue
    
    data = pentacubes_validated[name]
    vertices = data['vertices']
    faces = data['faces']
    
    edge_set = set()
    max_face_verts = 0
    
    for face in faces:
        max_face_verts = max(max_face_verts, face['vert_count'])
        face_verts = face['vertices']
        for i in range(len(face_verts)):
            v1_idx = face_verts[i]
            v2_idx = face_verts[(i + 1) % len(face_verts)]
            
            v1 = vertices[v1_idx]
            v2 = vertices[v2_idx]
            
            if are_parallel_or_perpendicular(v1, v2):
                edge = tuple(sorted([v1_idx, v2_idx]))
                edge_set.add(edge)
    
    edge_count = len(edge_set)
    face_count = len(faces)
    
    total_stats["verts"] += data['vert_count']
    total_stats["edges"] += edge_count
    total_stats["faces"] += face_count
    total_stats["max_face"] = max(total_stats["max_face"], max_face_verts)
    
    print(f"  {name:8s}: V={data['vert_count']:2d}, E={edge_count:2d}, F={face_count:2d}, MaxVpF={max_face_verts:2d}")

print(f"\n  TOTAL: {total_stats['verts']:3d} verts, {total_stats['edges']:3d} edges, {total_stats['faces']:3d} faces")
print(f"  Max vertices per face: {total_stats['max_face']}")
print(f"\n✓ All done!")