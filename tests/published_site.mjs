import test from 'node:test';
import assert from 'node:assert/strict';
import { readFileSync } from 'node:fs';
import { pathToFileURL } from 'node:url';
import { join, resolve } from 'node:path';

const dist = resolve('dist');
const manifest = JSON.parse(readFileSync(join(dist, 'asset-manifest.json'), 'utf8'));
const modulePath = join(dist, manifest.assets.find((asset) => /\.js$/.test(asset) && asset.includes('/pkg/')).replace(/^\.\//, ''));
const wasmPath = join(dist, manifest.assets.find((asset) => /\.wasm$/.test(asset)).replace(/^\.\//, ''));
const { initSync, subtract_png } = await import(pathToFileURL(modulePath).href);
initSync({ module: readFileSync(wasmPath) });

const fixture = new Uint8Array(Buffer.from(
  readFileSync('tests/fixtures/rgb8_jpeg.hex', 'utf8').trim(), 'hex'
));

test('the published fingerprinted WASM bundle decodes JPEG', () => {
  const png = subtract_png(fixture, fixture, false);
  assert.deepEqual(Array.from(png.subarray(0, 8)), [137, 80, 78, 71, 13, 10, 26, 10]);
});

test('the published shell imports the fingerprinted browser bundle', () => {
  const html = readFileSync(join(dist, 'index.html'), 'utf8');
  const app = manifest.assets.find((asset) => /\.js$/.test(asset) && !asset.includes('/pkg/'));
  assert.ok(html.includes('src="' + app + '"'));
  assert.match(readFileSync(join(dist, 'sw.js'), 'utf8'), new RegExp('brdiff-browser-' + manifest.revision));
});
