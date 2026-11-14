// Cache the complete static app so it can be opened again while offline.
// Bump this name when deploying updated JS or WebAssembly files.
const CACHE = "brdiff-browser-poc-v1";
const ASSETS = [
  "./",
  "./index.html",
  "./app.js",
  "./pkg/brdiff.js",
  "./pkg/brdiff_bg.wasm",
];

self.addEventListener("install", (event) => {
  event.waitUntil(caches.open(CACHE).then((cache) => cache.addAll(ASSETS)));
  self.skipWaiting();
});

self.addEventListener("activate", (event) => {
  event.waitUntil(
    caches.keys().then((names) =>
      Promise.all(
        names.filter((name) => name.startsWith("brdiff-browser-poc-") && name !== CACHE)
          .map((name) => caches.delete(name))
      )
    ).then(() => self.clients.claim())
  );
});

self.addEventListener("fetch", (event) => {
  if (event.request.method !== "GET") return;
  const url = new URL(event.request.url);
  if (url.origin !== self.location.origin ||
      !url.pathname.startsWith(new URL(self.registration.scope).pathname)) return;

  event.respondWith(
    caches.match(event.request).then((cached) => cached || fetch(event.request))
  );
});
