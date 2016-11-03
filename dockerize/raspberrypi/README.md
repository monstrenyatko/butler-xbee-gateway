# HOW TO DOCKER

## About

Run using [Docker](https://www.docker.com).

## Build the container

* Place the `deb` package to install directory
* Execute build script:
```sh
	./build.sh
```

## Run

* Create `Configuration` storage:
```sh
	BUTLER_XBEE_GW_CFG="butler-xbee-gateway-config"
	docker volume create --name $BUTLER_XBEE_GW_CFG
```
* Copy `Configuration` to the storage:
```sh
	docker run -v $BUTLER_XBEE_GW_CFG:/mnt --rm -v $(pwd):/src hypriot/armhf-busybox \
		cp /src/config.json /mnt/config.json
```
* Edit `Configuration` (OPTIONAL):
```sh
	docker run -v $BUTLER_XBEE_GW_CFG:/mnt --rm -it hypriot/armhf-busybox \
		vi /mnt/config.json
```
* Start prebuilt image:
```sh
	docker-compose up -d --no-build
```
* Stop/Restart:
```sh
	docker stop butler-xbee-gateway
	docker start butler-xbee-gateway
```
