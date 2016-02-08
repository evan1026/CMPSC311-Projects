# prints the usage for this file
printUsage() {
  echo "Usage: $0 <input file> <output count file> <output runtime file>"
}

# prints the usage and then exits
die() {
  printUsage
  exit 1
}

# checks to make sure that an argument has been supplied, dies otherwise
checkArg() {
  if [ -z "$1" ]; then
    die
  fi
}

# the start time in nano seconds
start=$(date +%s%N)

# renaming the arguments for convenience
inputfile="$1"
outputcountfile="$2"
outputruntimefile="$3"

# checking to ensure all arguments were supplied
checkArg "$inputfile"
checkArg "$outputcountfile"
checkArg "$outputruntimefile"

# checking to make sure there's no extraneous arguments
if [ -n "$4" ]; then
  die
fi

cat "$inputfile" | tr -cd '[:alnum:][:blank:]\n' | awk -f "./awkprog.awk" | sort > "$outputcountfile"

# lastly, let's get the runtime information, and convert it to milliseconds to make it more relatable
end=$(date +%s%N)
diff=$((end - start))

# and then write it to the file
echo The number of nanoseconds elapsed is $diff > $outputruntimefile
