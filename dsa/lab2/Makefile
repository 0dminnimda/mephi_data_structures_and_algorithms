# MY_CC="cc"
# ARGS="queue.c memo.c main.c -o main.out -O3 -g"

# if [[ $? != "0" ]]; then
#     echo "I did not find 'cc', so using 'clang' instead"
#     MY_CC="clang"
#     ARGS="$ARGS -Wno-deprecated-declarations"
# fi

# if [[ $1 == "vector" ]]; then
#     ARGS="$ARGS -D QUEUE_IMPL='V'"
# fi

# COMMAND="$MY_CC $ARGS"

# printf "Running \"$COMMAND\"\n\n"

# $COMMAND


DOT := $$(which cc &> /dev/null; echo $?)

all:
ifndef DOT
    $(error "dot is not available please install graphviz")
endif
    dot -Tpdf -o pres.pdf pres.dot 

ifeq ($(TARGET), android)
 $(info Android)
 CC=arm-linux-androideabi-gcc
else
 $(info native build)
 CC=something else
endif
