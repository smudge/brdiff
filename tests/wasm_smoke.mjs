import test from "node:test";
import assert from "node:assert/strict";
import { readFileSync } from "node:fs";
import { initSync, subtract_png, tiff_from_png } from "../web/pkg/brdiff.js";

initSync({ module: readFileSync(new URL("../web/pkg/brdiff_bg.wasm", import.meta.url)) });

const jpeg = new Uint8Array(Buffer.from(
  readFileSync(new URL("./fixtures/rgb8_jpeg.hex", import.meta.url), "utf8").trim(), "hex"
));

test("WASM decodes JPEG and emits a blue-red PNG", () => {
  const result = subtract_png(jpeg, jpeg, false);
  assert.equal(result[0], 0x89); // PNG signature
  assert.equal(result[1], 0x50);
  assert.equal(result[2], 0x4e);
  assert.equal(result[3], 0x47);
});

test("WASM emits a meaningful error for invalid image bytes", () => {
  assert.throws(
    () => subtract_png(new Uint8Array([1, 2, 3]), jpeg, false),
    /decode base image/i
  );
});


test("WASM encodes the PNG difference as TIFF", () => {
  const png = subtract_png(jpeg, jpeg, false);
  const tiff = tiff_from_png(png);
  const signature = Array.from(tiff.subarray(0, 4));
  assert.ok(
    JSON.stringify(signature) === JSON.stringify([0x49, 0x49, 0x2a, 0x00]) ||
    JSON.stringify(signature) === JSON.stringify([0x4d, 0x4d, 0x00, 0x2a]),
    "valid TIFF header"
  );
});

test("WASM returns an error for invalid TIFF source bytes", () => {
  assert.throws(
    () => tiff_from_png(new Uint8Array([1, 2, 3])),
    /decode difference image/i
  );
});
