#!/usr/bin/python

import os

targetfh = open("targets.txt")

targets = targetfh.readlines()

for target in targets:
    os.system(target)
