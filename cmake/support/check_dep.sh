#!/usr/bin/env bash
set -euo pipefail
IFS=$'\n\t'

function test_dep() {
    local dep=$1;shift
    if ! type ${dep} > /dev/null 2>&1
    then
        echo "missing ${dep} dependency"
        return 1
    fi
    return 0
}

readonly deps=('cmake' 'gcc-5' 'g++-5' 'unzip' 'autoconf' 'python' 'ruby' 'yad')
declare -a mising_dep
stat=0

for dep in ${deps[@]}
do
    if ! test_dep "${dep}"
    then
        missing_dep=("${missing_dep[@]}" ${dep})
        stat=1
    fi
done

echo "stat=${stat}"
if [[ ${stat} -ne 0 ]]
then
    echo "Missing dependencies"
    distro=$(lsb_release -si)
    case ${distro} in
        "Ubuntu"|"Debian")
            echo "Using debian package manager"
            source scripts/install_dep_deb.sh
            ;;
        "Arch"|"ManjaroLinux")
            echo "Using arch package manager"
            source scripts/install_dep_arch.sh
            ;;
        default)
            echo "Please install ${missing_dep[*]}"
            exit 1;;
    esac
    if ! pre_install
    then
        echo "Initialisation failed" >&2
        exit 1
    fi
    echo "Installation command may ask your password"
    for dep in ${missing_dep[@]}
    do
        if ! install_dep "${dep}"
        then
            echo "Failed to install ${dep}" >&2
            exit 2
        fi
    done
    exit 1
fi

echo "All dependencies satisfied"

exit 0
