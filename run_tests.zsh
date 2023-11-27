# run_tests.zsh
# Derek Tan

pass_count=0
fail_count=0
test_prgm_name=''

# Run all unit test programs and track how many pass / fail.
for prgm in $(find -f ./bin/test_*);
do
    exec $test_prgm_name

    # Use an arithmetic expansion to increment pass/fail counts.
    if [[ $? -eq 0 ]]
    then
        pass_count=$((pass_count + 1))
    else
        fail_count=$((fail_count + 1))
    fi
done

echo -e "\n\033[0;32mAll tests ran: $pass_count passed, $fail_count failed.\033[0m"
