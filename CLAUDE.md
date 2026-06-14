# Project guidelines

## Pull requests

- Keep PRs small: **no more than 300 lines of changed code** (additions + deletions).
- Auto-generated lines do **not** count toward the limit — e.g. dependency manifests/lockfiles
  (`package-lock.json`, `Cargo.lock`, vendored deps), generated code, and build artifacts.
- If a change would exceed 300 hand-written lines, split it into multiple smaller PRs.
