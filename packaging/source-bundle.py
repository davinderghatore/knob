#!/usr/bin/env python3
"""Put Knob's complete source code next to the installer (AGPL-3.0 requirement).

Usage: source-bundle.py OUTPUT_DIRECTORY
Writes knob-source.tar.gz and SOURCE_SHA256SUMS. Uses the files git tracks, so
build output and local settings are never included.
"""
import gzip
import hashlib
import subprocess
import sys
import tarfile
from io import BytesIO
from pathlib import Path

root = Path(__file__).resolve().parents[1]
output = Path(sys.argv[1]).resolve()
output.mkdir(parents=True, exist_ok=True)
tracked = subprocess.check_output(['git', '-C', str(root), 'ls-files', '-z'], text=True).split('\0')
files = sorted(name for name in tracked if name and (root / name).is_file())
if 'LICENSE' not in files:
    raise SystemExit('LICENSE must be tracked before building an installer.')

archive = output / 'knob-source.tar.gz'
manifest = []
with archive.open('wb') as raw, gzip.GzipFile(filename='', mode='wb', fileobj=raw, mtime=0) as zipped, \
        tarfile.open(fileobj=zipped, mode='w') as tar:
    for name in files:
        path = root / name
        data = path.read_bytes()
        manifest.append(f'{hashlib.sha256(data).hexdigest()}  {name}')
        entry = tarfile.TarInfo(f'knob/{name}')
        entry.size, entry.uid, entry.gid, entry.mtime = len(data), 0, 0, 0
        entry.mode = 0o755 if path.stat().st_mode & 0o111 else 0o644
        tar.addfile(entry, BytesIO(data))
(output / 'SOURCE_SHA256SUMS').write_text('\n'.join(manifest) + '\n')
print(f'Source archive: {archive}; files: {len(files)}')
