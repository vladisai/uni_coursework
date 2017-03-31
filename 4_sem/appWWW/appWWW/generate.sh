#! /usr/bin/env bash
/bin/rm -r ./gen
mkdir gen
cp -r ./elections/static/*.css ./gen/
var="@import url(\"./base.css\");"
sed -i "1s:.*:${var}:" ./gen/style*.css
time $1 ./download.py
