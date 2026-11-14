// Generated at publish time: CACHE and ASSETS are replaced by content hashes.
const CACHE = "__BRDIFF_CACHE__";
const ASSETS = "__BRDIFF_ASSETS__";
const PREFIX = "brdiff-browser-";

self.addEventListener("install", (event) => {
  event.waitUntil(caches.open(CACHE).then((cache) => cache.addAll(ASSETS)));
  self.skipWaiting();
});

self.addEventListener("activate", (event) => {
  event.waitUntil((async () => {
    const names = await caches.keys();
    // The previous POC's worker cached navigation HTML first. Refresh any
    // clients from that older worker once so they enter the new update model.
    const upgradingFromLegacy = names.some((name) => name.startsWith("brdiff-browser-poc-"));
    await Promise.all(names
      .filter((name) => name.startsWith(PREFIX) && name !== CACHE)
      .map((name) => caches.delete(name)));
    await self.clients.claim();
    if (upgradingFromLegacy) {
      const tabs = await self.clients.matchAll({ type: "window" });
      await Promise.all(tabs.map((tab) => tab.navigate(tab.url).catch(() => {})));
    }
  })());
});

self.addEventListener("fetch", (event) => {
  if (event.request.method !== "GET") return;
  const url = new URL(event.request.url);
  const scope = new URL(self.registration.scope);
  if (url.origin !== scope.origin || !url.pathname.startsWith(scope.pathname)) return;

  const isDocument = event.request.mode === "navigate" ||
    url.pathname === scope.pathname ||
    url.pathname === new URL("./index.html", scope).pathname;

  if (isDocument) {
    // On each online visit, fetch current HTML (and its versioned asset URLs).
    // Offline fallback stays consistent with the assets precached at install.
    // Do not put newer HTML in an older worker's cache before its assets exist.
    event.respondWith((async () => {
      try {
        return await fetch(event.request, { cache: "no-cache" });
      } catch (_) {
        return (await caches.match(event.request)) ||
          (await caches.match("./index.html")) || Response.error();
      }
    })());
  } else {
    // Hashed URLs are immutable: only fetch the network on cache misses.
    event.respondWith(caches.match(event.request).then((hit) => hit || fetch(event.request)));
  }
});
