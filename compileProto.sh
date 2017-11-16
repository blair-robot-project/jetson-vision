#!/usr/bin/env bash
protoc -I=proto --cpp_out=gen proto/*.proto