#!/bin/bash
read -p "Enter commit message: " msg
git add .
git commit -m "$msg" || :
git push
read -p "Enter commit message: " sub_msg
git submodule foreach "git add . && git commit -m \"$sub_msg\" || : && git push"