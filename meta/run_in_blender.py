import json
import bpy

output = {}

for obj in bpy.data.objects:
    if obj.type != 'MESH':
        continue
    
    mesh = obj.data
    
    # Vertices
    vertices = [[v.co.x, v.co.y, v.co.z] for v in mesh.vertices]
    
    # Edges (line segments)
    edges = [[e.vertices[0], e.vertices[1]] for e in mesh.edges]
    
    # Faces (loops of edges defining planes)
    faces = []
    for face in mesh.polygons:
        loop_indices = list(face.loop_indices)
        vertex_indices = [mesh.loops[i].vertex_index for i in loop_indices]
        # Determine winding (for cullability)
        is_clockwise = face.use_smooth  # or detect via normal
        faces.append({
            "vertices": vertex_indices,
            "normal": [face.normal.x, face.normal.y, face.normal.z]
        })
    
    # Object origin (transform)
    origin = [obj.location.x, obj.location.y, obj.location.z]
    
    output[obj.name] = {
        "origin": origin,
        "vertices": vertices,
        "edges": edges,
        "faces": faces
    }

with open("/tmp/pentacubes.json", "w") as f:
    json.dump(output, f, indent=2)

print("Exported to /tmp/pentacubes.json")