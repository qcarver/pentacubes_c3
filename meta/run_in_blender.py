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
            "vertices": list(face.vertices),  # Already CCW per Blender normal
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

# ===== PASS 2: Transform Coordinates (No Winding Fixes) =====

pentacubes_ordered = ['A', 'B', 'E', 'E\'', 'F', 'G', 'G\'', 'H', 'H\'', 'I', 'J', 'J\'', 'K', 'L', 'M', 'N', 'P', 'Q', 'R', 'R\'', 'S', 'S\'', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z']

pentacubes_validated = {}

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
    
    # Keep faces as-is (trust Blender winding)
    fixed_faces = []
    for face in faces:
        fixed_faces.append({
            "vertices": face['vertices'],  # No reversal
            "vert_count": len(face['vertices']),
            "normal": face['normal']
        })
    
    pentacubes_validated[name] = {
        'vertices': positive_verts,
        'faces': fixed_faces,
        'vert_count': len(positive_verts),
        'face_count': len(fixed_faces)
    }

print(f"✓ PASS 2: Transformed {len(pentacubes_validated)} pentacubes (trusted Blender winding)")

# ===== PASS 3: Generate Optimized C Code =====

with open(output_file, 'w') as f:
    f.write('#include "pentacubes.h"\n\n')
    
    for name in pentacubes_ordered:
        if name not in pentacubes_validated:
            continue
        
        pentacube = pentacubes_validated[name]
        safe_name = name.replace("'", "_prime")
        
        # Vertices
        f.write(f"static const float pentacube_{safe_name}_verts[][3] = {{\n")
        for v in pentacube['vertices']:
            f.write(f"    {{{v[0]}, {v[1]}, {v[2]}}},\n")
        f.write("};\n\n")
        
        # Face normals
        f.write(f"static const float pentacube_{safe_name}_normals[][3] = {{\n")
        for face in pentacube['faces']:
            n = face['normal']
            f.write(f"    {{{n[0]:.6f}f, {n[1]:.6f}f, {n[2]:.6f}f}},\n")
        f.write("};\n\n")
        
        # Face vertices (flattened)
        f.write(f"static const int pentacube_{safe_name}_face_verts[] = {{")
        vert_strs = []
        for face in pentacube['faces']:
            vert_strs.extend([str(v) for v in face['vertices']])
        f.write(", ".join(vert_strs))
        f.write("};\n\n")
        
        # Face vertex counts
        f.write(f"static const int pentacube_{safe_name}_face_counts[] = {{")
        count_strs = [str(face['vert_count']) for face in pentacube['faces']]
        f.write(", ".join(count_strs))
        f.write("};\n\n")
    
    # Main data array
    f.write(f"const pentacube_data_t pentacube_data[PENTACUBE_COUNT] = {{\n")
    for name in pentacubes_ordered:
        if name not in pentacubes_validated:
            continue
        pentacube = pentacubes_validated[name]
        safe_name = name.replace("'", "_prime")
        f.write(f"    {{\n")
        f.write(f"        .name = \"{name}\",\n")
        f.write(f"        .vertices = (const float *)pentacube_{safe_name}_verts,\n")
        f.write(f"        .face_normals = (const float *)pentacube_{safe_name}_normals,\n")
        f.write(f"        .face_vertices = pentacube_{safe_name}_face_verts,\n")
        f.write(f"        .face_vertex_counts = pentacube_{safe_name}_face_counts,\n")
        f.write(f"        .vertex_count = {pentacube['vert_count']},\n")
        f.write(f"        .face_count = {pentacube['face_count']}\n")
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