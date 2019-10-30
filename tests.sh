
#!/bin/bash

tests="min malloc10"
failed=0

for test in $tests; do
    echo "$test"
    in="./tests/$test/in.txt"
    out="./tests/$test/out.txt"
    exp="./tests/$test/exp.txt"
    ./env.out < $in > $out
    
    echo "    output"
    sed -n l $out
    echo "    expected"
    sed -n l $exp
    
    if cmp -s "$exp" "$out"; then
        echo "  passed"
    else
        echo "  failed"
        failed=1
        echo "    output"
        sed -n l $out
        echo "    expected"
        sed -n l $exp
    fi
done

exit $failed
