#!/bin/bash

set -e

cmd=$(ls build/bin | fzf)
"build/bin/"$cmd $@
