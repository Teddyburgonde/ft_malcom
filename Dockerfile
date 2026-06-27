FROM alpine:3.20

RUN apk add --no-cache \
		build-base \
		linux-headers \
		iputils \
		iproute2 \
		tcpdump

WORKDIR /app
COPY . /app/
RUN make fclean && make
