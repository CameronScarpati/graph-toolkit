#!/bin/bash
find . -regex '.*\.\(h\|cpp\)' -exec clang-format-14 -i -style=file {} \; \
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
