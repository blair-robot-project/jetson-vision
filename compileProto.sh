#!/usr/bin/env bash
protoc -I=proto --cpp_out=gen/cpp --python_out=gen/python proto/*.proto