# project.zsh
# By: Derek Tan

debug_option='y';
debug_flag=1;

# prompt for build mode
read -p "Do debug build (y/n)? " debug_option

if [[ $debug_option == "y" ]]
then
    debug_flag=1
else
    debug_flag=0
fi

# Do following tasks:
# 1. clean old execs
# 2. attempt to build project
# 3. print msgs. of success / failure
make clean && make DEBUG_BUILD=$debug_flag all && echo -e "\033[0;32mBuild succeeded!\033[0m" || echo -e "\033[0;31mBuild failed!\033[0m"
