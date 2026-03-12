#!/bin/bash
CLANG_FORMAT_VERSION="${CLANG_FORMAT_VERSION:-17}"
CLANG_FORMAT="clang-format-${CLANG_FORMAT_VERSION}"

find . -regex '.*\.\(h\|cpp\)' -exec ${CLANG_FORMAT} -i -style=file {} \; \
&& git diff --quiet \
|| {
  for i in {{1..80}}; do echo -n = ; done
  echo
  echo "The following files don't meet the style guidelines:"
  git diff --name-only | sed "s/^/ - /"
  echo
  for i in {{1..80}}; do echo -n = ; done
  echo
  echo "Detailed style violations are as below:"
  git diff --color=always -U0 | sed "/^\x1b\[32m+/d;s/^\x1b\[36m/\n\0/;s/^\x1b\[1mdiff/\n\0/"
  echo
  exit 1
}
