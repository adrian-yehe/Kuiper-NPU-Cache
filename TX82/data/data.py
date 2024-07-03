#!/usr/bin/env python
# coding=utf-8
# generate_incremental_bin.py
import struct

# Specify the size of the file in bytes
file_size = 1024 * 1024  # 1 MB

# Open a file in binary write mode
with open('incremental_data.bin', 'wb') as f:
    for i in range(file_size // 4):
        # Write the integer in little-endian format
        f.write(struct.pack('<I', i))

