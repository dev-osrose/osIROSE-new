#!/usr/bin/env bash
set -euo pipefail
IFS=$'\n\t'

function test_dep() {
    local dep=$1;shift
    if ! eval "${dep}" --version > /dev/null
    then
        echo "missing ${dep} dependency"
        return 1
    fi
    return 0
}

readonly deps=('cmake' 'gcc-5' 'g++-5' 'unzip' 'autoconf' 'python' 'ruby')
declare -a mising_dep
stat=0

for dep in ${deps[@]}
do
    test_dep ${dep}
    rc=$?
    if [[ ${rc} -ne 0 ]]
    then
        missing_dep=("${missing_dep[@]}" ${dep})
    fi
    stat=${stat} || ${rc}
done

if [[ ${stat} -ne 0 ]]
then
    echo "Missing dependencies"
    case $(lsb_release -si) in
        "Ubuntu"|"Debian") source install_dep_deb.sh;;
        "Arch"|"Manjaro") source install_dep_arch.sh;;
        default)
            echo "Please install ${missing_dep[@]}"
            exit 1;;
    esac
    if ! pre_install
    then
        echo "Initialisation failed" > &2
    fi
    echo "Installation command may ask your password"
    for dep in ${missing_dep[@]}
    do
        if ! install ${dep}
        then
            echo "Failed to install ${dep}" > &2
            exit 2
        fi
    done
    exit 1
fi

echo "All dependencies satisfied"

exit 0
