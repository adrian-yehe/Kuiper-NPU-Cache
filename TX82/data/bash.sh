#!/bin/bash
# Step 1: Create the Python script
cat << EOF > generate_incremental_bin.py
import struct

# Specify the size of the file in bytes
file_size = 1024 * 1024  # 1 MB

# Open a file in binary write mode
with open('incremental_data.bin', 'wb') as f:
    for i in range(file_size // 4):
        # Write the integer in little-endian format
        f.write(struct.pack('<I', i))
EOF

# Step 2: Run the Python script
python3 generate_incremental_bin.py

# Step 3: Verify the generated file
ls -lh incremental_data.bin
hexdump -C incremental_data.bin | head

