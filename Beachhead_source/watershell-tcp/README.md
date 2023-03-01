## watershell-tcp
Port of [watershell](https://github.com/wumb0/watershell)

## Prerequisites

Ensure that docker is installed on your host to compile.

```shell
sudo apt install docker docker.io
```

## Compile

```shell
sudo ./docker-cmd.sh
```

## Known Problems
- In older versions of linux `g++` might not come with a regex library. As part of figuring out Layer 2 we use regex so that way the code isnt even more painful to read/write. 
- You cannot connect to a watershell from the box it is running on. The watershell client MUST be run on another box in order for it to work.
