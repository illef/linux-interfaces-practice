#!/bin/bash

set -e

./build.sh
cmd=$(ls build/bin | fzf)
"build/bin/"$cmd $@
