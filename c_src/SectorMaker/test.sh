#!/bin/bash

declare -A longoptspec
longoptspec=( [loglevel]=1 [junk]=1 [stuff]=0 ) #use associative array to declare how many arguments a long option expects, in this case we declare that loglevel expects/has one argument, long options that aren't listed in this way will have zero arguments by default
optspec=":h-:l:j:s"
while getopts "$optspec" opt; do
while true; do
    case "${opt}" in
        -) #OPTARG is name-of-long-option or name-of-long-option=value
            if [[ "${OPTARG}" =~ .*=.* ]] #with this --key=value format only one argument is possible
            then
                opt=${OPTARG/=*/}
                OPTARG=${OPTARG#*=}
                ((OPTIND--))
            else #with this --key value1 value2 format multiple arguments are possible
                opt="$OPTARG"
                OPTARG=(${@:OPTIND:$((longoptspec[$opt]))})
            fi
            ((OPTIND+=longoptspec[$opt]))
            continue #now that opt/OPTARG are set we can process them as if getopts would've given us long options
            ;;
        l|loglevel)
          loglevel=$OPTARG
          echo "Parsing option: '--${opt}', value: '${loglevel}'" >&2
            ;;
        j|junk)
          junk=$OPTARG
          echo "Parsing option: '--${opt}', value: '${junk}'" >&2
            ;;
        s|stuff)
          echo "Parsing option: '--${opt}'" >&2
            ;;
        h|help)
            echo "usage: $0 [--loglevel[=]<value>]" >&2
            exit 2
            ;;
    esac
break; done
done

# print out the remaining arguments
shift $((OPTIND-1))
for i in $@
do
    echo $i
done

# End of file

