# run_tests.zsh
# Derek Tan

test_prgm_name=''

# Clear debug log to remove obsolete info.
echo "" > debug_log.txt

# Run all unit test programs and track how many pass / fail.
for prgm in $(find -f ./bin/test_*);
do
    # Log debug msgs of stdout and stderr. 
    exec $prgm >> debug_log.txt 2>&1

    # Print test prgm statuses.
    if [[ $? -eq 0 ]]
    then
        echo -e "\033[0;32mPASS: $prgm\033[0m\n"
    else
        echo -e "\033[0;31mFAIL: $prgm\033[0m\n"
    fi
done

echo -e "\n\033[0;32mAll tests ran: $pass_count passed, $fail_count failed.\033[0m"
