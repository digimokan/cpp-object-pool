#!/bin/sh

cd ..
./third_party/smart-build/src/smart-build.sh \
  --clean-all \
  --build-type-debug \
  --make-and-run-tests

