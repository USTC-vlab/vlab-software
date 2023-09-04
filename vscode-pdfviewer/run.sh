#!/bin/sh

cd "$(dirname "$0")"
docker build -t vlab-vscode-pdfviewer .
docker run --rm --name=vscode-pdfviewer -v "$PWD/output":/output vlab-vscode-pdfviewer
