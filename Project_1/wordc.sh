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
inputfile=$1
outputcountfile=$2
outputruntimefile=$3

# checking to ensure all arguments were supplied
checkArg $inputfile
checkArg $outputcountfile
checkArg $outputruntimefile

# checking to make sure there's no extraneous arguments
if [ -n "$4" ]; then
  die
fi

# this creates an associative array that will store the occurances of each word
declare -A words

# reading the file
while read -r line || [[ -n "$line" ]]; do

  # reading the words in a line
  for word in $line; do

    # this "clean word" is the word stripped of any symbols and converted to lowercase
    cleanword=$(echo $word | tr -cd '[[:alnum:]]' | tr '[:upper:]' '[:lower:]')

    # if we've seen the word before, increase the count. otherwise, declare it as counted once
    if [ -z "${words[$cleanword]}" ]; then
      words[$cleanword]=1
    else
      words[$cleanword]=$((${words[$cleanword]} + 1))
    fi

  done

# "done" ends the loop and the rest prints the file to stdin so it can be read by "read" back where the while was declared
done < "$inputfile"

# now we create the output, sort it, and write it to the output file
for word in "${!words[@]}"; do
  echo $word, ${words[$word]}
done | sort > $outputcountfile

# lastly, let's get the runtime information, and convert it to milliseconds to make it more relatable
end=$(date +%s%N)
diff=$(echo "scale=2; ($end - $start) / 1000000" | bc -l)

# and then write it to the file
echo $diff ms > $outputruntimefile
