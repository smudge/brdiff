import test from "node:test";
import assert from "node:assert/strict";
import { readFileSync } from "node:fs";
import { initSync, subtract_png } from "../web/pkg/brdiff.js";

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
