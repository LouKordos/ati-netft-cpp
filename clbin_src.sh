find src/ -type f -exec printf "\n\n---------------------------File: {}--------------------------\n\n" ";" -exec cat {} ";" | curl -F 'clbin=<-' https://clbin.com
