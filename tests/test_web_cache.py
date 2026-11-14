"""Tests for reproducible, checksum-addressed static publishing."""

import json
import tempfile
import unittest
from pathlib import Path

from scripts.build_web import build, checksum


class BrowserCacheBuildTests(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.root = Path(self.tmp.name)
        self.src = self.root / "web"
        (self.src / "pkg").mkdir(parents=True)
        (self.src / "index.html").write_text('<script type="module" src="./app.js"></script>')
        (self.src / "app.js").write_text('import init from "./pkg/brdiff.js";')
        (self.src / "pkg/brdiff.js").write_text("new URL('brdiff_bg.wasm', import.meta.url)")
        (self.src / "pkg/brdiff_bg.wasm").write_bytes(b"wasm-1")
        (self.src / "pkg/package.json").write_text('{"type": "module"}')
        self.template = Path(__file__).resolve().parents[1] / "web/sw.js"
        (self.src / "sw.js").write_bytes(self.template.read_bytes())
        self.out = self.root / "dist"

    def test_references_and_checksums_match_files(self):
        manifest = build(self.src, self.out)
        index = (self.out / "index.html").read_text()
        app_path = next(self.out.glob("app.*.js"))
        glue_path = next((self.out / "pkg").glob("brdiff.*.js"))
        wasm_path = next((self.out / "pkg").glob("brdiff_bg.*.wasm"))
        self.assertIn(f'src="./{app_path.name}"', index)
        self.assertIn(f'"./pkg/{glue_path.name}"', app_path.read_text())
        self.assertIn(f"'{wasm_path.name}'", glue_path.read_text())
        for file in (app_path, glue_path, wasm_path):
            self.assertIn(checksum(file.read_bytes()), file.name)
        self.assertEqual(json.loads((self.out / "asset-manifest.json").read_text()), manifest)
        worker = (self.out / "sw.js").read_text()
        self.assertIn(f'"brdiff-browser-{manifest["revision"]}"', worker)
        for asset in manifest["assets"]:
            self.assertTrue((self.out / asset.removeprefix("./")).exists() if asset != "./" else True)
            self.assertIn(f'"{asset}"', worker)
        self.assertNotIn("__BRDIFF_", worker)
        self.assertNotIn('src="./app.js"', index)

    def test_same_inputs_are_reproducible(self):
        a = build(self.src, self.out)
        before = {p.relative_to(self.out): p.read_bytes() for p in self.out.rglob("*") if p.is_file()}
        b = build(self.src, self.out)
        after = {p.relative_to(self.out): p.read_bytes() for p in self.out.rglob("*") if p.is_file()}
        self.assertEqual(a, b)
        self.assertEqual(before, after)

    def test_wasm_change_invalidates_every_dependent_asset(self):
        a = build(self.src, self.out)
        self.assertEqual(len(a["assets"]), 5)
        (self.src / "pkg/brdiff_bg.wasm").write_bytes(b"wasm-2")
        b = build(self.src, self.out)
        self.assertNotEqual(a["revision"], b["revision"])
        self.assertNotEqual(a["assets"][2:], b["assets"][2:])
        self.assertFalse(any(p.name.endswith(f"{checksum(b'wasm-1')}.wasm") for p in self.out.rglob("*")))

    def test_html_and_worker_changes_invalidate_offline_cache(self):
        a = build(self.src, self.out)
        (self.src / "index.html").write_text('<script type="module" src="./app.js"></script>updated')
        b = build(self.src, self.out)
        self.assertNotEqual(a["revision"], b["revision"])
        (self.src / "sw.js").write_bytes(self.template.read_bytes() + b"\n// update\n")
        c = build(self.src, self.out)
        self.assertNotEqual(b["revision"], c["revision"])


if __name__ == "__main__":
    unittest.main()
