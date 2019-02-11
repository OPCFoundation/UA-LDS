FROM microsoft/dotnet:latest

ADD / /lds

RUN \
        set -ex \
    && \
        apt-get update && apt-get install -y \
            build-essential \
            git cmake \
            libcurl4-openssl-dev libssl-dev \
            libavahi-compat-libdnssd-dev \
            dbus rsyslog avahi-daemon avahi-utils \
    && \
	    rm -rf /lds/build && mkdir /lds/build && cd /lds/build \
    &&  cmake .. && cmake --build . \
    && \
        cp /lds/docker-initd.sh /etc/init.d/lds \
    &&  echo "service rsyslog start" >> /etc/init.d/lds \
    &&  echo "service dbus start" >> /etc/init.d/lds \
    &&  echo "avahi-daemon --no-drop-root --daemonize --syslog" >> /etc/init.d/lds \
    &&  echo "./lds/build/bin/opcualds -c /lds/etc/ualds.conf " >> /etc/init.d/lds \
    &&  chmod +x /etc/init.d/lds
        
EXPOSE 5353

ENTRYPOINT service lds start && bash

