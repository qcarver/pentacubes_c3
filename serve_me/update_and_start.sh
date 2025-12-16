#!/bin/bash
# filepath: /home/qcarver/Dev/ota/pentacubes_c3/serve_me/update_and_start.sh

set -e

# Extract PROJECT_VER from CMakeLists.txt
PROJECT_VER=$(grep 'set(PROJECT_VER' ../CMakeLists.txt | sed 's/.*"\(.*\)".*/\1/')

echo "Updating version.json with version: $PROJECT_VER"

# Generate version.json
cat > version.json <<EOF
{
  "version": "$PROJECT_VER",
  "image_url": "http://192.168.1.98:8070/pentacubes_c3.bin"
}
EOF

echo "✓ Now serving pentacubes_c3 version $PROJECT_VER on port 8070"
python3 -m http.server 8070