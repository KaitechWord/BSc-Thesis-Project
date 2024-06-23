script_folder="/home/kajtekk/Documents/code/BSc-Thesis-Project/build/dependencies"
echo "echo Restoring environment" > "$script_folder/deactivate_conanrunenv-debug-x86_64.sh"
for v in ACLOCAL_PATH AUTOMAKE_CONAN_INCLUDES PATH M4 ALSA_CONFIG_DIR OPENSSL_MODULES
do
    is_defined="true"
    value=$(printenv $v) || is_defined="" || true
    if [ -n "$value" ] || [ -n "$is_defined" ]
    then
        echo export "$v='$value'" >> "$script_folder/deactivate_conanrunenv-debug-x86_64.sh"
    else
        echo unset $v >> "$script_folder/deactivate_conanrunenv-debug-x86_64.sh"
    fi
done


export ACLOCAL_PATH="$ACLOCAL_PATH:/home/kajtekk/.conan2/p/b/libto7c8b19ede4173/p/res/aclocal"
export AUTOMAKE_CONAN_INCLUDES="$AUTOMAKE_CONAN_INCLUDES:/home/kajtekk/.conan2/p/b/libto7c8b19ede4173/p/res/aclocal"
export PATH="/home/kajtekk/.conan2/p/autom480a421c82a75/p/bin:/home/kajtekk/.conan2/p/autocf2af015330354/p/bin:/home/kajtekk/.conan2/p/b/m4ae55c539fc216/p/bin:$PATH"
export M4="/home/kajtekk/.conan2/p/b/m4ae55c539fc216/p/bin/m4"
export ALSA_CONFIG_DIR="/home/kajtekk/.conan2/p/b/libalf1fc6e1c6bca4/p/res/alsa"
export OPENSSL_MODULES="/home/kajtekk/.conan2/p/b/opens24dcde72b5eee/p/lib/ossl-modules"