
#!/bin/bash

tests="min malloc10"

for test in $tests; do
    echo "$test"
    in="./tests/$test/in.txt"
    out="./tests/$test/out.txt"
    exp="./tests/$test/exp.txt"
    ./env.out < $in > $out
    if cmp -s "$exp" "$out"; then
        echo "  passed"
    else
        echo "  failed"
    fi
done
