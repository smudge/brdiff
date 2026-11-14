import init, { subtract_png, tiff_from_png } from "./pkg/brdiff.js";

const form = document.querySelector("#converter");
const baseInput = document.querySelector("#base");
const subtractionInput = document.querySelector("#subtraction");
const invertInput = document.querySelector("#invert");
const convertButton = document.querySelector("#convert");
const status = document.querySelector("#status");
const result = document.querySelector("#result");
const preview = document.querySelector("#preview");
const download = document.querySelector("#download");
const downloadTiff = document.querySelector("#download-tiff");

// Large decoded raster images require significantly more memory than file size.
// Keep the first version conservative; this is not a streaming converter.
const MAX_FILE_BYTES = 128 * 1024 * 1024;
let outputUrl = null;
let tiffUrl = null;
let differencePng = null;

const ready = init();
ready.then(() => {
  convertButton.disabled = false;
  convertButton.textContent = "Convert";
  status.textContent = "";
}).catch((error) => {
  status.textContent = `Unable to load WebAssembly: ${error}`;
  convertButton.textContent = "WebAssembly unavailable";
});

form.addEventListener("submit", async (event) => {
  event.preventDefault();
  const baseFile = baseInput.files[0];
  const subtractionFile = subtractionInput.files[0];
  if (!baseFile || !subtractionFile) return;

  convertButton.disabled = true;
  result.hidden = true;
  differencePng = null;
  if (tiffUrl) URL.revokeObjectURL(tiffUrl);
  tiffUrl = null;
  downloadTiff.href = "#";

  try {
    await ready;
    if (baseFile.size > MAX_FILE_BYTES || subtractionFile.size > MAX_FILE_BYTES) {
      throw new Error("Each image must be 128 MiB or smaller.");
    }

    status.textContent = "Processing…";
    const [base, subtraction] = await Promise.all([
      baseFile.arrayBuffer(),
      subtractionFile.arrayBuffer(),
    ]);
    const png = subtract_png(new Uint8Array(base), new Uint8Array(subtraction), invertInput.checked);
    const blob = new Blob([png], { type: "image/png" });
    differencePng = png;

    if (outputUrl) URL.revokeObjectURL(outputUrl);
    outputUrl = URL.createObjectURL(blob);
    preview.src = outputUrl;
    download.href = outputUrl;
    const filename = baseFile.name.replace(/\.[^.]+$/, "").replace(/[^a-z0-9_-]+/gi, "-").slice(0, 50) || "image";
    download.download = `brdiff-${filename}.png`;
    downloadTiff.download = `brdiff-${filename}.tif`;
    result.hidden = false;
    status.textContent = `Done · ${(blob.size / 1024).toFixed(1)} KiB`;
  } catch (error) {
    status.textContent = `Conversion failed: ${error}`;
  } finally {
    convertButton.disabled = false;
  }
});

downloadTiff.addEventListener("click", (event) => {
  if (!differencePng) {
    event.preventDefault();
    return;
  }

  try {
    if (!tiffUrl) {
      const tiff = tiff_from_png(differencePng);
      tiffUrl = URL.createObjectURL(new Blob([tiff], { type: "image/tiff" }));
    }
    downloadTiff.href = tiffUrl;
  } catch (error) {
    event.preventDefault();
    status.textContent = `TIFF encoding failed: ${error}`;
  }
});

window.addEventListener("pagehide", () => {
  if (outputUrl) URL.revokeObjectURL(outputUrl);
  if (tiffUrl) URL.revokeObjectURL(tiffUrl);
});

// The service worker only caches these local static assets for subsequent visits.
// It never receives or stores the user's source images.
if ("serviceWorker" in navigator && window.isSecureContext) {
  navigator.serviceWorker.register("./sw.js", { updateViaCache: "none" })
    .catch((error) => console.warn("Service worker registration failed:", error));
}
