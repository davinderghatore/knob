#!/bin/bash
# Uploads the release keys from ~/.knob-release-keys to the GitHub repo's secrets, which the
# release workflow needs. Run once (and again only if the keys change). Needs `gh auth login`.
# The keys go straight from the files to GitHub: nothing is printed.
set -euo pipefail
repo=simranxdhindsa/knob
keys=$HOME/.knob-release-keys
for file in signing-p12-base64.txt signing-p12-password.txt sparkle-private-key.txt; do
    [ -s "$keys/$file" ] || { echo "Missing $keys/$file" >&2; exit 1; }
done
gh secret set KNOB_SIGNING_P12_BASE64 --repo "$repo" < "$keys/signing-p12-base64.txt"
gh secret set KNOB_SIGNING_P12_PASSWORD --repo "$repo" < "$keys/signing-p12-password.txt"
gh secret set SPARKLE_PRIVATE_KEY --repo "$repo" < "$keys/sparkle-private-key.txt"
echo "Release secrets are set on github.com/$repo."
