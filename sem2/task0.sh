#!/bin/bash

function read_man_file() {
  local dir_name=$1
  local func_name=$2
  local file_name="$dir_name/$func_name.3"
  local file_name_gz="$file_name.gz"
  local content=""
  if [ -r "$file_name" ]
  then
    content=$(cat "$file_name")
  fi
  if [ -r "$file_name_gz" ]
  then
    content=$(gunzip -c "$file_name_gz")
  fi
  echo "$content"
}

function find_include() {
  local content=$1
  local match_pattern='^\.B #include <([a-zA-Z0-9._]+)>'
  local sed_command="/$match_pattern/!d; s/$match_pattern/\1/;"
  echo "$content" | sed -E "$sed_command"
}

while read -r entry
do
  IFS=:
  result="---"
  for man_dir in $MANPATH
  do
    content=$(read_man_file "$man_dir/man3" "$entry")
    if [ ! -z "$content" ]
    then
      result=$(find_include "$content")
      continue
    fi
  done
  echo "$result"
done

