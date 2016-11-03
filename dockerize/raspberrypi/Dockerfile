FROM resin/rpi-raspbian:jessie

MAINTAINER Oleg Kovalenko <monstrenyatko@gmail.com>

ENV APP_DEB_PKG=butler-xbee-gateway-*-linux-armhf.deb

COPY install/$APP_DEB_PKG /tmp/

RUN dpkg --install /tmp/$APP_DEB_PKG \
    && apt-get update && apt-get install -y -f \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/* \
    && rm -rf /tmp/*

VOLUME ["/config"]

CMD ["butler-xbee-gateway", "-c", "/config/config.json"]
