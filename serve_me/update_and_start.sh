#!/bin/bash

set -e

# Extract APP_VERSION from set(APP_VERSION "...") in CMakeLists.txt
APP_VERSION=$(grep 'set(APP_VERSION' ../CMakeLists.txt | sed 's/.*"\(.*\)".*/\1/')

# Extract CONFIG_YAOTAU_SERVER_VERSION_URL from Kconfig
# Looks for: default "http://..."
SERVER_URL=$(grep -A1 'config YAOTAU_SERVER_VERSION_URL' ../components/yaotau/Kconfig | grep 'default' | sed 's/.*"\(.*\)".*/\1/')

# Extract just the host:port from the full URL (remove /version.json)
BASE_URL=$(echo "$SERVER_URL" | sed 's|/version.json||')

echo "Updating version.json with version: $APP_VERSION"
echo "Server URL: $SERVER_URL"

# Generate version.json
cat > version.json <<EOF
{
  "version": "$APP_VERSION",
  "image_url": "$BASE_URL/pentacubes_c3.bin"
}
EOF

echo "✓ Now serving pentacubes_c3 version $APP_VERSION on port 8070"
python3 -m http.server 8070
