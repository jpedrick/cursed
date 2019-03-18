#!/bin/bash
for i in *.{cpp,hpp}
do
  if ! grep -q Copyright $i
  then
    cat ./per_file_copyright.txt $i > $i.new && mv $i $i.old && mv $i.new $i
  fi
done
