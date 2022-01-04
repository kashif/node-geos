FROM node:6

WORKDIR /usr/src/node-geos

# apt-get install
RUN set -ex \
    && apt-get update \
    && apt-get install -y --no-install-recommends \
      curl \
      build-essential \
      cmake \
      g++ \
      git

# geos
ARG GEOS_GIT_TAG=3.5.0

RUN set -ex \
    && cd /usr/src \
    && git clone https://github.com/libgeos/geos.git \
    && cd geos \
    && git checkout ${GEOS_GIT_TAG} \
    && mkdir cmake-build \
    && cd cmake-build \
    && cmake -DCMAKE_BUILD_TYPE=Release .. \
    && make -j$(nproc) \
    && make install \
    && ldconfig

COPY . .

RUN npm set unsafe-perm true \
    && npm install

CMD ["npm", "test"]
