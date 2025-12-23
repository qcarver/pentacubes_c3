#!/bin/bash

set -e

# Extract VERSION from project() line in CMakeLists.txt
VERSION=$(grep '^project(pentacubes_c3 VERSION' ../CMakeLists.txt | sed 's/.*VERSION \([0-9.]*\).*/\1/')

echo "Updating version.json with version: $VERSION"

# Generate version.json
cat > version.json <<EOF
{
  "version": "$VERSION",
  "image_url": "http://192.168.1.98:8070/pentacubes_c3.bin"
}
