import init, { subtract_png } from "./pkg/brdiff.js";

const form = document.querySelector("#converter");
const baseInput = document.querySelector("#base");
const subtractionInput = document.querySelector("#subtraction");
const invertInput = document.querySelector("#invert");
const convertButton = document.querySelector("#convert");
const status = document.querySelector("#status");
const result = document.querySelector("#result");
const preview = document.querySelector("#preview");
const download = document.querySelector("#download");
const offlineStatus = document.querySelector("#offline-status");

// Large decoded raster images require significantly more memory than file size.
// Keep the first version conservative; this is not a streaming converter.
const MAX_FILE_BYTES = 128 * 1024 * 1024;
let outputUrl = null;

const ready = init();
ready.then(() => {
  convertButton.disabled = false;
  convertButton.textContent = "Create difference image";
  status.textContent = "Ready. Select two images to begin.";
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

  try {
    await ready;
    if (baseFile.size > MAX_FILE_BYTES || subtractionFile.size > MAX_FILE_BYTES) {
      throw new Error("Each source image must be at most 128 MiB for this proof of concept.");
    }

    status.textContent = "Reading and processing images locally…";
    const [base, subtraction] = await Promise.all([
      baseFile.arrayBuffer(),
      subtractionFile.arrayBuffer(),
    ]);
    const png = subtract_png(new Uint8Array(base), new Uint8Array(subtraction), invertInput.checked);
    const blob = new Blob([png], { type: "image/png" });

    if (outputUrl) URL.revokeObjectURL(outputUrl);
    outputUrl = URL.createObjectURL(blob);
    preview.src = outputUrl;
    download.href = outputUrl;
    const filename = baseFile.name.replace(/\.[^.]+$/, "").replace(/[^a-z0-9_-]+/gi, "-").slice(0, 50) || "image";
    download.download = `brdiff-${filename}.png`;
    result.hidden = false;
    status.textContent = `Complete. PNG size: ${(blob.size / 1024).toFixed(1)} KiB.`;
  } catch (error) {
    status.textContent = `Conversion failed: ${error}`;
  } finally {
    convertButton.disabled = false;
  }
});

window.addEventListener("pagehide", () => {
  if (outputUrl) URL.revokeObjectURL(outputUrl);
});

// The service worker only caches these local static assets for subsequent visits.
// It never receives or stores the user's source images.
if ("serviceWorker" in navigator && window.isSecureContext) {
  navigator.serviceWorker.register("./sw.js")
    .then(() => navigator.serviceWorker.ready)
    .then(() => {
      offlineStatus.textContent = "Offline ready: after this first visit, the app can load without a network connection.";
    })
    .catch((error) => {
      offlineStatus.textContent = `Offline caching unavailable: ${error}`;
    });
}
