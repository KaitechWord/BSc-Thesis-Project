script_folder="/home/kajtekk/Documents/code/BSc-Thesis-Project/build/dependencies"
echo "echo Restoring environment" > "$script_folder/deactivate_conanbuildenv-debug-x86_64.sh"
for v in ACLOCAL_PATH AUTOMAKE_CONAN_INCLUDES AUTOCONF AUTORECONF AUTOHEADER AUTOM4TE M4
do
    is_defined="true"
    value=$(printenv $v) || is_defined="" || true
    if [ -n "$value" ] || [ -n "$is_defined" ]
    then
        echo export "$v='$value'" >> "$script_folder/deactivate_conanbuildenv-debug-x86_64.sh"
    else
        echo unset $v >> "$script_folder/deactivate_conanbuildenv-debug-x86_64.sh"
    fi
done


export ACLOCAL_PATH="$ACLOCAL_PATH:/home/kajtekk/.conan2/p/b/libto7c8b19ede4173/p/res/aclocal"
export AUTOMAKE_CONAN_INCLUDES="$AUTOMAKE_CONAN_INCLUDES:/home/kajtekk/.conan2/p/b/libto7c8b19ede4173/p/res/aclocal"
export AUTOCONF="/home/kajtekk/.conan2/p/autocf2af015330354/p/bin/autoconf"
export AUTORECONF="/home/kajtekk/.conan2/p/autocf2af015330354/p/bin/autoreconf"
export AUTOHEADER="/home/kajtekk/.conan2/p/autocf2af015330354/p/bin/autoheader"
export AUTOM4TE="/home/kajtekk/.conan2/p/autocf2af015330354/p/bin/autom4te"
export M4="/home/kajtekk/.conan2/p/b/m4ae55c539fc216/p/bin/m4"