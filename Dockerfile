# Build the actual Rust code into the WebAssembly module consumed by web/app.js.
FROM rust:1-bookworm AS build
WORKDIR /app
RUN rustup target add wasm32-unknown-unknown && cargo install wasm-pack --locked
COPY . .
RUN wasm-pack build --release --target web --out-dir web/pkg

# Serve the built site without a Rust runtime, Node, or backend API.
FROM caddy:2-alpine
COPY --from=build /app/web/ /usr/share/caddy/
COPY web/Caddyfile /etc/caddy/Caddyfile
EXPOSE 8080
