# Browser proof of concept

A static browser UI for `brdiff`. File bytes enter the Rust `image` decoder,
the original luminance subtraction runs inside WebAssembly, and Rust encodes
the output as PNG. JavaScript handles only file selection, download, and UI.
The CLI continues to work unchanged.

## Hosted demo on GitHub Pages

The intended address is **https://smudge.github.io/brdiff/**.

The `.github/workflows/browser-wasm.yml` workflow builds the Rust/WASM app
and publishes the generated `web/` directory using GitHub Pages Actions.
During review, publishing is restricted to the
`poc/offline-browser-wasm` branch: changes to `master` do not deploy the
POC. After merging the PR, change the deploy job's branch condition to
`refs/heads/master` to maintain the site from the default branch.

Pages must be enabled in this repository at
**Settings → Pages → Build and deployment → Source: GitHub Actions**.
Creating a workflow does not itself enable the Pages site; a repository
owner must do this once. After enabling it, rerun the workflow on the POC
branch from the Actions tab or push a new commit to that branch.

The GitHub Pages URL includes `/brdiff/`. All local asset references,
service-worker registration, and cache entries use relative paths so the
browser works from this project subpath. GitHub Pages uses HTTPS.

## Build and run locally

Install Rust, the `wasm32-unknown-unknown` target, and `wasm-pack`:

```sh
rustup target add wasm32-unknown-unknown
cargo install wasm-pack --locked
cargo test
wasm-pack build --target web --release --out-dir web/pkg
python3 -m http.server 8000 --directory web
```

Open http://localhost:8000. Don't open `index.html` using a `file://`
URL; browsers require HTTP(S) to load WASM modules correctly.

No runtime server, Node.js, bundler, CDN dependency, or upload endpoint is
needed for the generated site; GitHub Pages only serves static files.

## Offline and privacy

The service worker precaches the HTML, JavaScript, and generated WebAssembly
files after the first successful visit on HTTPS or localhost. Once installed,
it lets the site reopen offline. Browsers may evict the cache; the user must
first visit online again if that happens. To deploy a new version, bump the
cache version in `sw.js`.

All image bytes remain in the browser and are passed to WebAssembly. Source
files and results are **not** uploaded or persisted in browser storage.
The preview uses a temporary object URL, and the PNG is downloaded locally.

## Scope / limitations

- Two same-sized images in, one PNG out. Invert behaves like `--invert`.
- The Rust `image` crate provides PNG, JPEG, TIFF, BMP and PPM decoding,
  including TIFF even where browser-native image decoding is unavailable.
- For this POC, each source file is capped at 128 MiB. Decoded rasters can
  consume significantly more memory; large images may still exhaust browser
  memory. Processing is on the main thread and can temporarily block the UI.
- As with the existing CLI, inputs are converted to 8-bit RGB before
  subtraction. Higher-bit-depth imaging and metadata are **not** preserved.
- Only PNG output is exposed. This is an intentionally narrow first step,
  not a general-purpose image format converter.
- JPEG decoding uses the single-threaded decoder in browser WASM; native CLI
  decoding retains its existing multithreaded configuration. The CI workflow
  tests a JPEG through both the native CLI and the built WASM entry point.

The output is bit-for-bit consistent with the CLI's pixel arithmetic
because `Image::subtract` is shared. Browser byte decoding and PNG encoding
are new; native tests exercise the byte-oriented path.
