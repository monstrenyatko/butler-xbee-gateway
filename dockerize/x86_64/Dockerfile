FROM debian:jessie

MAINTAINER Oleg Kovalenko <monstrenyatko@gmail.com>

RUN apt-get update \
    && DEBIAN_FRONTEND=noninteractive apt-get install -y apt-utils \
    && DEBIAN_FRONTEND=noninteractive dpkg-reconfigure apt-utils \
    && DEBIAN_FRONTEND=noninteractive apt-get upgrade -y \
    && DEBIAN_FRONTEND=noninteractive apt-get install -y \
        wget \
    && DEBIAN_FRONTEND=noninteractive apt-get clean \
    && rm -rf /var/lib/apt/lists/* \
    && rm -rf /tmp/*

ENV GOSU_VERSION 1.10
RUN set -x \
    && dpkgArch="$(dpkg --print-architecture | awk -F- '{ print $NF }')" \
    && wget -O /usr/local/bin/gosu "https://github.com/tianon/gosu/releases/download/$GOSU_VERSION/gosu-$dpkgArch" \
    && wget -O /usr/local/bin/gosu.asc "https://github.com/tianon/gosu/releases/download/$GOSU_VERSION/gosu-$dpkgArch.asc" \
    && export GNUPGHOME="$(mktemp -d)" \
    && gpg --keyserver ha.pool.sks-keyservers.net --recv-keys B42F6819007F00F88E364FD4036A9C25BF357DD4 \
    && gpg --batch --verify /usr/local/bin/gosu.asc /usr/local/bin/gosu \
    && rm -r "$GNUPGHOME" /usr/local/bin/gosu.asc \
    && chmod +x /usr/local/bin/gosu \
    && gosu nobody true

ENV APP_DEB_PKG=butler-xbee-gateway-*-linux-amd64.deb
COPY tmp/$APP_DEB_PKG /tmp/
RUN apt-get update \
    && dpkg --install --force-depends /tmp/$APP_DEB_PKG \
    && DEBIAN_FRONTEND=noninteractive apt-get install -y -f \
    && DEBIAN_FRONTEND=noninteractive apt-get clean \
    && rm -rf /var/lib/apt/lists/* \
    && rm -rf /tmp/*

ENV APP_USERNAME="butler-xbee-gateway"
RUN addgroup --system $APP_USERNAME \
    && adduser --system --no-create-home --ingroup $APP_USERNAME $APP_USERNAME \
    && usermod -a -G dialout $APP_USERNAME

COPY run.sh /
RUN chmod +x /run.sh

COPY app-entrypoint.sh /
RUN chmod +x /app-entrypoint.sh

COPY config.json /etc/butler-xbee-gateway/config.json

ENTRYPOINT ["/run.sh"]
CMD ["butler-xbee-gateway-app", "-c", "/etc/butler-xbee-gateway/config.json"]
