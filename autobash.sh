#!/bin/bash

for file in ./Benchmark/*
do
  OUTPUT="$(./v2Dinic.x < ${file})"
  echo "${file##*/}" "${OUTPUT}" >> v2Dinic.out
done
