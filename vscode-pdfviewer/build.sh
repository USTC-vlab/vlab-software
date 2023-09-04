#!/bin/sh -e

WORKDIR="$PWD/vscode-pdfviewer"
REPO=https://github.com/tomoki1207/vscode-pdfviewer
COMMIT=d5f1ea28d1826dad60a3f4b6f025109caea9b4c2

git clone --filter=blob:none --single-branch "$REPO" "$WORKDIR"

cd "$WORKDIR"
git checkout "$COMMIT"
sed -i 's/"ui"/"ui", "workspace"/' package.json
npm install
npm run package
mv *.vsix /output/
