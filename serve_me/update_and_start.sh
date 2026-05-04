#!/bin/bash

set -e

# Extract APP_VERSION from set(APP_VERSION "...") in CMakeLists.txt
APP_VERSION=$(grep 'set(APP_VERSION' ../CMakeLists.txt | sed 's/.*"\(.*\)".*/\1/')

# Prefer actual project configuration from sdkconfig, fallback to Kconfig default.
if [ -f ../sdkconfig ]; then
  SERVER_URL=$(grep '^CONFIG_YAOTAU_SERVER_VERSION_URL=' ../sdkconfig | sed 's/^CONFIG_YAOTAU_SERVER_VERSION_URL="\(.*\)"$/\1/' || true)
fi

if [ -z "$SERVER_URL" ]; then
  SERVER_URL=$(grep -A1 'config YAOTAU_SERVER_VERSION_URL' ../components/yaotau/Kconfig | grep 'default' | sed 's/.*"\(.*\)".*/\1/' || true)
fi

if [ -z "$SERVER_URL" ]; then
  echo "ERROR: Could not determine YAOTAU server URL from sdkconfig or Kconfig."
  exit 1
fi

# Extract just the host:port from the full URL (remove /version.json)
BASE_URL=$(echo "$SERVER_URL" | sed 's|/version.json||')

if ! echo "$BASE_URL" | grep -Eq '^https?://[^/]+$'; then
  echo "ERROR: Derived BASE_URL is not absolute: '$BASE_URL'"
  echo "Expected something like: http://192.168.1.98:8070"
  exit 1
fi

echo "Updating version.json with version: $APP_VERSION"
echo "Base URL: $BASE_URL"

# Generate version.json with FULL URL (not relative path)
cat > version.json <<EOF
{
  "version": "$APP_VERSION",
  "image_url": "${BASE_URL}/pentacubes_c3.bin"
}
EOF

cat version.json
echo ""
echo "✓ Now serving pentacubes_c3 version $APP_VERSION on port 8070"
python3 -m http.server 8070
