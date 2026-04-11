#!/bin/bash

read -p "Enter commit message: " msg
git add .
git commit -m "$msg" || :
git push

# Push changes in all submodules
git submodule foreach "read -p 'Enter commit message for \"$name\": ' sub_msg && git add . && git commit -m \"\$sub_msg\" || : && git push"