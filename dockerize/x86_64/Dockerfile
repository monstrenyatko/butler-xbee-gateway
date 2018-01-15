FROM debian:jessie

MAINTAINER Oleg Kovalenko <monstrenyatko@gmail.com>

ENV APP_DEB_PKG=butler-xbee-gateway-*-linux-amd64.deb

COPY tmp/$APP_DEB_PKG /tmp/

RUN apt-get update \
 && DEBIAN_FRONTEND=noninteractive apt-get install -y apt-utils \
 && DEBIAN_FRONTEND=noninteractive dpkg-reconfigure apt-utils \
 && DEBIAN_FRONTEND=noninteractive apt-get upgrade -y \
 && dpkg --install --force-depends /tmp/$APP_DEB_PKG \
 && DEBIAN_FRONTEND=noninteractive apt-get install -y -f \
 && DEBIAN_FRONTEND=noninteractive apt-get clean \
 && rm -rf /var/lib/apt/lists/* \
 && rm -rf /tmp/*

COPY config.json /etc/butler-xbee-gateway/config.json

CMD ["butler-xbee-gateway", "-c", "/etc/butler-xbee-gateway/config.json"]
