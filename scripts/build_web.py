#!/usr/bin/env python3
"""Prepare a self-contained, content-addressed GitHub Pages bundle.

Run *after* wasm-pack builds web/pkg. This intentionally uses only the Python
standard library: no JS bundler or external build-time dependency is required.
"""

import hashlib
import json
import shutil
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


def checksum(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()[:16]


def replace_exact(text: str, old: str, new: str) -> str:
    if text.count(old) != 1:
        raise ValueError(f"Expected exactly one {old!r} reference; check wasm-pack output")
    return text.replace(old, new)


def build(source: Path, output: Path) -> dict:
    source = source.resolve()
    output = output.resolve()
    if source == output or source in output.parents or output in source.parents:
        raise ValueError("The build output must be outside the source directory")

    # The generated wasm-bindgen JS resolves its companion WASM file relative
    # to import.meta.url. Rename both and update that exact reference.
    wasm = (source / "pkg/brdiff_bg.wasm").read_bytes()
    wasm_name = f"brdiff_bg.{checksum(wasm)}.wasm"
    wasm_js = replace_exact(
        (source / "pkg/brdiff.js").read_text(encoding="utf-8"),
        "'brdiff_bg.wasm'",
        f"'{wasm_name}'",
    ).encode("utf-8")
    wasm_js_name = f"brdiff.{checksum(wasm_js)}.js"

    app = replace_exact(
        (source / "app.js").read_text(encoding="utf-8"),
        '"./pkg/brdiff.js"',
        f'"./pkg/{wasm_js_name}"',
    ).encode("utf-8")
    app_name = f"app.{checksum(app)}.js"

    index = replace_exact(
        (source / "index.html").read_text(encoding="utf-8"),
        'src="./app.js"',
        f'src="./{app_name}"',
    ).encode("utf-8")

    # Include the service worker source itself so even worker-only changes
    # trigger a new installation and refresh the offline cache.
    worker_template = (source / "sw.js").read_bytes()
    payloads = [index, app, wasm_js, wasm, worker_template]
    revision = checksum(b"".join(len(p).to_bytes(8, "big") + p for p in payloads))
    assets = ["./", "./index.html", f"./{app_name}",
              f"./pkg/{wasm_js_name}", f"./pkg/{wasm_name}"]
    worker = worker_template.decode("utf-8")
    worker = replace_exact(worker, '"__BRDIFF_CACHE__"', f'"brdiff-browser-{revision}"')
    worker = replace_exact(worker, '"__BRDIFF_ASSETS__"', json.dumps(assets))

    # Recreate output so obsolete fingerprinted files do not persist between
    # builds or get uploaded into the Pages artifact.
    if output.exists():
        shutil.rmtree(output)
    (output / "pkg").mkdir(parents=True)
    (output / "index.html").write_bytes(index)
    (output / app_name).write_bytes(app)
    (output / "pkg" / wasm_js_name).write_bytes(wasm_js)
    (output / "pkg" / wasm_name).write_bytes(wasm)
    (output / "pkg" / "package.json").write_bytes((source / "pkg/package.json").read_bytes())
    (output / "sw.js").write_text(worker, encoding="utf-8")

    manifest = {"revision": revision, "assets": assets}
    (output / "asset-manifest.json").write_text(
        json.dumps(manifest, indent=2) + "\n", encoding="utf-8"
    )
    return manifest


if __name__ == "__main__":
    import argparse

    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--source", type=Path, default=ROOT / "web")
    parser.add_argument("--output", type=Path, default=ROOT / "dist")
    args = parser.parse_args()
    manifest = build(args.source, args.output)
    print(f"Created {args.output}: {manifest['revision']}")
