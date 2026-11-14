# Browser proof of concept

A static browser UI for `brdiff`. File bytes enter the Rust `image` decoder,
the original luminance subtraction runs inside WebAssembly, and Rust encodes
the output as PNG. JavaScript handles only file selection, download, and UI.
The CLI continues to work unchanged.

## Build and run

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

The built `web/` directory can be published as a static site, including
on GitHub Pages. No server, Node.js, bundler, CDN, or runtime network request
is necessary once the page's assets are available.

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
- No deployment has been configured. The CI workflow builds a downloadable
  static-site artifact; hosting can be added separately.

The output is bit-for-bit consistent with the CLI's pixel arithmetic
because `Image::subtract` is shared. Browser byte decoding and PNG encoding
are new; native tests exercise the byte-oriented path.
