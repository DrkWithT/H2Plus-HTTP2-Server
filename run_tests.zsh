# run_tests.zsh
# Derek Tan

test_prgm_name=''

# Run all unit test programs and track statuses.
for prgm in $(find -f ./bin/test_*);
do
    # Log debug msgs of stdout and stderr. 
    $prgm && echo -e "\033[0;32mPASS: $prgm\033[0m\n" || echo -e "\033[0;31mFAIL: $prgm\033[0m\n"
done

echo -e "\n\033[0;32mAll tests ran.\033[0m"
