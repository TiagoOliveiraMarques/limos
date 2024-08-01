FROM php:8.3-zts-bookworm as builder

# Install golang
COPY --from=golang:1.22.5 /usr/local/go /usr/local/go
ENV PATH="/usr/local/go/bin:$PATH"

WORKDIR /app
COPY go.* .
RUN go mod download

COPY . .
RUN go build -o /app/limos .

FROM builder as test
WORKDIR /app
ENTRYPOINT ["go", "test", "./..."]

FROM php:8.3-zts-bookworm
WORKDIR /app
COPY --from=builder /app/limos /app/limos
ENTRYPOINT ["/app/limos"]