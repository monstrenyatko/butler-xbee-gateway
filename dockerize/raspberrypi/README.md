HOW TO DOCKER
=============

About
=====

Run using [Docker](https://www.docker.com).

Build the image
===============

* Execute build script:
```sh
	./build.sh monstrenyatko/rpi-butler-xbee-gateway
```

Run the container
=================

* Start prebuilt image:

  ```sh
    docker-compose up -d
  ```
* Stop/Restart:

  ```sh
    docker-compose stop
    docker-compose start
  ```
* [**OPTIONAL**] Override the main configuration file:

  ```yaml
    butler-xbee-gateway:
      ...
      volumes:
        - ./config.json:/etc/butler-xbee-gateway/config.json:ro
  ```
