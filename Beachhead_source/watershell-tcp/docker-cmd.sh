#!/bin/bash
docker run -v $(pwd):/working gcc:9.3.0 bash -c 'cd /working; bash build.sh watershell_static_tcp_port_1337'
