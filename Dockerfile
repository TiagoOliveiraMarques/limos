FROM golang:1.22.5 as builder

WORKDIR /app
COPY go.* .
RUN go mod download

COPY . .
RUN go build -o /app/limos .

FROM scratch
WORKDIR /app
COPY --from=builder /app/limos /app/limos
ENTRYPOINT ["/app/limos"]